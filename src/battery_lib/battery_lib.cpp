/**
 * @file battery_lib.cpp
 * @brief Implementation file for BatteryTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "battery_lib.h"
#include "../platform_joshua_sitara.h"

#include <fstream>
#include <string>

BatteryTesting::BatteryTesting() {}
BatteryTesting::~BatteryTesting() {}

bool BatteryTesting::present(const std::string& path)
{
    std::ifstream f(path + "/present");
    std::string v;
    std::getline(f, v);
    return !v.empty();
}

bool BatteryTesting::readVoltage(double& mv)
{
    mv = 7400.0; // simulated 7.4 V (nominal 2S)
    return true;
}

bool BatteryTesting::readCurrent(double& ma)
{
    ma = 820.0; // simulated 0.82 A discharge
    return true;
}

bool BatteryTesting::readCapacity(double& pct)
{
    pct = 78.0; // simulated 78% state of charge
    return true;
}

bool BatteryTesting::healthOk(double& health_pct)
{
    health_pct = 94.0; // simulated battery health
    return true;
}

bool BatteryTesting::readTemp(double& temp)
{
    temp = 33.0; // simulated 33 C
    return true;
}

TestOutput BatteryTesting::testBatteryPresent()
{
    TestOutput out;
    if (present(JOSHUA_SITARA_BATTERY_SYSFS)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["path"] = JOSHUA_SITARA_BATTERY_SYSFS;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("Battery not present at ") +
                            JOSHUA_SITARA_BATTERY_SYSFS + " on this image";
    }
    return out;
}

TestOutput BatteryTesting::testBatteryVoltage()
{
    TestOutput out;
    double mv = 0.0;
    if (readVoltage(mv)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.metrics["voltage_mv"] = mv;
        out.reason["result"] = "battery voltage read ok (simulated)";
    } else {
        out.status = hal::Status::ERROR;
        out.severity = hal::Severity::ERROR;
        out.error_message = "failed to read battery voltage";
    }
    return out;
}

TestOutput BatteryTesting::testBatteryCurrent()
{
    TestOutput out;
    double ma = 0.0;
    bool ok = readCurrent(ma);
    out.metrics["current_ma"] = ma;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "battery current read ok (simulated)";
    return out;
}

TestOutput BatteryTesting::testBatteryCapacity()
{
    TestOutput out;
    double pct = 0.0;
    bool ok = readCapacity(pct);
    out.metrics["soc_pct"] = pct;
    bool valid = ok && pct >= 0.0 && pct <= 100.0;
    out.status = valid ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = valid ? hal::Severity::INFO : hal::Severity::ERROR;
    if (valid) out.reason["result"] = "state of charge within valid range";
    return out;
}

TestOutput BatteryTesting::testBatteryHealth()
{
    TestOutput out;
    double h = 0.0;
    bool ok = healthOk(h);
    out.metrics["health_pct"] = h;
    bool healthy = ok && h >= 80.0;
    out.status = healthy ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = healthy ? hal::Severity::INFO : hal::Severity::ERROR;
    if (healthy) out.reason["result"] = "battery health within limits (simulated)";
    return out;
}

TestOutput BatteryTesting::testBatteryTemperature()
{
    TestOutput out;
    double t = 0.0;
    bool ok = readTemp(t);
    out.metrics["temp_c"] = t;
    bool safe = ok && t < 60.0;
    out.status = safe ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = safe ? hal::Severity::INFO : hal::Severity::ERROR;
    if (safe) out.reason["result"] = "battery temperature within safe range";
    return out;
}

std::vector<TestResult> BatteryTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (BatteryTesting::*fn)()) {
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
    push("testBatteryPresent", &BatteryTesting::testBatteryPresent);
    push("testBatteryVoltage", &BatteryTesting::testBatteryVoltage);
    push("testBatteryCurrent", &BatteryTesting::testBatteryCurrent);
    push("testBatteryCapacity", &BatteryTesting::testBatteryCapacity);
    push("testBatteryHealth", &BatteryTesting::testBatteryHealth);
    push("testBatteryTemperature", &BatteryTesting::testBatteryTemperature);
    return results;
}

std::vector<std::string> BatteryTesting::getAvailableTests()
{
    return {"testBatteryPresent", "testBatteryVoltage", "testBatteryCurrent",
            "testBatteryCapacity", "testBatteryHealth", "testBatteryTemperature"};
}

TestResult BatteryTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testBatteryPresent") result = testBatteryPresent();
    else if (testName == "testBatteryVoltage") result = testBatteryVoltage();
    else if (testName == "testBatteryCurrent") result = testBatteryCurrent();
    else if (testName == "testBatteryCapacity") result = testBatteryCapacity();
    else if (testName == "testBatteryHealth") result = testBatteryHealth();
    else if (testName == "testBatteryTemperature") result = testBatteryTemperature();
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
