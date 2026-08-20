#include "test_runner.h"
#include "uart_lib/uart_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<UartTesting>("UartTesting", argc, argv);
}
