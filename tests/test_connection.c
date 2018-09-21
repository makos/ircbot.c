#include "connection/connection.h"
#include "minunit.h"
#include <stdio.h>

static char *test_connection_create_success()
{
    IRC_Connection *test_con = connection_create("127.0.0.1", "8080");
    mu_assert("test_connection_create_success(): IRC_Connection is NULL",
              test_con);
    return 0;
}

static char *test_connection_create_failure()
{
    IRC_Connection *test_con =
        connection_create("127.0.0.1", "this is over 8 chars");
    mu_assert("test_connection_create_failure(): IRC_Connection is NOT NULL",
              !test_con);
    return 0;
}

static char *test_connection_connect_success()
{
    IRC_Connection *test_con = connection_create("127.0.0.1", "8080");
    int result = connection_connect(test_con);
    connection_disconnect(test_con);
    mu_assert("test_connection_connect_success(): failure", result == 1);
    return 0;
}

static char *test_connection_connect_failure()
{
    IRC_Connection *test_con = connection_create("zxcyasdwe.com.de.au", "80");
    int result = connection_connect(test_con);
    mu_assert("test_connection_connect_failure(): failure", result == 0);
    return 0;
}

static char *test_connection_disconnect_success()
{
    IRC_Connection *test_con = connection_create("127.0.0.1", "8080");
    connection_connect(test_con);
    int result = connection_disconnect(test_con);
    mu_assert("test_connection_disconnect_success(): failure", result != -1);
    return 0;
}

static char *test_connection_send_success()
{
    IRC_Connection *test_con = connection_create("127.0.0.1", "8080");
    connection_connect(test_con);
    int result = connection_send(test_con, "USER ircbot ircbot ircbot ircbot");
    connection_disconnect(test_con);
    mu_assert("test_connection_send_success(): failure", result == 1);
    return 0;
}

static char *test_connection_send_failure()
{
    IRC_Connection *test_con = connection_create("127.0.0.1", "8080");
    connection_connect(test_con);
    connection_disconnect(test_con);
    int result = connection_send(test_con, "USER ircbot ircbot ircbot ircbot");
    mu_assert("test_connection_send_failure(): failure", result != 1);
    return 0;
}

static char *test_connection_recv_success()
{
    IRC_Connection *test_con = connection_create("127.0.0.1", "8080");
    connection_connect(test_con);
    int result = connection_read(test_con);
    connection_disconnect(test_con);
    mu_assert("test_connection_recv_success(): failure", result >= 0);
    return 0;
}

static char *test_connection_recv_failure()
{
    IRC_Connection *test_con = connection_create("127.0.0.1", "8080");
    connection_connect(test_con);
    connection_disconnect(test_con);
    int result = connection_read(test_con);
    mu_assert("test_connection_recv_success(): failure", result < 0);
    return 0;
}
