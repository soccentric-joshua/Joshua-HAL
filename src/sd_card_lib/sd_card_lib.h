/**
 * @file sd_card_lib.h
 * @brief Header file for SdCardTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef SDCARDTESTING_H
#define SDCARDTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class SdCardTesting : public TestBase
 * @brief Class for testing the SD card storage interface on the Sitara AM62x.
 */
class SdCardTesting : public TestBase
{
  public:
    SdCardTesting();
    ~SdCardTesting();

    TestOutput testSdCardPresent();
    TestOutput testSdCardCapacity();
    TestOutput testSdCardReadWrite();
    TestOutput testSdCardSpeed();
    TestOutput testSdCardHealth();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& dev);
    long long capacityBytes();
    bool doReadWrite(const std::string& dev, double& throughput_mbps);
    bool healthOk();
};

#endif // SDCARDTESTING_H
