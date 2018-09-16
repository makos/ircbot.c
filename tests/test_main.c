#include "minunit.h"
#include "test_bot.c"
#include "test_connection.c"
#include "test_connection_keepalive.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define isatty(id) _isatty(id)
#else
#include <unistd.h>
#endif

int tests_run = 0;
int tests_failed = 0;
char **messages;
char RED[] = "\033[0;31m";
char GREEN[] = "\033[0;32m";
char YELLOW[] = "\033[0;33m";
char COLOR_END[] = "\033[0m";

typedef char(*(*callback_t)());

static char *connection_tests()
{
    printf("\n%sCONNECTION TESTS%s\n", GREEN, COLOR_END);
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

static char *connection_keepalive()
{
    mu_run_test("connection_keepalive", test_connection_keepalive);
    return 0;
}

static char *bot_tests()
{
    printf("\n%sBOT TESTS%s\n", GREEN, COLOR_END);
    mu_run_test("bot_create_success", test_bot_create_success);
    mu_run_test("bot_create_failure", test_bot_create_failure);
    mu_run_test("bot_add_command_success", test_bot_add_command_success);
    mu_run_test("bot_add_command_failure", test_bot_add_command_failure);
    mu_run_test("bot_connect_success", test_bot_connect_success);
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
    callback_t to_call;

    if (!(isatty(fileno(stdout)))) {
        strcpy(RED, "");
        strcpy(GREEN, "");
        strcpy(YELLOW, "");
        strcpy(COLOR_END, "");
    }

    messages = malloc(64);
    for (int i = 0; i < 64; i++) {
        messages[i] = malloc(128);
    }

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "all") == 0) {
            to_call = run_all;
        } else if (strcmp(argv[i], "connection") == 0) {
            to_call = connection_tests;
        } else if (strcmp(argv[i], "keepalive") == 0) {
            to_call = connection_keepalive;
        } else if (strcmp(argv[i], "bot") == 0) {
            to_call = bot_tests;
        } else {
            to_call = run_all;
        }
    }

    to_call();

    if (tests_failed > 0) {
        printf("\n%s==========FAILURE==========%s\n", RED, COLOR_END);
        for (int i = 0; i < tests_failed; i++) {
            printf("%s%s%s\n", RED, messages[i], COLOR_END);
        }
    } else {
        printf("\n%s====ALL TESTS PASSED====%s\n", GREEN, COLOR_END);
    }

    printf("\nFailed: %d, passed: %d, total: %d\n", tests_failed,
           tests_run - tests_failed, tests_run);
    return 0;
}
