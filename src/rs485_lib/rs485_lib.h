/**
 * @file rs485_lib.h
 * @brief Header file for Rs485Testing class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef RS485TESTING_H
#define RS485TESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class Rs485Testing : public TestBase
 * @brief Class for testing the RS-485 half-duplex serial bus.
 */
class Rs485Testing : public TestBase
{
  public:
    Rs485Testing();
    ~Rs485Testing();

    TestOutput testRs485DevicePresent();
    TestOutput testRs485HalfDuplex();
    TestOutput testRs485Baud();
    TestOutput testRs485Loopback();
    TestOutput testRs485DirectionControl();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& path);
    bool halfDuplexTransfer(const uint8_t* tx, int len, uint8_t* rx, int& rxlen);
    double throughput(int baud);
    bool testDriverEnableToggle();
};

#endif // RS485TESTING_H
