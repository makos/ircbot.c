#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "client.h"
#include "server/irc_server.h"

IRC_Client *client_create()
{
    // int result = 0;
    IRC_Client *temp = malloc(sizeof(IRC_Client));

    client_zero_buffers(temp);

    return temp;
}

int client_connect(IRC_Client *client, IRC_Server *server)
{
    if (server->ai_result == NULL) {
        printf("client_connect(): addrinfo * is NULL\n");
        return 0;
    }
    client->connect_socket =
        socket(server->ai_result->ai_family, server->ai_result->ai_socktype,
               server->ai_result->ai_protocol);
    if (client->connect_socket == INVALID_SOCKET) {
        printf("client_connect(): INVALID_SOCKET\n");
        return 0;
    }

    int result = connect(client->connect_socket, server->ai_result->ai_addr,
                         server->ai_result->ai_addrlen);
    if (result == SOCKET_ERROR) {
        closesocket(client->connect_socket);
        printf("client_connect(): error %d\n", result);
        return 0;
    }

    freeaddrinfo(server->ai_result);
    return 1;
}

int client_send(IRC_Client *client, const char msg[])
{
    int result = send(client->connect_socket, msg, (int)strlen(msg), 0);
    if (result == SOCKET_ERROR) {
        printf("client_send(): error %d\n", result);
        closesocket(client->connect_socket);
        return 0;
    }
    return 1;
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

void client_zero_buffers(IRC_Client *client)
{
    for (int i = 0; i < MAXBUFLEN; i++) {
        // client->sendbuf[i] = 0;
        client->recvbuf[i] = 0;
    }
}