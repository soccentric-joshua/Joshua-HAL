/**
 * @file mipi_csi_lib.h
 * @brief Header file for MipiCsiTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef MIPICSITESTING_H
#define MIPICSITESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class MipiCsiTesting : public TestBase
 * @brief Class for testing the MIPI CSI-2 camera interface.
 */
class MipiCsiTesting : public TestBase
{
  public:
    MipiCsiTesting();
    ~MipiCsiTesting();

    TestOutput testMipiCsiPresent();
    TestOutput testMipiCsiLaneCount();
    TestOutput testMipiCsiPixelFormat();
    TestOutput testMipiCsiFrameRate();
    TestOutput testMipiCsiDataIntegrity();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& path);
    bool readLaneCount(int& lanes);
    bool enumerateFormat(std::string& fmt);
    double measureFrameRate();
    bool checkDataIntegrity(int& errors);
};

#endif // MIPICSITESTING_H
