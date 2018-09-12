#include "client.h"
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

IRC_Connection* client_create(char addr[], char port[], int recvbuflen,
                              int sendbuflen) {
    IRC_Connection* temp = malloc(sizeof(IRC_Connection));

    temp->i_result = 0;

    strcpy(temp->ipv4_addr, addr);
    strcpy(temp->port, port);

    temp->recvbuflen = recvbuflen;
    temp->sendbuflen = sendbuflen;

    temp->recvbuf = calloc(recvbuflen, sizeof(char));
    temp->sendbuf = calloc(sendbuflen, sizeof(char));

    temp->i_result = WSAStartup(MAKEWORD(2, 2), temp->wsaData);
    if (temp->i_result != 0) {
        return NULL;
    }

    return temp;
}
