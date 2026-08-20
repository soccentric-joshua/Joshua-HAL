#include "test_runner.h"
#include "nvme_lib/nvme_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<NvmeTesting>("NvmeTesting", argc, argv);
}
