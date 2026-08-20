/**
 * @file mipi_dsi_lib.h
 * @brief Header file for MipiDsiTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef MIPIDSITESTING_H
#define MIPIDSITESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class MipiDsiTesting : public TestBase
 * @brief Class for testing the MIPI DSI display interface.
 */
class MipiDsiTesting : public TestBase
{
  public:
    MipiDsiTesting();
    ~MipiDsiTesting();

    TestOutput testMipiDsiPresent();
    TestOutput testMipiDsiPanel();
    TestOutput testMipiDsiResolution();
    TestOutput testMipiDsiBacklight();
    TestOutput testMipiDsiCommandMode();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& path);
    bool panelConnected(std::string& name);
    bool readResolution(int& w, int& h);
    bool setBacklight(int percent);
    bool testCommandTransfer();
};

#endif // MIPIDSITESTING_H
