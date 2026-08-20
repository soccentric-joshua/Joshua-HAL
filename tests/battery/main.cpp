#include "test_runner.h"
#include "battery_lib/battery_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<BatteryTesting>("BatteryTesting", argc, argv);
}
