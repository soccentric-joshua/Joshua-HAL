#include "test_runner.h"
#include "pwm_output_lib/pwm_output_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<PwmOutputTesting>("PwmOutputTesting", argc, argv);
}
