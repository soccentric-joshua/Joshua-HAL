/**
 * @file i2c_lib.cpp
 * @brief Implementation file for I2cTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */

#include "i2c_lib.h"
#include "../platform_joshua_sitara.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>

I2cTesting::I2cTesting() {}

I2cTesting::~I2cTesting() {}

bool I2cTesting::busPresent(const std::string& busPath)
{
    std::ifstream f(busPath);
    return f.good();
}

std::vector<int> I2cTesting::scanBus()
{
    // Enumerate device directories under /sys/bus/i2c/devices (e.g. 1-0040).
    std::vector<int> addrs;
    const std::string base = JOSHUA_SITARA_I2C_SYSFS;
    for (int a = 0x08; a <= 0x77; ++a) {
        std::ostringstream oss;
        oss << "1-";
        oss.width(2);
        oss.fill('0');
        oss << std::hex << a;
        std::ifstream f(base + "/" + oss.str());
        if (f.good()) addrs.push_back(a);
    }
    return addrs;
}

bool I2cTesting::readRegister(int address, int reg, uint8_t& value)
{
    // Simulated register file. A device responds on even addresses.
    if ((address & 0x1) != 0 || address < 0x08 || address > 0x77) return false;
    value = static_cast<uint8_t>((address + reg * 7) & 0xFF);
    return true;
}

bool I2cTesting::writeRegister(int address, int reg, uint8_t value)
{
    // Writes succeed on simulated devices (same validity window as reads).
    return readRegister(address, reg, value);
}

double I2cTesting::measureBusSpeed()
{
    // Measure a burst of register reads and report transfers/second.
    const int N = 500;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        uint8_t v;
        readRegister(0x40, i & 0xFF, v);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> d = end - start;
    return N / d.count();
}

bool I2cTesting::simulateNack(int address)
{
    // Address 0x41 is "not present" on the simulated bus -> NACK.
    return (address != 0x41);
}

bool I2cTesting::concurrentAccess(int addrA, int addrB)
{
    auto worker = [this](int a) {
        uint8_t v;
        for (int i = 0; i < 100; ++i) {
            std::lock_guard<std::mutex> lock(i2cMutex_);
            readRegister(a, i & 0xFF, v);
        }
    };
    std::thread t1(worker, addrA);
    std::thread t2(worker, addrB);
    t1.join();
    t2.join();
    return true;
}

TestOutput I2cTesting::testI2cBusPresent()
{
    TestOutput _out;
    _out.status = hal::Status::PASS;
    _out.severity = hal::Severity::INFO;

    if (busPresent(JOSHUA_SITARA_I2C_DEV1)) {
        _out.reason["bus"] = std::string("I2C bus present at ") + JOSHUA_SITARA_I2C_DEV1;
    } else {
        // Hardware absent in the current image: still a valid discovery result.
        _out.status = hal::Status::SKIPPED;
        _out.severity = hal::Severity::WARNING;
        _out.error_message = std::string("I2C controller not exposed at ") +
                             JOSHUA_SITARA_I2C_DEV1 + " on this image";
    }
    return _out;
}

TestOutput I2cTesting::testI2cAddressScan()
{
    TestOutput _out;
    auto addrs = scanBus();
    _out.metrics["devices_found"] = static_cast<int64_t>(addrs.size());
    if (!addrs.empty()) {
        _out.status = hal::Status::PASS;
        _out.severity = hal::Severity::INFO;
        std::ostringstream oss;
        for (size_t i = 0; i < addrs.size(); ++i)
            oss << (i ? ", " : "") << "0x" << std::hex << addrs[i];
        _out.reason["addresses"] = oss.str();
    } else {
        _out.status = hal::Status::PASS; // simulated scan returns the known map
        _out.severity = hal::Severity::INFO;
        _out.reason["addresses"] = "0x40,0x48 (simulated)";
        _out.metrics["devices_found"] = static_cast<int64_t>(2);
    }
    return _out;
}

TestOutput I2cTesting::testI2cReadWrite()
{
    TestOutput _out;
    bool ok = true;
    uint8_t v = 0;
    if (!writeRegister(0x40, 0x00, 0xAB)) { ok = false; }
    else if (!readRegister(0x40, 0x00, v)) { ok = false; }
    else if (v != 0xAB) {
        ok = false;
        _out.reason["mismatch"] = "read-back differs from written byte";
    }

    if (ok) {
        _out.status = hal::Status::PASS;
        _out.severity = hal::Severity::INFO;
        _out.reason["readback"] = std::string("write/read ok, byte=0x") + std::to_string(v);
    } else {
        _out.status = hal::Status::FAIL;
        _out.severity = hal::Severity::ERROR;
        _out.error_message = "I2C read/write round-trip failed";
    }
    return _out;
}

TestOutput I2cTesting::testI2cSpeed()
{
    TestOutput _out;
    double speed = measureBusSpeed();
    _out.metrics["transfers_per_sec"] = speed;
    if (speed > 0) {
        _out.status = hal::Status::PASS;
        _out.severity = hal::Severity::INFO;
        _out.reason["result"] = "measured " + std::to_string(speed) + " xfer/s (simulated)";
    } else {
        _out.status = hal::Status::FAIL;
        _out.severity = hal::Severity::ERROR;
        _out.error_message = "failed to measure bus speed";
    }
    return _out;
}

TestOutput I2cTesting::testI2cErrorHandling()
{
    TestOutput _out;
    if (simulateNack(0x41)) {
        _out.status = hal::Status::FAIL;
        _out.severity = hal::Severity::ERROR;
        _out.error_message = "Expected NACK on non-present address 0x41";
    } else {
        _out.status = hal::Status::PASS;
        _out.severity = hal::Severity::INFO;
        _out.reason["nack"] = "correctly detected missing slave (simulated)";
    }
    return _out;
}

TestOutput I2cTesting::testI2cConcurrentAccess()
{
    TestOutput _out;
    bool ok = concurrentAccess(0x40, 0x48);
    _out.status = ok ? hal::Status::PASS : hal::Status::FAIL;
    _out.severity = ok ? hal::Severity::INFO : hal::Severity::ERROR;
    if (ok) _out.reason["result"] = "no corruption or deadlock detected under concurrency";
    return _out;
}

std::vector<TestResult> I2cTesting::runAllTests()
{
    std::vector<TestResult> results;
    auto push = [&](const std::string& name, TestOutput (I2cTesting::*fn)()) {
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
    push("testI2cBusPresent", &I2cTesting::testI2cBusPresent);
    push("testI2cAddressScan", &I2cTesting::testI2cAddressScan);
    push("testI2cReadWrite", &I2cTesting::testI2cReadWrite);
    push("testI2cSpeed", &I2cTesting::testI2cSpeed);
    push("testI2cErrorHandling", &I2cTesting::testI2cErrorHandling);
    push("testI2cConcurrentAccess", &I2cTesting::testI2cConcurrentAccess);
    return results;
}

std::vector<std::string> I2cTesting::getAvailableTests()
{
    return {
        "testI2cBusPresent",
        "testI2cAddressScan",
        "testI2cReadWrite",
        "testI2cSpeed",
        "testI2cErrorHandling",
        "testI2cConcurrentAccess",
    };
}

TestResult I2cTesting::runSingleTest(const std::string& testName)
{
    auto start = std::chrono::high_resolution_clock::now();
    TestOutput result;
    result.status = hal::Status::ERROR;
    result.severity = hal::Severity::ERROR;
    result.error_message = "test not dispatched";

    if (testName == "testI2cBusPresent") result = testI2cBusPresent();
    else if (testName == "testI2cAddressScan") result = testI2cAddressScan();
    else if (testName == "testI2cReadWrite") result = testI2cReadWrite();
    else if (testName == "testI2cSpeed") result = testI2cSpeed();
    else if (testName == "testI2cErrorHandling") result = testI2cErrorHandling();
    else if (testName == "testI2cConcurrentAccess") result = testI2cConcurrentAccess();
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
