#include "test_server.h"
#include <signal.h>
#include <stdio.h>

int main()
{
    signal(SIGPIPE, SIG_IGN);
    Mock_Server *server = server_create();
    if (!server) {
        fprintf(stderr, "server is NULL\n");
        return 1;
    }

    while (1) {
        server_read(server);
    }

    return 0;
}
