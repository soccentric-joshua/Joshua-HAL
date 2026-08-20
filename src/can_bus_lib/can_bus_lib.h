/**
 * @file can_bus_lib.h
 * @brief Header file for CanBusTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef CANBUSTESTING_H
#define CANBUSTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class CanBusTesting : public TestBase
 * @brief Class for testing CAN bus interfaces on the Sitara AM62x + PRU-ICSS.
 */
class CanBusTesting : public TestBase
{
  public:
    CanBusTesting();
    ~CanBusTesting();

    TestOutput testCanInterfacePresent();
    TestOutput testCanBitrate();
    TestOutput testCanFrameTransmit();
    TestOutput testCanFrameReceive();
    TestOutput testCanErrorFrames();
    TestOutput testCanBusOffRecovery();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool interfaceUp(const std::string& iface);
    bool transmitFrame(uint32_t id, const uint8_t* data, int len);
    bool receiveFrame(uint32_t& id, uint8_t* data, int& len);
    bool injectError();
    bool busOffRecovery();
};

#endif // CANBUSTESTING_H
