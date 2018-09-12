#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "irc_server.h"

IRC_Server *server_create(const char address[], const char port[])
{
    IRC_Server *temp = malloc(sizeof(IRC_Server));
    temp->address = address;
    temp->port = port;
    temp->ai_result = NULL;
    // Apparently, calloc() here DOESN'T fill the memory with 0's.
    // temp->ai_hints = calloc((size_t)1, sizeof(temp->ai_hints));
    temp->ai_hints = malloc(sizeof(struct addrinfo));
    memset(temp->ai_hints, 0, sizeof(struct addrinfo));

    temp->ai_hints->ai_family = AF_UNSPEC;
    temp->ai_hints->ai_socktype = SOCK_STREAM;
    temp->ai_hints->ai_protocol = IPPROTO_TCP;

    int result = 0;
    result = getaddrinfo(temp->address, temp->port, temp->ai_hints,
                         &temp->ai_result);
    if (result != 0) {
        printf("getaddrinfo() error: %d\n", result);
        WSACleanup();
        return NULL;
    }

    return temp;
}
