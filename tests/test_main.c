#include "minunit.h"
#include "test_connection.c"
#include <stdio.h>

int tests_run = 0;

static char *run_all()
{
    mu_run_test(test_connection_create_success);

    mu_run_test(test_connection_create_failure);

    mu_run_test(test_connection_connect_success);

    mu_run_test(test_connection_connect_failure);

    mu_run_test(test_connection_disconnect_success);

    mu_run_test(test_connection_send_success);

    mu_run_test(test_connection_send_failure);

    mu_run_test(test_connection_recv_success);

    mu_run_test(test_connection_recv_failure);
    return 0;
}

int main()
{
    char *result = run_all();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }

    printf("Test ran: %d\n", tests_run);
    return 0;
}
