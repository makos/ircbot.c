/**
 * @file connection.c
 * @author Mateusz Makowski
 * @date 17.09.2018
 * @brief Implementation of connection.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <errno.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#define ERROR 0
#define OK 1

#include "connection.h"

static void connection_print_error()
{
#ifdef __linux__
    fprintf(stderr, "connection_receive(): error %s %d\n", strerror(errno),
            errno);
#else
    fprintf(stderr, "connection_read(): error %d\n", WSAGetLastError());
#endif
}

static int connection_init_socket()
{
#ifdef _WIN32
    WSADATA wsa_data;
    int result = 0;

    result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        fprintf(stderr, "WSAStartup error: %d\n", result);
        return ERROR;
    }
    return OK;
#else
    return OK;
#endif
}

static void connection_cleanup()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

static int connection_close_socket(IRC_Connection *connection)
{
    int status = 0;
#ifdef _WIN32
    status = shutdown(connection->socket, SD_BOTH);
    if (status == 0) {
        status = closesocket(connection->socket);
    }
    connection_cleanup();
#else
    if (connection->socket > 0) {
        status = shutdown(connection->socket, SHUT_RDWR);
    } else {
        status = -1;
    }
    if (status == 0) {
        status = close(connection->socket);
    }
#endif

    /*if (connection->ai_hints->ai_addr != NULL)
        free(connection->ai_hints->ai_addr);
    if (connection->ai_hints->ai_canonname != NULL)
        free(connection->ai_hints->ai_canonname);
    if (connection->ai_hints->ai_next != NULL)
        free(connection->ai_hints->ai_next);
    if (connection->ai_hints != NULL)
        free(connection->ai_hints);
    if (connection->ai_result != NULL)
        free(connection->ai_result);
    if (connection->recvbuf != NULL)
        free(connection->recvbuf);
    if (connection != NULL)
        free(connection);*/

    return status;
}

static void connection_clear_buffer(IRC_Connection *connection)
{
    memset(connection->recvbuf, 0, connection->recvbuflen * sizeof(char));
}

static void connection_setup_strings(IRC_Connection *connection,
                                     const char *address, const char *port)
{
    connection->recvbuflen = 512;

    for (int i = 0; i < MAX_ADDR_LEN; i++) {
        connection->address[i] = '\0';
    }
    for (int i = 0; i < MAX_PORT_LEN; i++) {
        connection->port[i] = '\0';
    }

    strcpy(connection->address, address);
    strcpy(connection->port, port);
    connection->recvbuf = calloc((size_t)connection->recvbuflen, sizeof(char));
}

static int connection_setup_addrinfo(IRC_Connection *connection)
{
    connection->ai_result = NULL;
    connection->ai_hints = calloc((size_t)1, sizeof(struct addrinfo));
    connection->ai_hints->ai_family = AF_UNSPEC;
    connection->ai_hints->ai_socktype = SOCK_STREAM;
    connection->ai_hints->ai_protocol = IPPROTO_TCP;

    return getaddrinfo(connection->address, connection->port,
                       connection->ai_hints, &connection->ai_result);
}

IRC_Connection *connection_create(const char address[], const char port[])
{
    if (strlen(address) > (size_t)32 || strlen(port) > (size_t)8) {
        fprintf(stderr, "connection_create(): address or port too long\n");
        return NULL;
    }

    if (connection_init_socket() == 0) {
        return NULL;
    }

    IRC_Connection *temp = malloc(sizeof(IRC_Connection));

    connection_setup_strings(temp, address, port);

    int result = connection_setup_addrinfo(temp);
    if (result != 0) {
        fprintf(stderr, "getaddrinfo() error: %d\n", result);
        connection_cleanup();
        // TODO: free memory here
        return NULL;
    }

    return temp;
}

int connection_connect(IRC_Connection *connection)
{
    if (connection == NULL) {
        fprintf(stderr, "connection_connect(): connection is NULL\n");
        return ERROR;
    }

    if (connection->ai_result == NULL) {
        fprintf(stderr, "connection_connect(): addrinfo is NULL\n");
        return ERROR;
    }

    connection->socket = socket(connection->ai_result->ai_family,
                                connection->ai_result->ai_socktype,
                                connection->ai_result->ai_protocol);
    if (connection->socket == INVALID_SOCKET) {
        fprintf(stderr, "connection_connect(): invalid socket\n");
        return ERROR;
    }

    int result = connect(connection->socket, connection->ai_result->ai_addr,
                         connection->ai_result->ai_addrlen);
    if (result == SOCKET_ERROR) {
        connection_close_socket(connection);
        fprintf(stderr, "connection_connect(): error %d\n", result);
        return ERROR;
    }

    freeaddrinfo(connection->ai_result);
    return OK;
}

int connection_disconnect(IRC_Connection *connection)
{
    if (connection == NULL) {
        fprintf(stderr, "connection_disconnect(): connection is NULL\n");
        return ERROR;
    }

    return connection_close_socket(connection);
}

int connection_send(IRC_Connection *connection, const char msg[])
{
    if (connection == NULL) {
        fprintf(stderr, "connection_send(): connection is NULL\n");
        return ERROR;
    }

    char *temp_msg = malloc(MAX_MSG_LEN * sizeof(char));
    strcpy(temp_msg, msg);
    strcat(temp_msg, "\r\n");

    int result = send(connection->socket, temp_msg, (int)strlen(temp_msg), 0);
    if (result == SOCKET_ERROR) {
        fprintf(stderr, "connection_send(): error %d\n", result);
        connection_close_socket(connection);

        free(temp_msg);
        return ERROR;
    }

    printf("SENT: %s", temp_msg);
    free(temp_msg);

    return OK;
}

int connection_read(IRC_Connection *connection)
{
    if (connection == NULL) {
        fprintf(stderr, "connection_read(): connection is NULL\n");
        return ERROR;
    }

    connection_clear_buffer(connection);

    int result = recv(connection->socket, connection->recvbuf,
                      connection->recvbuflen, 0);
    if (result > 0) {
        // Bytes received.
        return result;
    } else if (result == 0) {
        fprintf(stderr, "Connection closed\n");
        connection_close_socket(connection);
    } else {
    }
    return result;
}
