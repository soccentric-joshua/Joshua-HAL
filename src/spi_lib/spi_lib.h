/**
 * @file spi_lib.h
 * @brief Header file for SpiTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef SPITESTING_H
#define SPITESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class SpiTesting : public TestBase
 * @brief Class for testing SPI bus interfaces on the Sitara AM62x.
 */
class SpiTesting : public TestBase
{
  public:
    SpiTesting();
    ~SpiTesting();

    TestOutput testSpiBusPresent();
    TestOutput testSpiMode();
    TestOutput testSpiTransfer();
    TestOutput testSpiSpeed();
    TestOutput testSpiDuplexMode();
    TestOutput testSpiErrorHandling();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool busPresent(const std::string& path);
    bool transferByte(uint8_t tx, uint8_t& rx);
    double measureSpeed();
    bool testMode(int mode);
    bool simulateFault();
};

#endif // SPITESTING_H
