#include "test_runner.h"
#include "emmc_lib/emmc_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<EmmcTesting>("EmmcTesting", argc, argv);
}
