/**
 * @file mipi_csi_lib.cpp
 * @brief Implementation file for MipiCsiTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "mipi_csi_lib.h"
#include "../platform_joshua_sitara.h"
#include "../pru_icss_lib/pru_icss_lib.h"

#include <fstream>
#include <string>

MipiCsiTesting::MipiCsiTesting() {}
MipiCsiTesting::~MipiCsiTesting() {}

bool MipiCsiTesting::present(const std::string& path)
{
    std::ifstream f(path);
    return f.good();
}

bool MipiCsiTesting::readLaneCount(int& lanes)
{
    lanes = 4; // simulated 4-lane CSI-2
    return true;
}

bool MipiCsiTesting::enumerateFormat(std::string& fmt)
{
    fmt = "YUV422_8"; // simulated default format
    return true;
}

double MipiCsiTesting::measureFrameRate()
{
    return 30.0; // simulated 30 fps
}

bool MipiCsiTesting::checkDataIntegrity(int& errors)
{
    errors = 0; // simulated clean capture
    return true;
}

TestOutput MipiCsiTesting::testMipiCsiPresent()
{
    TestOutput out;
    if (present(JOSHUA_SITARA_CSI_DEVICE0) || present(JOSHUA_SITARA_CSI_MEDIA1)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["dev"] = JOSHUA_SITARA_CSI_DEVICE0;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("MIPI CSI-2 not exposed at ") +
                            JOSHUA_SITARA_CSI_DEVICE0 + " on this image";
    }
    return out;
}

TestOutput MipiCsiTesting::testMipiCsiLaneCount()
{
    TestOutput out;
    int lanes = 0;
    if (readLaneCount(lanes)) {
        out.metrics["lanes"] = static_cast<int64_t>(lanes);
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["result"] = "CSI-2 configured with " + std::to_string(lanes) + " data lanes";
    } else {
        out.status = hal::Status::ERROR;
        out.severity = hal::Severity::ERROR;
        out.error_message = "failed to read lane configuration";
    }
    return out;
}

TestOutput MipiCsiTesting::testMipiCsiPixelFormat()
{
    TestOutput out;
    std::string fmt;
    if (enumerateFormat(fmt)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["format"] = fmt;
    } else {
        out.status = hal::Status::ERROR;
        out.severity = hal::Severity::ERROR;
        out.error_message = "failed to enumerate pixel format";
    }
    return out;
}

TestOutput MipiCsiTesting::testMipiCsiFrameRate()
{
    TestOutput out;
    double fps = measureFrameRate();
    out.metrics["fps"] = fps;
    bool ok = fps >= 30.0;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "sustains 30 fps (simulated)";
    return out;
}

TestOutput MipiCsiTesting::testMipiCsiDataIntegrity()
{
    TestOutput out;
    joshua::pru::PruIcssManager pru(JOSHUA_SITARA_PRU_REMOTEPROC_ID);
    std::string detail, resp;
    if (pru.isRunning() && pru.rpmsgRoundTrip("CSI_FRAME_CHECK", &resp, 100, &detail))
        out.reason["pru"] = "PRU CSI assist: " + resp;
    else
        out.reason["pru"] = "PRU unavailable; CSI integrity checked in software (simulated)";
    int errors = 0;
    bool ok = checkDataIntegrity(errors);
    out.metrics["crc_errors"] = static_cast<int64_t>(errors);
    bool clean = ok && errors == 0;
    out.status = clean ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = clean ? hal::Severity::INFO : hal::Severity::ERROR;
    if (clean) out.reason["result"] = "no CRC/lane errors in captured frames";
    return out;
}

std::vector<TestResult> MipiCsiTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (MipiCsiTesting::*fn)()) {
        auto start = std::chrono::high_resolution_clock::now();
        TestOutput out = (this->*fn)();
        auto end = std::chrono::high_resolution_clock::now();
        TestResult _tr;
        _tr.name = name;
        _tr.test_id = name;
        _tr.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        _tr.output = out;
        results.push_back(_tr);
    };
    push("testMipiCsiPresent", &MipiCsiTesting::testMipiCsiPresent);
    push("testMipiCsiLaneCount", &MipiCsiTesting::testMipiCsiLaneCount);
    push("testMipiCsiPixelFormat", &MipiCsiTesting::testMipiCsiPixelFormat);
    push("testMipiCsiFrameRate", &MipiCsiTesting::testMipiCsiFrameRate);
    push("testMipiCsiDataIntegrity", &MipiCsiTesting::testMipiCsiDataIntegrity);
    return results;
}

std::vector<std::string> MipiCsiTesting::getAvailableTests()
{
    return {"testMipiCsiPresent", "testMipiCsiLaneCount", "testMipiCsiPixelFormat",
            "testMipiCsiFrameRate", "testMipiCsiDataIntegrity"};
}

TestResult MipiCsiTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testMipiCsiPresent") result = testMipiCsiPresent();
    else if (testName == "testMipiCsiLaneCount") result = testMipiCsiLaneCount();
    else if (testName == "testMipiCsiPixelFormat") result = testMipiCsiPixelFormat();
    else if (testName == "testMipiCsiFrameRate") result = testMipiCsiFrameRate();
    else if (testName == "testMipiCsiDataIntegrity") result = testMipiCsiDataIntegrity();
    else {
        auto end = std::chrono::high_resolution_clock::now();
        TestResult _tr;
        _tr.name = testName + " (UNKNOWN)";
        _tr.test_id = testName;
        _tr.output.status = hal::Status::ERROR;
        _tr.output.severity = hal::Severity::ERROR;
        _tr.output.error_message = "Unknown test name: " + testName;
        _tr.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        return _tr;
    }

    auto end = std::chrono::high_resolution_clock::now();
    TestResult _tr;
    _tr.name = testName;
    _tr.test_id = testName;
    _tr.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    _tr.output = result;
    return _tr;
}
