#include "test_runner.h"
#include "sd_card_lib/sd_card_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<SdCardTesting>("SdCardTesting", argc, argv);
}
