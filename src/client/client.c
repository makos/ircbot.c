#include "client.h"
#include "server/irc_server.h"
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

static void client_zero_buffers(IRC_Client *client)
{
    for (int i = 0; i < MAXBUFLEN; i++) {
        client->recvbuf[i] = 0;
    }
}

IRC_Client *client_create()
{
    IRC_Client *temp = malloc(sizeof(IRC_Client));

    client_zero_buffers(temp);

    return temp;
}

int client_connect(IRC_Client *client, IRC_Server *server)
{
    if (server->ai_result == NULL) {
        printf("client_connect(): addrinfo * is NULL\n");
        return ERROR;
    }
    client->connect_socket =
        socket(server->ai_result->ai_family, server->ai_result->ai_socktype,
               server->ai_result->ai_protocol);
    if (client->connect_socket == INVALID_SOCKET) {
        printf("client_connect(): INVALID_SOCKET\n");
        return ERROR;
    }

    int result = connect(client->connect_socket, server->ai_result->ai_addr,
                         server->ai_result->ai_addrlen);
    if (result == SOCKET_ERROR) {
        closesocket(client->connect_socket);
        printf("client_connect(): error %d\n", result);
        return ERROR;
    }

    freeaddrinfo(server->ai_result);
    return OK;
}

int client_send(IRC_Client *client, const char msg[])
{
    int result = send(client->connect_socket, msg, (int)strlen(msg), 0);
    if (result == SOCKET_ERROR) {
        printf("client_send(): error %d\n", result);
        closesocket(client->connect_socket);
        return ERROR;
    }
    return OK;
}

int client_receive(IRC_Client *client)
{
    // result here can be error code or bytes received.
    int result = recv(client->connect_socket, client->recvbuf, MAXBUFLEN, 0);
    if (result > 0) {
        return result;
    } else if (result == 0) {
        printf("Connection closed\n");
    } else {
        printf("client_receive(): error %d\n", result);
    }
    return result;
}

void client_disconnect(IRC_Client *client)
{
    closesocket(client->connect_socket);
}
