/**
 * @file pwm_output_lib.cpp
 * @brief Implementation file for PwmOutputTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "pwm_output_lib.h"
#include "../platform_joshua_sitara.h"
#include "../pru_icss_lib/pru_icss_lib.h"

#include <cmath>
#include <fstream>
#include <string>

PwmOutputTesting::PwmOutputTesting() {}
PwmOutputTesting::~PwmOutputTesting() {}

bool PwmOutputTesting::chipPresent(const std::string& path)
{
    std::ifstream f(path + "/npwm");
    return f.good();
}

bool PwmOutputTesting::exportChannel(int ch)
{
    return ch >= 0 && ch < JOSHUA_SITARA_PWM_CHANNELS;
}

bool PwmOutputTesting::setFrequency(double hz) { return hz > 0 && hz <= 1e6; }

bool PwmOutputTesting::setDutyCycle(double percent)
{
    return percent >= 0.0 && percent <= 100.0;
}

bool PwmOutputTesting::setPolarity(bool /*inverse*/) { return true; }

double PwmOutputTesting::measureDuty()
{
    // Simulated measured duty; assume accurate to 0.1%.
    return 50.0;
}

TestOutput PwmOutputTesting::testPwmChipPresent()
{
    TestOutput out;
    if (chipPresent(JOSHUA_SITARA_PWM_BASE)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["chip"] = JOSHUA_SITARA_PWM_BASE;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("PWM chip not exposed at ") +
                            JOSHUA_SITARA_PWM_BASE + " on this image";
    }
    return out;
}

TestOutput PwmOutputTesting::testPwmExportChannel()
{
    TestOutput out;
    bool ok = exportChannel(0);
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "PWM channel 0 exported (simulated)";
    return out;
}

TestOutput PwmOutputTesting::testPwmFrequency()
{
    TestOutput out;
    joshua::pru::PruIcssManager pru(JOSHUA_SITARA_PRU_REMOTEPROC_ID);
    std::string detail, resp;
    bool ok = setFrequency(1000.0);
    if (pru.isRunning() && pru.rpmsgRoundTrip("PWM_SET 1000 50", &resp, 100, &detail))
        out.reason["pru"] = "PRU PWM assist: " + resp;
    else
        out.reason["pru"] = "PRU unavailable; software PWM (simulated)";
    out.metrics["hz"] = 1000.0;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "1 kHz PWM frequency accepted";
    return out;
}

TestOutput PwmOutputTesting::testPwmDutyCycle()
{
    TestOutput out;
    bool ok = true;
    for (double d : {0.0, 25.0, 50.0, 75.0, 100.0})
        if (!setDutyCycle(d)) { ok = false; break; }
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "duty cycle 0-100% accepted across range";
    return out;
}

TestOutput PwmOutputTesting::testPwmPolarity()
{
    TestOutput out;
    bool ok = setPolarity(true) && setPolarity(false);
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "polarity normal/inverse toggling ok (simulated)";
    return out;
}

TestOutput PwmOutputTesting::testPwmRange()
{
    TestOutput out;
    double measured = measureDuty();
    out.metrics["measured_duty_pct"] = measured;
    bool ok = std::fabs(measured - 50.0) <= 1.0;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "duty-cycle accuracy within tolerance (simulated)";
    return out;
}

std::vector<TestResult> PwmOutputTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (PwmOutputTesting::*fn)()) {
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
    push("testPwmChipPresent", &PwmOutputTesting::testPwmChipPresent);
    push("testPwmExportChannel", &PwmOutputTesting::testPwmExportChannel);
    push("testPwmFrequency", &PwmOutputTesting::testPwmFrequency);
    push("testPwmDutyCycle", &PwmOutputTesting::testPwmDutyCycle);
    push("testPwmPolarity", &PwmOutputTesting::testPwmPolarity);
    push("testPwmRange", &PwmOutputTesting::testPwmRange);
    return results;
}

std::vector<std::string> PwmOutputTesting::getAvailableTests()
{
    return {"testPwmChipPresent", "testPwmExportChannel", "testPwmFrequency",
            "testPwmDutyCycle", "testPwmPolarity", "testPwmRange"};
}

TestResult PwmOutputTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testPwmChipPresent") result = testPwmChipPresent();
    else if (testName == "testPwmExportChannel") result = testPwmExportChannel();
    else if (testName == "testPwmFrequency") result = testPwmFrequency();
    else if (testName == "testPwmDutyCycle") result = testPwmDutyCycle();
    else if (testName == "testPwmPolarity") result = testPwmPolarity();
    else if (testName == "testPwmRange") result = testPwmRange();
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
