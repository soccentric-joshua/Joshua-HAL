#include "test_runner.h"
#include "rtc_lib/rtc_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<RtcTesting>("RtcTesting", argc, argv);
}
