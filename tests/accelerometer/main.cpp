#include "test_runner.h"
#include "accelerometer_lib/accelerometer_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<AccelerometerTesting>("AccelerometerTesting", argc, argv);
}
