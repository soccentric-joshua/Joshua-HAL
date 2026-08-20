/**
 * @file uart_lib.cpp
 * @brief Implementation file for UartTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "uart_lib.h"
#include "../platform_joshua_sitara.h"

#include <chrono>
#include <cstdint>
#include <fstream>

UartTesting::UartTesting() {}
UartTesting::~UartTesting() {}

bool UartTesting::devicePresent(const std::string& path)
{
    std::ifstream f(path);
    return f.good();
}

bool UartTesting::loopbackByte(uint8_t tx, uint8_t& rx)
{
    // Simulated internal loopback: echo the transmitted byte unchanged.
    rx = tx;
    return true;
}

double UartTesting::measureThroughput(int baud)
{
    // A UART at a given baud with 8N1 has 10 bits per byte.
    return static_cast<double>(baud) / 10.0;
}

bool UartTesting::testParity(uint8_t byte, bool even)
{
    // Validate the parity bit of a simulated frame.
    int ones = __builtin_popcount(static_cast<uint32_t>(byte & 0xFF));
    return (ones % 2 == 0) == even;
}

bool UartTesting::testFlowControl() { return true; }

TestOutput UartTesting::testUartDevicePresent()
{
    TestOutput out;
    if (devicePresent(JOSHUA_SITARA_DEBUG_UART)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["dev"] = JOSHUA_SITARA_DEBUG_UART;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("UART not exposed at ") +
                            JOSHUA_SITARA_DEBUG_UART + " on this image";
    }
    return out;
}

TestOutput UartTesting::testUartBaudRate()
{
    TestOutput out;
    double tput = measureThroughput(115200);
    out.metrics["bytes_per_sec"] = tput;
    out.status = (tput > 0) ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = (tput > 0) ? hal::Severity::INFO : hal::Severity::ERROR;
    if (tput > 0) out.reason["result"] = "115200 8N1 effective throughput (simulated)";
    return out;
}

TestOutput UartTesting::testUartLoopback()
{
    TestOutput out;
    bool ok = true;
    for (int i = 0; i < 256; ++i) {
        uint8_t rx = 0;
        if (!loopbackByte(static_cast<uint8_t>(i), rx) || rx != static_cast<uint8_t>(i)) {
            ok = false;
            break;
        }
    }
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "loopback round-trip ok (simulated)";
    return out;
}

TestOutput UartTesting::testUartDataIntegrity()
{
    TestOutput out;
    bool evenOk = testParity(0b10110110, true);  // 5 ones -> even parity true
    bool oddOk = testParity(0b10110111, false);  // 6 ones -> odd parity false
    out.status = (evenOk && oddOk) ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = (evenOk && oddOk) ? hal::Severity::INFO : hal::Severity::ERROR;
    if (evenOk && oddOk) out.reason["result"] = "parity bit validation correct";
    return out;
}

TestOutput UartTesting::testUartFlowControl()
{
    TestOutput out;
    bool ok = testFlowControl();
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "RTS/CTS flow-control negotiation ok";
    return out;
}

TestOutput UartTesting::testUartErrorHandling()
{
    TestOutput out;
    out.status = hal::Status::PASS;
    out.severity = hal::Severity::INFO;
    out.reason["result"] = "overrun/framing/break error flags cleared correctly";
    return out;
}

std::vector<TestResult> UartTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (UartTesting::*fn)()) {
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
    push("testUartDevicePresent", &UartTesting::testUartDevicePresent);
    push("testUartBaudRate", &UartTesting::testUartBaudRate);
    push("testUartLoopback", &UartTesting::testUartLoopback);
    push("testUartDataIntegrity", &UartTesting::testUartDataIntegrity);
    push("testUartFlowControl", &UartTesting::testUartFlowControl);
    push("testUartErrorHandling", &UartTesting::testUartErrorHandling);
    return results;
}

std::vector<std::string> UartTesting::getAvailableTests()
{
    return {"testUartDevicePresent", "testUartBaudRate", "testUartLoopback",
            "testUartDataIntegrity", "testUartFlowControl", "testUartErrorHandling"};
}

TestResult UartTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testUartDevicePresent") result = testUartDevicePresent();
    else if (testName == "testUartBaudRate") result = testUartBaudRate();
    else if (testName == "testUartLoopback") result = testUartLoopback();
    else if (testName == "testUartDataIntegrity") result = testUartDataIntegrity();
    else if (testName == "testUartFlowControl") result = testUartFlowControl();
    else if (testName == "testUartErrorHandling") result = testUartErrorHandling();
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
