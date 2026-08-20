/**
 * @file pwm_output_lib.h
 * @brief Header file for PwmOutputTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef PWMOUTPUTTESTING_H
#define PWMOUTPUTTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class PwmOutputTesting : public TestBase
 * @brief Class for testing PWM output channels on the Sitara AM62x.
 */
class PwmOutputTesting : public TestBase
{
  public:
    PwmOutputTesting();
    ~PwmOutputTesting();

    TestOutput testPwmChipPresent();
    TestOutput testPwmExportChannel();
    TestOutput testPwmFrequency();
    TestOutput testPwmDutyCycle();
    TestOutput testPwmPolarity();
    TestOutput testPwmRange();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool chipPresent(const std::string& path);
    bool exportChannel(int ch);
    bool setFrequency(double hz);
    bool setDutyCycle(double percent);
    bool setPolarity(bool inverse);
    double measureDuty();
};

#endif // PWMOUTPUTTESTING_H
