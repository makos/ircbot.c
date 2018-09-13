#include "client.h"
#include "server/irc_server.h"
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

IRC_Client *client_create()
{
    IRC_Client *temp = malloc(sizeof(IRC_Client));
    temp->server = NULL;

    client_clear_buffer(temp);

    return temp;
}

int client_connect(IRC_Client *client, IRC_Server *server)
{
    client->server = server;
    if (server->ai_result == NULL) {
        printf("client_connect(): addrinfo is NULL\n");
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
    printf("SENT: %s", msg);
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

int client_parse(IRC_Client *client)
{
    char *result = strstr(client->recvbuf, "PING");
    if (result == NULL) {
        return ERROR;
    }

    return client_pong(client);
}

int client_pong(IRC_Client *client)
{
    // PING sends an unique ID to be sent back with the PONG message.
    // This captures that ID and sends it back.
    char *pong = malloc(32 * sizeof(char));
    char *ping_id = strtok(client->recvbuf, ":");
    while (strcmp(ping_id, "PING ") == 0) {
        ping_id = strtok(NULL, ":");
    }
    strcpy(pong, "PONG ");
    strcat(pong, ping_id);
    strcat(pong, "\n");

    if (client_send(client, pong) == ERROR) {
        return ERROR;
    }
    return OK;
}

void client_clear_buffer(IRC_Client *client)
{
    memset(client->recvbuf, 0, MAXBUFLEN * sizeof(char));
}