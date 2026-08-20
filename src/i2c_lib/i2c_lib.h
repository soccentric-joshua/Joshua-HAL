/**
 * @file i2c_lib.h
 * @brief Header file for I2cTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef I2CTESTING_H
#define I2CTESTING_H

#include "lib_base.h"

#include <mutex>
#include <string>
#include <vector>

/**
 * @class I2cTesting : public TestBase
 * @brief Class for testing the I2C bus interface on the Sitara AM62x.
 * @details Exercises bus enumeration, device scanning, read/write transfers,
 * fast-mode speed, error handling and concurrent access.
 */
class I2cTesting : public TestBase
{
  public:
    I2cTesting();
    ~I2cTesting();

    TestOutput testI2cBusPresent();
    TestOutput testI2cAddressScan();
    TestOutput testI2cReadWrite();
    TestOutput testI2cSpeed();
    TestOutput testI2cErrorHandling();
    TestOutput testI2cConcurrentAccess();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    // Simulated I2C device register map (address -> register -> byte).
    std::mutex i2cMutex_;

    bool busPresent(const std::string& busPath);
    std::vector<int> scanBus();
    bool readRegister(int address, int reg, uint8_t& value);
    bool writeRegister(int address, int reg, uint8_t value);
    double measureBusSpeed();
    bool simulateNack(int address);
    bool concurrentAccess(int addrA, int addrB);
};

#endif // I2CTESTING_H
