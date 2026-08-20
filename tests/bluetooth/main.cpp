#include "test_runner.h"
#include "bluetooth_lib/bluetooth_lib.h"

int main(int argc, char* argv[])
{
    return runTestApp<BluetoothTesting>("BluetoothTesting", argc, argv);
}
