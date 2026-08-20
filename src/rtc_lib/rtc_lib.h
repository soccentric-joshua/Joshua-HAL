/**
 * @file rtc_lib.h
 * @brief Header file for RtcTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef RTCTESTING_H
#define RTCTESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class RtcTesting : public TestBase
 * @brief Class for testing the Real-Time Clock on the Sitara AM62x.
 */
class RtcTesting : public TestBase
{
  public:
    RtcTesting();
    ~RtcTesting();

    TestOutput testRtcPresent();
    TestOutput testRtcReadTime();
    TestOutput testRtcSetTime();
    TestOutput testRtcPersistence();
    TestOutput testRtcAlarm();
    TestOutput testRtcAccuracy();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& path);
    bool readUnixTime(int64_t& t);
    bool setUnixTime(int64_t t);
    bool testAlarm();
    double driftPpm();
};

#endif // RTCTESTING_H
