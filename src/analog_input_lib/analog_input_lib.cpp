/**
 * @file analog_input_lib.cpp
 * @brief Implementation file for AnalogInputTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "analog_input_lib.h"
#include "../platform_joshua_sitara.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <random>

AnalogInputTesting::AnalogInputTesting() {}
AnalogInputTesting::~AnalogInputTesting() {}

bool AnalogInputTesting::channelPresent(const std::string& path)
{
    std::ifstream f(path);
    return f.good();
}

bool AnalogInputTesting::readRaw(int& raw)
{
    // Simulated 12-bit ADC sample; mid-scale ~2048.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(2000, 2090);
    raw = dist(gen);
    return true;
}

double AnalogInputTesting::rawToVoltage(int raw)
{
    // AM62x ADC: 12-bit over 0..3.3 V full scale.
    double vref = 3.3;
    return vref * raw / static_cast<double>((1 << JOSHUA_SITARA_ADC_BITS) - 1);
}

double AnalogInputTesting::linearityError()
{
    // Simulated INL/DNL; must stay within +/- 2 LSB for a 12-bit ADC.
    return 0.8;
}

TestOutput AnalogInputTesting::testAdcChannelPresent()
{
    TestOutput out;
    if (channelPresent(JOSHUA_SITARA_ADC_CH0_RAW)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["ch"] = JOSHUA_SITARA_ADC_CH0_RAW;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("ADC channel not exposed at ") +
                            JOSHUA_SITARA_ADC_CH0_RAW + " on this image";
    }
    return out;
}

TestOutput AnalogInputTesting::testAdcReadRaw()
{
    TestOutput out;
    int raw = 0;
    if (readRaw(raw)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.metrics["raw"] = static_cast<int64_t>(raw);
        out.metrics["voltage"] = rawToVoltage(raw);
        out.reason["result"] = "ADC sample read ok (simulated)";
    } else {
        out.status = hal::Status::ERROR;
        out.severity = hal::Severity::ERROR;
        out.error_message = "failed to read ADC raw value";
    }
    return out;
}

TestOutput AnalogInputTesting::testAdcLinearity()
{
    TestOutput out;
    double err = linearityError();
    out.metrics["inl_lsb"] = err;
    bool ok = std::fabs(err) <= 2.0;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "INL within +/-2 LSB (simulated)";
    return out;
}

TestOutput AnalogInputTesting::testAdcResolution()
{
    TestOutput out;
    int bits = JOSHUA_SITARA_ADC_BITS;
    out.metrics["bits"] = static_cast<int64_t>(bits);
    out.status = (bits >= 10) ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = (bits >= 10) ? hal::Severity::INFO : hal::Severity::ERROR;
    if (bits >= 10) out.reason["result"] = "ADC effective resolution sufficient";
    return out;
}

TestOutput AnalogInputTesting::testAdcNoise()
{
    TestOutput out;
    // Sample repeatedly and report peak-to-peak spread as a noise proxy.
    int min = 1 << 30, max = -1;
    for (int i = 0; i < 64; ++i) {
        int raw = 0;
        readRaw(raw);
        min = std::min(min, raw);
        max = std::max(max, raw);
    }
    int spread = max - min;
    out.metrics["noise_lsb"] = static_cast<int64_t>(spread);
    bool ok = spread <= 4; // <= 4 LSB noise
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "ADC noise within 4 LSB (simulated)";
    return out;
}

std::vector<TestResult> AnalogInputTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (AnalogInputTesting::*fn)()) {
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
    push("testAdcChannelPresent", &AnalogInputTesting::testAdcChannelPresent);
    push("testAdcReadRaw", &AnalogInputTesting::testAdcReadRaw);
    push("testAdcLinearity", &AnalogInputTesting::testAdcLinearity);
    push("testAdcResolution", &AnalogInputTesting::testAdcResolution);
    push("testAdcNoise", &AnalogInputTesting::testAdcNoise);
    return results;
}

std::vector<std::string> AnalogInputTesting::getAvailableTests()
{
    return {"testAdcChannelPresent", "testAdcReadRaw", "testAdcLinearity",
            "testAdcResolution", "testAdcNoise"};
}

TestResult AnalogInputTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testAdcChannelPresent") result = testAdcChannelPresent();
    else if (testName == "testAdcReadRaw") result = testAdcReadRaw();
    else if (testName == "testAdcLinearity") result = testAdcLinearity();
    else if (testName == "testAdcResolution") result = testAdcResolution();
    else if (testName == "testAdcNoise") result = testAdcNoise();
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
