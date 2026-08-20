/**
 * @file analog_input_lib.h
 * @brief Header file for AnalogInputTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef ANALOGINPUTTESTING_H
#define ANALOGINPUTTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class AnalogInputTesting : public TestBase
 * @brief Class for testing the analog input (ADC) subsystem on the Sitara AM62x.
 */
class AnalogInputTesting : public TestBase
{
  public:
    AnalogInputTesting();
    ~AnalogInputTesting();

    TestOutput testAdcChannelPresent();
    TestOutput testAdcReadRaw();
    TestOutput testAdcLinearity();
    TestOutput testAdcResolution();
    TestOutput testAdcNoise();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool channelPresent(const std::string& path);
    bool readRaw(int& raw);
    double rawToVoltage(int raw);
    double linearityError();
};

#endif // ANALOGINPUTTESTING_H
