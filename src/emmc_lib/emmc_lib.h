/**
 * @file emmc_lib.h
 * @brief Header file for EmmcTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef EMMCTESTING_H
#define EMMCTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class EmmcTesting : public TestBase
 * @brief Class for testing eMMC flash storage on the Sitara AM62x.
 */
class EmmcTesting : public TestBase
{
  public:
    EmmcTesting();
    ~EmmcTesting();

    TestOutput testEmmcPresent();
    TestOutput testEmmcCapacity();
    TestOutput testEmmcReadWrite();
    TestOutput testEmmcSpeed();
    TestOutput testEmmcWearLeveling();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& dev);
    long long capacityBytes();
    bool doReadWrite(double& throughput_mbps);
    bool wearLevelingOk(double& lifetime_pct);
};

#endif // EMMCTESTING_H
