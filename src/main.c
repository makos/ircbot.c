#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "client.h"

#pragma comment(lib, "Ws2_32.lib")
#define IRCPORT "6660"
#define BUFLEN 512

int main() {
    WSADATA wsaData;
    int i_result;

    i_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (i_result != 0) {
        printf("WSAStartup failed: %d\n", i_result);
        return 1;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    i_result = getaddrinfo("irc.rizon.net", IRCPORT, &hints, &result);
    if (i_result != 0) {
        printf("getaddrinfo failed: %d\n", i_result);
        WSACleanup();
        return 1;
    }

    SOCKET connect_socket = INVALID_SOCKET;
    ptr = result;
    connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (connect_socket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    i_result = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (i_result == SOCKET_ERROR) {
        closesocket(connect_socket);
        connect_socket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (connect_socket == INVALID_SOCKET) {
        printf("Unabel to connect to server!\n");
        WSACleanup();
        return 1;
    }

    int recvbuflen = BUFLEN;
    char *sendbuf = "USER circbot circbothost circbotserv C_IRC_BOT";
    char recvbuf[BUFLEN + 1];

    i_result = send(connect_socket, sendbuf, (int)strlen(sendbuf), 0);
    if (i_result == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return 1;
    }

    printf("Bytes sent: %ld\n", i_result);

    i_result = (shutdown(connect_socket, SD_SEND));
    if (i_result == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return 1;
    }

    do {
        i_result = recv(connect_socket, recvbuf, recvbuflen, 0);
        if (i_result > 0) {
            printf("Bytes received: %d\n", i_result);
            for (int i = 0; i < i_result; i++) {
                printf("%c", recvbuf[i]);
            }
        } else if (i_result == 0) {
            printf("Connection closed\n");
        } else {
            printf("recv failed: %d\n", WSAGetLastError());
        }
    } while (i_result > 0);

    closesocket(connect_socket);
    WSACleanup();

    printf("\n");
    return 0;
}