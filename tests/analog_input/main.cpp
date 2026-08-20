#include "test_runner.h"
#include "analog_input_lib/analog_input_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<AnalogInputTesting>("AnalogInputTesting", argc, argv);
}
