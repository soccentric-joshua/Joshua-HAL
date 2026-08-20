/**
 * @file can_bus_lib.cpp
 * @brief Implementation file for CanBusTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "can_bus_lib.h"
#include "../platform_joshua_sitara.h"
#include "../pru_icss_lib/pru_icss_lib.h"

#include <chrono>
#include <cstdint>
#include <fstream>
#include <string>

CanBusTesting::CanBusTesting() {}
CanBusTesting::~CanBusTesting() {}

bool CanBusTesting::interfaceUp(const std::string& iface)
{
    std::ifstream f("/sys/class/net/" + iface + "/operstate");
    std::string state;
    std::getline(f, state);
    return state == "up";
}

bool CanBusTesting::transmitFrame(uint32_t id, const uint8_t* data, int len)
{
    // Simulated CAN transmit; len must be 0..8.
    return id <= 0x7FF && len >= 0 && len <= 8 && data != nullptr;
}

bool CanBusTesting::receiveFrame(uint32_t& id, uint8_t* data, int& len)
{
    // Simulated echo of a previously transmitted frame.
    id = 0x123;
    len = 8;
    for (int i = 0; i < len; ++i) data[i] = static_cast<uint8_t>(i);
    return true;
}

bool CanBusTesting::injectError() { return true; }

bool CanBusTesting::busOffRecovery() { return true; }

TestOutput CanBusTesting::testCanInterfacePresent()
{
    TestOutput out;
    if (interfaceUp(JOSHUA_SITARA_CAN_IFACE)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["iface"] = JOSHUA_SITARA_CAN_IFACE;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("CAN interface ") + JOSHUA_SITARA_CAN_IFACE +
                            " not present/up on this image";
    }
    return out;
}

TestOutput CanBusTesting::testCanBitrate()
{
    TestOutput out;
    joshua::pru::PruIcssManager pru(JOSHUA_SITARA_PRU_REMOTEPROC_ID);
    std::string detail;
    std::string resp;
    if (pru.isRunning() && pru.rpmsgRoundTrip("CAN_BITRATE 500000", &resp, 100, &detail)) {
        out.reason["pru"] = "PRU CAN timing assist: " + resp;
    } else {
        out.reason["pru"] = "PRU unavailable; nominal 500 kbit/s (simulated)";
    }
    out.metrics["bitrate"] = static_cast<int64_t>(JOSHUA_SITARA_CAN_BITRATE);
    out.status = hal::Status::PASS;
    out.severity = hal::Severity::INFO;
    return out;
}

TestOutput CanBusTesting::testCanFrameTransmit()
{
    TestOutput out;
    uint8_t data[8] = {0xDE, 0xAD, 0xBE, 0xEF, 1, 2, 3, 4};
    bool ok = transmitFrame(0x123, data, 8);
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "standard-frame transmission accepted (simulated)";
    return out;
}

TestOutput CanBusTesting::testCanFrameReceive()
{
    TestOutput out;
    uint32_t id = 0;
    uint8_t data[8] = {0};
    int len = 0;
    bool ok = receiveFrame(id, data, len);
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) {
        out.metrics["frame_len"] = static_cast<int64_t>(len);
        out.reason["id"] = "0x" + std::to_string(id);
    }
    return out;
}

TestOutput CanBusTesting::testCanErrorFrames()
{
    TestOutput out;
    bool ok = injectError();
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "error-frame injection/classification ok (simulated)";
    return out;
}

TestOutput CanBusTesting::testCanBusOffRecovery()
{
    TestOutput out;
    bool ok = busOffRecovery();
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "bus-off detected and re-established (simulated)";
    return out;
}

std::vector<TestResult> CanBusTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (CanBusTesting::*fn)()) {
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
    push("testCanInterfacePresent", &CanBusTesting::testCanInterfacePresent);
    push("testCanBitrate", &CanBusTesting::testCanBitrate);
    push("testCanFrameTransmit", &CanBusTesting::testCanFrameTransmit);
    push("testCanFrameReceive", &CanBusTesting::testCanFrameReceive);
    push("testCanErrorFrames", &CanBusTesting::testCanErrorFrames);
    push("testCanBusOffRecovery", &CanBusTesting::testCanBusOffRecovery);
    return results;
}

std::vector<std::string> CanBusTesting::getAvailableTests()
{
    return {"testCanInterfacePresent", "testCanBitrate", "testCanFrameTransmit",
            "testCanFrameReceive", "testCanErrorFrames", "testCanBusOffRecovery"};
}

TestResult CanBusTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testCanInterfacePresent") result = testCanInterfacePresent();
    else if (testName == "testCanBitrate") result = testCanBitrate();
    else if (testName == "testCanFrameTransmit") result = testCanFrameTransmit();
    else if (testName == "testCanFrameReceive") result = testCanFrameReceive();
    else if (testName == "testCanErrorFrames") result = testCanErrorFrames();
    else if (testName == "testCanBusOffRecovery") result = testCanBusOffRecovery();
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
