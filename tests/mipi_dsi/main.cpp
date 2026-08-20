#include "test_runner.h"
#include "mipi_dsi_lib/mipi_dsi_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<MipiDsiTesting>("MipiDsiTesting", argc, argv);
}
