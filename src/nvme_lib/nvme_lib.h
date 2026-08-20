/**
 * @file nvme_lib.h
 * @brief Header file for NvmeTesting class
 * @author Joshua HAL
 * @copyright (C) Joshua Sitara Platform
 */
#ifndef NVME_TESTING_H
#define NVME_TESTING_H

#include "lib_base.h"

#include <string>
#include <vector>

/**
 * @class NvmeTesting : public TestBase
 * @brief Class for testing NVMe SSD storage via the PCIe root port.
 */
class NvmeTesting : public TestBase
{
  public:
    NvmeTesting();
    ~NvmeTesting();

    TestOutput testNvmePresent();
    TestOutput testNvmeCapacity();
    TestOutput testNvmeReadWrite();
    TestOutput testNvmePerformance();
    TestOutput testNvmeSmart();

    std::vector<std::string> getAvailableTests() override;
    TestResult runSingleTest(const std::string& testName) override;

  protected:
    std::vector<TestResult> runAllTests() override;

  private:
    bool present(const std::string& dev);
    long long capacityBytes();
    bool doReadWrite(double& throughput_mbps);
    bool smartOk(double& temp_c, double& wear_pct);
};

#endif // __NVME_TESTING_H
