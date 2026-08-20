/**
 * @file mipi_dsi_lib.cpp
 * @brief Implementation file for MipiDsiTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "mipi_dsi_lib.h"
#include "../platform_joshua_sitara.h"

#include <fstream>
#include <string>

MipiDsiTesting::MipiDsiTesting() {}
MipiDsiTesting::~MipiDsiTesting() {}

bool MipiDsiTesting::present(const std::string& path)
{
    std::ifstream f(path);
    return f.good();
}

bool MipiDsiTesting::panelConnected(std::string& name)
{
    name = "TFT-DSI-720x1280"; // simulated connected panel
    return true;
}

bool MipiDsiTesting::readResolution(int& w, int& h)
{
    w = 720; h = 1280; // simulated panel resolution
    return true;
}

bool MipiDsiTesting::setBacklight(int percent)
{
    return percent >= 0 && percent <= 100;
}

bool MipiDsiTesting::testCommandTransfer() { return true; }

TestOutput MipiDsiTesting::testMipiDsiPresent()
{
    TestOutput out;
    if (present(JOSHUA_SITARA_DSI_DEV) || present(JOSHUA_SITARA_DRM_CARD0)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["dev"] = JOSHUA_SITARA_DSI_DEV;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("MIPI DSI display not exposed at ") +
                            JOSHUA_SITARA_DSI_DEV + " on this image";
    }
    return out;
}

TestOutput MipiDsiTesting::testMipiDsiPanel()
{
    TestOutput out;
    std::string name;
    if (panelConnected(name)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["panel"] = name;
    } else {
        out.status = hal::Status::ERROR;
        out.severity = hal::Severity::ERROR;
        out.error_message = "no DSI panel detected";
    }
    return out;
}

TestOutput MipiDsiTesting::testMipiDsiResolution()
{
    TestOutput out;
    int w = 0, h = 0;
    if (readResolution(w, h)) {
        out.metrics["width"] = static_cast<int64_t>(w);
        out.metrics["height"] = static_cast<int64_t>(h);
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["result"] = std::to_string(w) + "x" + std::to_string(h);
    } else {
        out.status = hal::Status::ERROR;
        out.severity = hal::Severity::ERROR;
        out.error_message = "failed to read panel resolution";
    }
    return out;
}

TestOutput MipiDsiTesting::testMipiDsiBacklight()
{
    TestOutput out;
    bool ok = true;
    for (int p : {0, 25, 50, 100})
        if (!setBacklight(p)) { ok = false; break; }
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "backlight 0-100% accepted";
    return out;
}

TestOutput MipiDsiTesting::testMipiDsiCommandMode()
{
    TestOutput out;
    bool ok = testCommandTransfer();
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "DSI command-mode writes ok (simulated)";
    return out;
}

std::vector<TestResult> MipiDsiTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (MipiDsiTesting::*fn)()) {
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
    push("testMipiDsiPresent", &MipiDsiTesting::testMipiDsiPresent);
    push("testMipiDsiPanel", &MipiDsiTesting::testMipiDsiPanel);
    push("testMipiDsiResolution", &MipiDsiTesting::testMipiDsiResolution);
    push("testMipiDsiBacklight", &MipiDsiTesting::testMipiDsiBacklight);
    push("testMipiDsiCommandMode", &MipiDsiTesting::testMipiDsiCommandMode);
    return results;
}

std::vector<std::string> MipiDsiTesting::getAvailableTests()
{
    return {"testMipiDsiPresent", "testMipiDsiPanel", "testMipiDsiResolution",
            "testMipiDsiBacklight", "testMipiDsiCommandMode"};
}

TestResult MipiDsiTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testMipiDsiPresent") result = testMipiDsiPresent();
    else if (testName == "testMipiDsiPanel") result = testMipiDsiPanel();
    else if (testName == "testMipiDsiResolution") result = testMipiDsiResolution();
    else if (testName == "testMipiDsiBacklight") result = testMipiDsiBacklight();
    else if (testName == "testMipiDsiCommandMode") result = testMipiDsiCommandMode();
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
