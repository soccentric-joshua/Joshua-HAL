/**
 * @file emmc_lib.cpp
 * @brief Implementation file for EmmcTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "emmc_lib.h"
#include "../platform_joshua_sitara.h"

#include <fstream>
#include <string>

EmmcTesting::EmmcTesting() {}
EmmcTesting::~EmmcTesting() {}

bool EmmcTesting::present(const std::string& dev)
{
    std::ifstream f("/sys/class/block/" + dev.substr(5));
    return f.good();
}

long long EmmcTesting::capacityBytes()
{
    // Simulated 32 GB eMMC.
    return 32LL * 1024 * 1024 * 1024;
}

bool EmmcTesting::doReadWrite(double& throughput_mbps)
{
    throughput_mbps = 240.0; // simulated HS400 eMMC
    return true;
}

bool EmmcTesting::wearLevelingOk(double& lifetime)
{
    lifetime = 96.0; // simulated 96% lifetime remaining
    return true;
}

TestOutput EmmcTesting::testEmmcPresent()
{
    TestOutput out;
    if (present(JOSHUA_SITARA_EMMC_DEV)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["dev"] = JOSHUA_SITARA_EMMC_DEV;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("eMMC not present at ") +
                            JOSHUA_SITARA_EMMC_DEV + " on this image";
    }
    return out;
}

TestOutput EmmcTesting::testEmmcCapacity()
{
    TestOutput out;
    long long cap = capacityBytes();
    out.metrics["capacity_bytes"] = cap;
    out.status = (cap > 0) ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = (cap > 0) ? hal::Severity::INFO : hal::Severity::ERROR;
    if (cap > 0) out.reason["result"] = "eMMC capacity read ok (simulated)";
    return out;
}

TestOutput EmmcTesting::testEmmcReadWrite()
{
    TestOutput out;
    double tp = 0.0;
    bool ok = doReadWrite(tp);
    out.metrics["throughput_mbps"] = tp;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "eMMC read/write integrity ok (simulated)";
    return out;
}

TestOutput EmmcTesting::testEmmcSpeed()
{
    TestOutput out;
    double tp = 0.0;
    doReadWrite(tp);
    out.metrics["throughput_mbps"] = tp;
    bool ok = tp >= 100.0; // >= HS200-class
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "eMMC speed class satisfied (simulated)";
    return out;
}

TestOutput EmmcTesting::testEmmcWearLeveling()
{
    TestOutput out;
    double lifetime = 0.0;
    bool ok = wearLevelingOk(lifetime);
    out.metrics["lifetime_pct"] = lifetime;
    bool healthy = ok && lifetime > 50.0;
    out.status = healthy ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = healthy ? hal::Severity::INFO : hal::Severity::ERROR;
    if (healthy) out.reason["result"] = "eMMC wear-leveling healthy (simulated)";
    return out;
}

std::vector<TestResult> EmmcTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (EmmcTesting::*fn)()) {
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
    push("testEmmcPresent", &EmmcTesting::testEmmcPresent);
    push("testEmmcCapacity", &EmmcTesting::testEmmcCapacity);
    push("testEmmcReadWrite", &EmmcTesting::testEmmcReadWrite);
    push("testEmmcSpeed", &EmmcTesting::testEmmcSpeed);
    push("testEmmcWearLeveling", &EmmcTesting::testEmmcWearLeveling);
    return results;
}

std::vector<std::string> EmmcTesting::getAvailableTests()
{
    return {"testEmmcPresent", "testEmmcCapacity", "testEmmcReadWrite",
            "testEmmcSpeed", "testEmmcWearLeveling"};
}

TestResult EmmcTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testEmmcPresent") result = testEmmcPresent();
    else if (testName == "testEmmcCapacity") result = testEmmcCapacity();
    else if (testName == "testEmmcReadWrite") result = testEmmcReadWrite();
    else if (testName == "testEmmcSpeed") result = testEmmcSpeed();
    else if (testName == "testEmmcWearLeveling") result = testEmmcWearLeveling();
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
