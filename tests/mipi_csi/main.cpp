#include "test_runner.h"
#include "mipi_csi_lib/mipi_csi_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<MipiCsiTesting>("MipiCsiTesting", argc, argv);
}
