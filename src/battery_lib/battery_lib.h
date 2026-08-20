/**
 * @file battery_lib.h
 * @brief Header file for BatteryTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef BATTERYTESTING_H
#define BATTERYTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class BatteryTesting : public TestBase
 * @brief Class for testing the battery/power-management subsystem.
 */
class BatteryTesting : public TestBase
{
  public:
    BatteryTesting();
    ~BatteryTesting();

    TestOutput testBatteryPresent();
    TestOutput testBatteryVoltage();
    TestOutput testBatteryCurrent();
    TestOutput testBatteryCapacity();
    TestOutput testBatteryHealth();
    TestOutput testBatteryTemperature();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& path);
    bool readVoltage(double& mv);
    bool readCurrent(double& ma);
    bool readCapacity(double& pct);
    bool healthOk(double& health_pct);
    bool readTemp(double& temp_c);
};

#endif // BATTERYTESTING_H
