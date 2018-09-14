#include "minunit.h"
#include "test_bot.c"
#include "test_connection.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int tests_run = 0;
int tests_failed = 0;
char **messages;

static char *connection_tests()
{
    mu_run_test("connection_create_success", test_connection_create_success);

    mu_run_test("connection_create_failure", test_connection_create_failure);

    mu_run_test("connection_connect_success", test_connection_connect_success);

    mu_run_test("connection_connect_failure", test_connection_connect_failure);

    mu_run_test("connection_disconnect_success",
                test_connection_disconnect_success);

    mu_run_test("connection_send_success", test_connection_send_success);

    mu_run_test("connection_send_failure", test_connection_send_failure);

    mu_run_test("connection_recv_success", test_connection_recv_success);

    mu_run_test("connection_recv_failure", test_connection_recv_failure);

    return 0;
}

static char *bot_tests()
{
    mu_run_test("bot_create_success", test_bot_create_success);
    return 0;
}

static char *run_all()
{
    char *result = NULL;

    result = bot_tests();
    if (result != 0) {
        return result;
    }

    result = connection_tests();
    if (result != 0) {
        return result;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char(*(*to_call)());

    messages = malloc(64);
    for (int i = 0; i < 64; i++) {
        messages[i] = malloc(128);
    }

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "all") == 0) {
            to_call = run_all;
        } else if (strcmp(argv[i], "connection") == 0) {
            to_call = connection_tests;
        } else if (strcmp(argv[i], "bot") == 0) {
            to_call = bot_tests;
        } else {
            to_call = run_all;
        }
    }

    to_call();

    if (tests_failed > 0) {
        printf("\n" RED "==========FAILURE==========\n" COLOR_END);
        for (int i = 0; i < tests_failed; i++) {
            printf(RED "%s\n" COLOR_END, messages[i]);
        }
    } else {
        printf("\n" GREEN "====ALL TESTS PASSED====\n" COLOR_END);
    }

    printf("\nFailed: %d, passed: %d, total: %d\n", tests_failed,
           tests_run - tests_failed, tests_run);
    return 0;
}
