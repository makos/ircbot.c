#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "minunit.h"

static void debug_test_setup()
{
    debug_set_out("");
    debug_disable();
}

static char *test_debug_set_out_success()
{
    debug_test_setup();

    debug_set_out("stdout");
    mu_assert("test_debug_set_out(): FD_OUT is NULL",
              strcmp(debug_get_fd_str(), "stdout") == 0);

    return 0;
}

static char *test_debug_set_out_failure()
{
    debug_test_setup();

    mu_assert("test_debug_set_out_failure(): FD_OUT is not NULL",
              strcmp(debug_get_fd_str(), "stdout") != 0);

    return 0;
}

static char *test_debug_enable()
{
    debug_test_setup();

    debug_set_out("stdout");
    debug_enable();
    mu_assert("test_debug_enable(): deubg output is disabled",
              debug_is_enabled());

    return 0;
}

static char *test_debug_disable()
{
    debug_test_setup();

    debug_set_out("stdout");
    debug_enable();
    debug_disable();
    mu_assert("test_debug_disable(): debug output is enabled",
              !debug_is_enabled());

    return 0;
}
