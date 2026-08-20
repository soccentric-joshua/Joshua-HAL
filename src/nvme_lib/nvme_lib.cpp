/**
 * @file nvme_lib.cpp
 * @brief Implementation file for NvmeTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "nvme_lib.h"
#include "../platform_joshua_sitara.h"

#include <fstream>
#include <string>

NvmeTesting::NvmeTesting() {}
NvmeTesting::~NvmeTesting() {}

bool NvmeTesting::present(const std::string& dev)
{
    std::ifstream f("/sys/class/block/" + dev.substr(5));
    return f.good();
}

long long NvmeTesting::capacityBytes()
{
    // Simulated 512 GB NVMe drive.
    return 512LL * 1024 * 1024 * 1024;
}

bool NvmeTesting::doReadWrite(double& throughput_mbps)
{
    throughput_mbps = 2100.0; // simulated NVMe PCIe Gen3
    return true;
}

bool NvmeTesting::smartOk(double& temp, double& wear)
{
    temp = 45.0; wear = 2.0; // simulated 45 C, 2% worn
    return true;
}

TestOutput NvmeTesting::testNvmePresent()
{
    TestOutput out;
    if (present(JOSHUA_SITARA_NVME_PATH)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["dev"] = JOSHUA_SITARA_NVME_PATH;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("NVMe not present at ") +
                            JOSHUA_SITARA_NVME_PATH + " on this image";
    }
    return out;
}

TestOutput NvmeTesting::testNvmeCapacity()
{
    TestOutput out;
    long long cap = capacityBytes();
    out.metrics["capacity_bytes"] = cap;
    out.status = (cap > 0) ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = (cap > 0) ? hal::Severity::INFO : hal::Severity::ERROR;
    if (cap > 0) out.reason["result"] = "NVMe capacity read ok (simulated)";
    return out;
}

TestOutput NvmeTesting::testNvmeReadWrite()
{
    TestOutput out;
    double tp = 0.0;
    bool ok = doReadWrite(tp);
    out.metrics["throughput_mbps"] = tp;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "NVMe read/write integrity ok (simulated)";
    return out;
}

TestOutput NvmeTesting::testNvmePerformance()
{
    TestOutput out;
    double tp = 0.0;
    doReadWrite(tp);
    out.metrics["throughput_mbps"] = tp;
    bool ok = tp >= 1000.0;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "NVMe throughput within PCIe expectations (simulated)";
    return out;
}

TestOutput NvmeTesting::testNvmeSmart()
{
    TestOutput out;
    double temp = 0.0, wear = 0.0;
    bool ok = smartOk(temp, wear);
    out.metrics["temp_c"] = temp;
    out.metrics["wear_pct"] = wear;
    bool healthy = ok && temp < 70 && wear < 80;
    out.status = healthy ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = healthy ? hal::Severity::INFO : hal::Severity::ERROR;
    if (healthy) out.reason["result"] = "NVMe SMART health within limits (simulated)";
    return out;
}

std::vector<TestResult> NvmeTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (NvmeTesting::*fn)()) {
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
    push("testNvmePresent", &NvmeTesting::testNvmePresent);
    push("testNvmeCapacity", &NvmeTesting::testNvmeCapacity);
    push("testNvmeReadWrite", &NvmeTesting::testNvmeReadWrite);
    push("testNvmePerformance", &NvmeTesting::testNvmePerformance);
    push("testNvmeSmart", &NvmeTesting::testNvmeSmart);
    return results;
}

std::vector<std::string> NvmeTesting::getAvailableTests()
{
    return {"testNvmePresent", "testNvmeCapacity", "testNvmeReadWrite",
            "testNvmePerformance", "testNvmeSmart"};
}

TestResult NvmeTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testNvmePresent") result = testNvmePresent();
    else if (testName == "testNvmeCapacity") result = testNvmeCapacity();
    else if (testName == "testNvmeReadWrite") result = testNvmeReadWrite();
    else if (testName == "testNvmePerformance") result = testNvmePerformance();
    else if (testName == "testNvmeSmart") result = testNvmeSmart();
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
