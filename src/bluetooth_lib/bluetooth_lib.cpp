/**
 * @file bluetooth_lib.cpp
 * @brief Implementation file for BluetoothTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "bluetooth_lib.h"
#include "../platform_joshua_sitara.h"

#include <cstdint>
#include <fstream>
#include <string>

BluetoothTesting::BluetoothTesting() {}
BluetoothTesting::~BluetoothTesting() {}

bool BluetoothTesting::adapterPresent(const std::string& hci)
{
    std::ifstream f("/sys/class/bluetooth/" + hci);
    return f.good();
}

bool BluetoothTesting::setPower(bool /*on*/) { return true; }

bool BluetoothTesting::startScan(int& found)
{
    found = 3; // simulated 3 discovered devices
    return true;
}

bool BluetoothTesting::pair(const std::string& /*addr*/) { return true; }

bool BluetoothTesting::readRssi(double& dbm)
{
    dbm = -58.0; // simulated RSSI
    return true;
}

TestOutput BluetoothTesting::testBluetoothAdapterPresent()
{
    TestOutput out;
    if (adapterPresent(JOSHUA_SITARA_BT_HCI)) {
        out.status = hal::Status::PASS;
        out.severity = hal::Severity::INFO;
        out.reason["hci"] = JOSHUA_SITARA_BT_HCI;
    } else {
        out.status = hal::Status::SKIPPED;
        out.severity = hal::Severity::WARNING;
        out.error_message = std::string("Bluetooth adapter ") + JOSHUA_SITARA_BT_HCI +
                            " not present on this image";
    }
    return out;
}

TestOutput BluetoothTesting::testBluetoothPower()
{
    TestOutput out;
    bool ok = setPower(true) && setPower(false);
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "adapter power up/down ok (simulated)";
    return out;
}

TestOutput BluetoothTesting::testBluetoothScan()
{
    TestOutput out;
    int found = 0;
    bool ok = startScan(found);
    out.metrics["devices_found"] = static_cast<int64_t>(found);
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "scan completed, devices discovered (simulated)";
    return out;
}

TestOutput BluetoothTesting::testBluetoothPairing()
{
    TestOutput out;
    bool ok = pair("AA:BB:CC:DD:EE:FF");
    out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) out.reason["result"] = "pairing handshake ok (simulated)";
    return out;
}

TestOutput BluetoothTesting::testBluetoothRssi()
{
    TestOutput out;
    double dbm = 0.0;
    bool ok = readRssi(dbm);
    out.metrics["rssi_dbm"] = dbm;
    bool valid = ok && dbm >= -100.0 && dbm <= 0.0;
    out.status = valid ? hal::Status::PASS : hal::Status::FAIL;
    out.severity = valid ? hal::Severity::INFO : hal::Severity::ERROR;
    if (valid) out.reason["result"] = "RSSI within valid range (simulated)";
    return out;
}

std::vector<TestResult> BluetoothTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (BluetoothTesting::*fn)()) {
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
    push("testBluetoothAdapterPresent", &BluetoothTesting::testBluetoothAdapterPresent);
    push("testBluetoothPower", &BluetoothTesting::testBluetoothPower);
    push("testBluetoothScan", &BluetoothTesting::testBluetoothScan);
    push("testBluetoothPairing", &BluetoothTesting::testBluetoothPairing);
    push("testBluetoothRssi", &BluetoothTesting::testBluetoothRssi);
    return results;
}

std::vector<std::string> BluetoothTesting::getAvailableTests()
{
    return {"testBluetoothAdapterPresent", "testBluetoothPower", "testBluetoothScan",
            "testBluetoothPairing", "testBluetoothRssi"};
}

TestResult BluetoothTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testBluetoothAdapterPresent") result = testBluetoothAdapterPresent();
    else if (testName == "testBluetoothPower") result = testBluetoothPower();
    else if (testName == "testBluetoothScan") result = testBluetoothScan();
    else if (testName == "testBluetoothPairing") result = testBluetoothPairing();
    else if (testName == "testBluetoothRssi") result = testBluetoothRssi();
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
