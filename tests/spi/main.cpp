#include "test_runner.h"
#include "spi_lib/spi_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<SpiTesting>("SpiTesting", argc, argv);
}
