#include "irc_server.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>

IRC_Server *server_create(const char address[], const char port[])
{
    IRC_Server *temp = malloc(sizeof(IRC_Server));
    temp->address = address;
    temp->port = port;
    temp->ai_result = NULL;
    temp->ai_hints = calloc((size_t)1, sizeof(struct addrinfo));

    temp->ai_hints->ai_family = AF_UNSPEC;
    temp->ai_hints->ai_socktype = SOCK_STREAM;
    temp->ai_hints->ai_protocol = IPPROTO_TCP;

    int result = 1;
    result = getaddrinfo(temp->address, temp->port, temp->ai_hints,
                         &temp->ai_result);
    if (result != 0) {
        printf("getaddrinfo() error: %d\n", result);
        WSACleanup();
        return NULL;
    }

    return temp;
}
