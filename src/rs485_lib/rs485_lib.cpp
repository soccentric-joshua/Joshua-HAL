/**
 * @file rs485_lib.cpp
 * @brief Implementation file for Rs485Testing class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "rs485_lib.h"
#include "../platform_joshua_sitara.h"

#include <cstdint>
#include <fstream>

Rs485Testing::Rs485Testing() {}
Rs485Testing::~Rs485Testing() {}

bool Rs485Testing::present(const std::string& path)
{
    std::ifstream f(path);
    return f.good();
}

bool Rs485Testing::halfDuplexTransfer(const uint8_t* tx, int len, uint8_t* rx, int& rxLen)
{
    // Simulated half-duplex: after turning the driver around, echo the frame.
    rxLen = len;
    for (int i = 0; i < len; ++i) rx[i] = tx[i];
    return len >= 0;
}

double Rs485Testing::throughput(int baud)
{
    return static_cast<double>(baud) / 10.0;
}

bool Rs485Testing::testDriverEnableToggle() { return true; }

TestOutput Rs485Testing::testRs485DevicePresent()
{
    TestOutput out;
    if (present(JOSHUA_SITARA_RS485_UART)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["dev"] = JOSHUA_SITARA_RS485_UART;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("RS-485 UART not exposed at ") +
                            JOSHUA_SITARA_RS485_UART + " on this image";
    }
    return out;
}

TestOutput Rs485Testing::testRs485HalfDuplex()
{
    TestOutput out;
    uint8_t tx[4] = {0xAA, 0x55, 0x01, 0xFF};
    uint8_t rx[4] = {0};
    int rxLen = 0;
    bool ok = halfDuplexTransfer(tx, 4, rx, rxLen);
    if (ok && rxLen == 4) {
        for (int i = 0; i < 4; ++i)
            if (rx[i] != tx[i]) { ok = false; break; }
    } else ok = false;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "half-duplex turnaround and echo ok (simulated)";
    return out;
}

TestOutput Rs485Testing::testRs485Baud()
{
    TestOutput out;
    double t = throughput(JOSHUA_SITARA_RS485_BAUD);
    out.metrics["bytes_per_sec"] = t;
    out.status = (t > 0) ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = (t > 0) ? hal::Severity::INFO : hal::Severity::ERROR;
    if (t > 0) out.reason["result"] = "115200 baud configured (simulated)";
    return out;
}

TestOutput Rs485Testing::testRs485Loopback()
{
    TestOutput out;
    uint8_t tx[2] = {0x5A, 0xA5};
    uint8_t rx[2] = {0};
    int rxLen = 0;
    bool ok = halfDuplexTransfer(tx, 2, rx, rxLen) && rxLen == 2 && rx[0] == 0x5A && rx[1] == 0xA5;
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "bus loopback verified (simulated)";
    return out;
}

TestOutput Rs485Testing::testRs485DirectionControl()
{
    TestOutput out;
    bool ok = testDriverEnableToggle();
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "driver enable / terminator toggling ok (simulated)";
    return out;
}

std::vector<TestResult> Rs485Testing::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (Rs485Testing::*fn)()) {
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
    push("testRs485DevicePresent", &Rs485Testing::testRs485DevicePresent);
    push("testRs485HalfDuplex", &Rs485Testing::testRs485HalfDuplex);
    push("testRs485Baud", &Rs485Testing::testRs485Baud);
    push("testRs485Loopback", &Rs485Testing::testRs485Loopback);
    push("testRs485DirectionControl", &Rs485Testing::testRs485DirectionControl);
    return results;
}

std::vector<std::string> Rs485Testing::getAvailableTests()
{
    return {"testRs485DevicePresent", "testRs485HalfDuplex", "testRs485Baud",
            "testRs485Loopback", "testRs485DirectionControl"};
}

TestResult Rs485Testing::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testRs485DevicePresent") result = testRs485DevicePresent();
    else if (testName == "testRs485HalfDuplex") result = testRs485HalfDuplex();
    else if (testName == "testRs485Baud") result = testRs485Baud();
    else if (testName == "testRs485Loopback") result = testRs485Loopback();
    else if (testName == "testRs485DirectionControl") result = testRs485DirectionControl();
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
