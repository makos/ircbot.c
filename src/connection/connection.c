#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#define ERROR 0
#define OK 1

#include "connection.h"

static int connection_init_socket()
{
#ifdef _WIN32
    WSADATA wsa_data;
    int result = 0;

    result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        printf("WSAStartup error: %d\n", result);
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
    status = shutdown(connection->socket, SHUT_RDWR);
    if (status == 0) {
        status = close(connection->socket);
    }
#endif
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
        printf("connection_create(): address or port too long\n");
        return NULL;
    }

    if (connection_init_socket() == 0) {
        return NULL;
    }

    IRC_Connection *temp = malloc(sizeof(IRC_Connection));

    connection_setup_strings(temp, address, port);

    int result = connection_setup_addrinfo(temp);
    if (result != 0) {
        printf("getaddrinfo() error: %d\n", result);
        connection_cleanup();
        return NULL;
    }

    return temp;
}

int connection_connect(IRC_Connection *connection)
{
    if (connection == NULL) {
        printf("connection_connect(): connection is NULL\n");
        return ERROR;
    }

    if (connection->ai_result == NULL) {
        printf("connection_connect(): addrinfo is NULL\n");
        return ERROR;
    }

    connection->socket = socket(connection->ai_result->ai_family,
                                connection->ai_result->ai_socktype,
                                connection->ai_result->ai_protocol);
    if (connection->socket == INVALID_SOCKET) {
        printf("connection_connect(): invalid socket\n");
        return ERROR;
    }

    int result = connect(connection->socket, connection->ai_result->ai_addr,
                         connection->ai_result->ai_addrlen);
    if (result == SOCKET_ERROR) {
        connection_close_socket(connection);
        printf("connection_connect(): error %d\n", result);
        return ERROR;
    }

    freeaddrinfo(connection->ai_result);
    return OK;
}

int connection_disconnect(IRC_Connection *connection)
{
    if (connection == NULL) {
        printf("connection_disconnect(): connection is NULL\n");
        return ERROR;
    }

    return connection_close_socket(connection);
}

int connection_send(IRC_Connection *connection, const char msg[])
{
    if (connection == NULL) {
        printf("connection_send(): connection is NULL\n");
        return ERROR;
    }

    int result = send(connection->socket, msg, (int)strlen(msg), 0);
    if (result == SOCKET_ERROR) {
        printf("connection_send(): error %d\n", result);
        connection_close_socket(connection);
        return ERROR;
    }

    printf("SENT: %s", msg);

    return OK;
}

int connection_read(IRC_Connection *connection)
{
    if (connection == NULL) {
        printf("connection_read(): connection is NULL\n");
        return ERROR;
    }

    int result = recv(connection->socket, connection->recvbuf,
                      connection->recvbuflen, 0);
    if (result > 0) {
        // Bytes received.
        return result;
    } else if (result == 0) {
        printf("Connection closed\n");
        connection_close_socket(connection);
    } else {
        printf("connection_receive(): error %d\n", result);
    }
    return result;
}
