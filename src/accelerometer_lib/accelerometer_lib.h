/**
 * @file accelerometer_lib.h
 * @brief Header file for AccelerometerTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef ACCELEROMETERTESTING_H
#define ACCELEROMETERTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class AccelerometerTesting : public TestBase
 * @brief Class for testing the accelerometer sensor on the IMU/I2C bus.
 */
class AccelerometerTesting : public TestBase
{
  public:
    AccelerometerTesting();
    ~AccelerometerTesting();

    TestOutput testAccelerometerPresent();
    TestOutput testAccelerometerReadAxes();
    TestOutput testAccelerometerRange();
    TestOutput testAccelerometerScale();
    TestOutput testAccelerometerSensitivity();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& path);
    bool readAxes(double& x, double& y, double& z);
    bool checkRange();
    bool checkScale();
    double measureNoiseFloor();
};

#endif // ACCELEROMETERTESTING_H
