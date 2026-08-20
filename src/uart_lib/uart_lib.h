/**
 * @file uart_lib.h
 * @brief Header file for UartTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef UARTTESTING_H
#define UARTTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class UartTesting : public TestBase
 * @brief Class for testing UART/serial interfaces on the Sitara AM62x.
 */
class UartTesting : public TestBase
{
  public:
    UartTesting();
    ~UartTesting();

    TestOutput testUartDevicePresent();
    TestOutput testUartBaudRate();
    TestOutput testUartLoopback();
    TestOutput testUartDataIntegrity();
    TestOutput testUartFlowControl();
    TestOutput testUartErrorHandling();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool devicePresent(const std::string& path);
    bool loopbackByte(uint8_t tx, uint8_t& rx);
    double measureThroughput(int baud);
    bool testParity(uint8_t byte, bool even);
    bool testFlowControl();
};

#endif // UARTTESTING_H
