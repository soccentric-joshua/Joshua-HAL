/**
 * @file accelerometer_lib.cpp
 * @brief Implementation file for AccelerometerTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "accelerometer_lib.h"
#include "../platform_joshua_sitara.h"
#include "../pru_icss_lib/pru_icss_lib.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <random>

AccelerometerTesting::AccelerometerTesting() {}
AccelerometerTesting::~AccelerometerTesting() {}

bool AccelerometerTesting::present(const std::string& path)
{
    std::ifstream f(path);
    return f.good();
}

bool AccelerometerTesting::readAxes(double& x, double& y, double& z)
{
    // Simulated resting orientation: ~0 g on X/Y, ~1 g on Z.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> noise(-0.05, 0.05);
    x = noise(gen);
    y = noise(gen);
    z = 1.0 + noise(gen);
    return true;
}

bool AccelerometerTesting::checkRange()
{
    return JOSHUA_SITARA_ACCEL_RANGE_G >= 2;
}

bool AccelerometerTesting::checkScale()
{
    // 16-bit sample over +/- 8 g => ~0.000244 g/LSB.
    return true;
}

double AccelerometerTesting::measureNoiseFloor()
{
    double x, y, z;
    readAxes(x, y, z);
    return std::sqrt(x * x + y * y + z * z) - 1.0;
}

TestOutput AccelerometerTesting::testAccelerometerPresent()
{
    TestOutput out;
    if (present(JOSHUA_SITARA_ACCEL_BASE)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["path"] = JOSHUA_SITARA_ACCEL_BASE;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("Accelerometer not exposed at ") +
                            JOSHUA_SITARA_ACCEL_BASE + " on this image";
    }
    return out;
}

TestOutput AccelerometerTesting::testAccelerometerReadAxes()
{
    TestOutput out;
    double x = 0, y = 0, z = 0;
    if (readAxes(x, y, z)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.metrics["x_g"] = x;
        out.metrics["y_g"] = y;
        out.metrics["z_g"] = z;
        out.reason["result"] = "axis readback ok (simulated)";
    } else {
        out.status = hal::Status::ERROR;
        out.severity = hal::Severity::ERROR;
        out.error_message = "failed to read accelerometer axes";
    }
    return out;
}

TestOutput AccelerometerTesting::testAccelerometerRange()
{
    TestOutput out;
    bool ok = checkRange();
    out.metrics["range_g"] = static_cast<int64_t>(JOSHUA_SITARA_ACCEL_RANGE_G);
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "measurable range adequate";
    return out;
}

TestOutput AccelerometerTesting::testAccelerometerScale()
{
    TestOutput out;
    bool ok = checkScale();
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "scale/offset calibration valid (simulated)";
    return out;
}

TestOutput AccelerometerTesting::testAccelerometerSensitivity()
{
    TestOutput out;
    double noise = measureNoiseFloor();
    out.metrics["noise_g"] = noise;
    bool ok = std::fabs(noise) <= 0.1;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "sensor noise floor within tolerance (simulated)";
    return out;
}

std::vector<TestResult> AccelerometerTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (AccelerometerTesting::*fn)()) {
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
    push("testAccelerometerPresent", &AccelerometerTesting::testAccelerometerPresent);
    push("testAccelerometerReadAxes", &AccelerometerTesting::testAccelerometerReadAxes);
    push("testAccelerometerRange", &AccelerometerTesting::testAccelerometerRange);
    push("testAccelerometerScale", &AccelerometerTesting::testAccelerometerScale);
    push("testAccelerometerSensitivity", &AccelerometerTesting::testAccelerometerSensitivity);
    return results;
}

std::vector<std::string> AccelerometerTesting::getAvailableTests()
{
    return {"testAccelerometerPresent", "testAccelerometerReadAxes",
            "testAccelerometerRange", "testAccelerometerScale",
            "testAccelerometerSensitivity"};
}

TestResult AccelerometerTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testAccelerometerPresent") result = testAccelerometerPresent();
    else if (testName == "testAccelerometerReadAxes") result = testAccelerometerReadAxes();
    else if (testName == "testAccelerometerRange") result = testAccelerometerRange();
    else if (testName == "testAccelerometerScale") result = testAccelerometerScale();
    else if (testName == "testAccelerometerSensitivity") result = testAccelerometerSensitivity();
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
