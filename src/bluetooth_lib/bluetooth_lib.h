/**
 * @file bluetooth_lib.h
 * @brief Header file for BluetoothTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef BLUETOOTHTESTING_H
#define BLUETOOTHTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class BluetoothTesting : public TestBase
 * @brief Class for testing the Bluetooth radio via BlueZ.
 */
class BluetoothTesting : public TestBase
{
  public:
    BluetoothTesting();
    ~BluetoothTesting();

    TestOutput testBluetoothAdapterPresent();
    TestOutput testBluetoothPower();
    TestOutput testBluetoothScan();
    TestOutput testBluetoothPairing();
    TestOutput testBluetoothRssi();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool adapterPresent(const std::string& hci);
    bool setPower(bool on);
    bool startScan(int& found);
    bool pair(const std::string& addr);
    bool readRssi(double& dbm);
};

#endif // BLUETOOTHTESTING_H
