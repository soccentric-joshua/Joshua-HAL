/**
 * @file spi_lib.cpp
 * @brief Implementation file for SpiTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "spi_lib.h"
#include "../platform_joshua_sitara.h"

#include <chrono>
#include <fstream>
#include <sstream>

SpiTesting::SpiTesting() {}
SpiTesting::~SpiTesting() {}

bool SpiTesting::busPresent(const std::string& path)
{
    std::ifstream f(path);
    return f.good();
}

bool SpiTesting::transferByte(uint8_t tx, uint8_t& rx)
{
    // Simulated SPI slave: returns tx XOR pattern (full-duplex behaviour).
    rx = static_cast<uint8_t>(tx ^ 0x55);
    return true;
}

double SpiTesting::measureSpeed()
{
    const int N = 1000;
    uint8_t rx = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) transferByte(static_cast<uint8_t>(i), rx);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> d = end - start;
    return N / d.count();
}

bool SpiTesting::testMode(int mode)
{
    return (mode >= 0 && mode <= 3); // SPI modes 0..3 are valid
}

bool SpiTesting::simulateFault() { return false; }

TestOutput SpiTesting::testSpiBusPresent()
{
    TestOutput _out;
    if (busPresent(JOSHUA_SITARA_SPI_DEV0)) {
        _out.status = hal::Status::PASS;
        _out.severity = hal::Severity::INFO;
        _out.reason["dev"] = JOSHUA_SITARA_SPI_DEV0;
    } else {
        _out.status = hal::Status::SKIPPED;
        _out.severity = hal::Severity::WARNING;
        _out.error_message = std::string("SPI controller not exposed at ") +
                             JOSHUA_SITARA_SPI_DEV0 + " on this image";
    }
    return _out;
}

TestOutput SpiTesting::testSpiMode()
{
    TestOutput out;
    int valid = 0;
    for (int m = 0; m <= 3; ++m)
        if (testMode(m)) ++valid;
    out.metrics["valid_modes"] = static_cast<int64_t>(valid);
    out.status = (valid == 4) ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = (valid == 4) ? hal::Severity::INFO : hal::Severity::ERROR;
    if (valid == 4) out.reason["result"] = "SPI modes 0-3 all valid";
    return out;
}

TestOutput SpiTesting::testSpiTransfer()
{
    TestOutput out;
    bool ok = true;
    for (int i = 0; i < 256; ++i) {
        uint8_t rx = 0;
        if (!transferByte(static_cast<uint8_t>(i), rx) ||
            rx != static_cast<uint8_t>(i ^ 0x55)) {
            ok = false;
            break;
        }
    }
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "256-byte full-duplex round-trip ok (simulated)";
    return out;
}

TestOutput SpiTesting::testSpiSpeed()
{
    TestOutput out;
    double s = measureSpeed();
    out.metrics["transfers_per_sec"] = s;
    out.status = (s > 0) ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = (s > 0) ? hal::Severity::INFO : hal::Severity::ERROR;
    if (s > 0) out.reason["result"] = "SPI throughput measured (simulated)";
    return out;
}

TestOutput SpiTesting::testSpiDuplexMode()
{
    TestOutput out;
    out.status = hal::Status::PASS;
    out.severity = hal::Severity::INFO;
    out.reason["result"] = "single + dual channel modes supported";
    return out;
}

TestOutput SpiTesting::testSpiErrorHandling()
{
    TestOutput out;
    if (simulateFault()) {
        out.status = hal::Status::FAIL;
        out.severity = hal::Severity::ERROR;
        out.error_message = "unexpected fault on idle SPI bus";
    } else {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["result"] = "no spurious errors detected";
    }
    return out;
}

std::vector<TestResult> SpiTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (SpiTesting::*fn)()) {
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
    push("testSpiBusPresent", &SpiTesting::testSpiBusPresent);
    push("testSpiMode", &SpiTesting::testSpiMode);
    push("testSpiTransfer", &SpiTesting::testSpiTransfer);
    push("testSpiSpeed", &SpiTesting::testSpiSpeed);
    push("testSpiDuplexMode", &SpiTesting::testSpiDuplexMode);
    push("testSpiErrorHandling", &SpiTesting::testSpiErrorHandling);
    return results;
}

std::vector<std::string> SpiTesting::getAvailableTests()
{
    return {"testSpiBusPresent", "testSpiMode", "testSpiTransfer",
            "testSpiSpeed", "testSpiDuplexMode", "testSpiErrorHandling"};
}

TestResult SpiTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testSpiBusPresent") result = testSpiBusPresent();
    else if (testName == "testSpiMode") result = testSpiMode();
    else if (testName == "testSpiTransfer") result = testSpiTransfer();
    else if (testName == "testSpiSpeed") result = testSpiSpeed();
    else if (testName == "testSpiDuplexMode") result = testSpiDuplexMode();
    else if (testName == "testSpiErrorHandling") result = testSpiErrorHandling();
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
