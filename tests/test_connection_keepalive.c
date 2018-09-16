#include "connection/connection.h"
#include "minunit.h"
#include <stdlib.h>
#include <string.h>

static int PINGS_RECEIVED = 0;

static void pong(IRC_Connection *connection)
{
    char *id = strtok(connection->recvbuf, ":");
    id = strtok(NULL, ":");
    char *pong_msg = malloc(32 * sizeof(char));
    strcpy(pong_msg, "PONG ");
    strcat(pong_msg, id);
    connection_send(connection, pong_msg);
}

static void parse(IRC_Connection *connection)
{
    // if (strstr(connection->recvbuf, "PING") != NULL) {
    if (strncmp(connection->recvbuf, "PING", (size_t)4) == 0) {
        PINGS_RECEIVED++;
        pong(connection);
    }
}

static char *test_connection_keepalive()
{
    IRC_Connection *temp = connection_create("irc.rizon.net", "6660");
    if (connection_connect(temp) == 0) {
        char *errmsg = "TEST: connection_connect(): error";
        return errmsg;
    }

    connection_send(temp, "USER ircbot ircbot ircbot ircbot");
    connection_send(temp, "NICK ircbot");

    int bytes = 0;
    do {
        bytes = connection_read(temp);
        parse(temp);
    } while (bytes > 0 && PINGS_RECEIVED < 4);

    connection_disconnect(temp);
    return 0;
}
