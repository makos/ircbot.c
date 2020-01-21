#include "debug.h"

int main(int argc, char *argv[])
{
    debug_enable();
    debug_set_out("stderr");
    // This will be logged.
    debug_log("Hello there!");

    int i = 99;
    debug_disable();
    // This won't be logged.
    debug_log("Value of i is: %i", i);
    debug_enable();

    return 0;
}
