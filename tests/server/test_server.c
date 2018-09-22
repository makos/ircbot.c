#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#endif

#include "test_server.h"

#define ERROR 0
#define OK 1

Mock_Server *server_create()
{
    Mock_Server *temp = malloc(sizeof(Mock_Server));
    temp->socket_client = INVALID_SOCKET;
    temp->client = malloc(sizeof(struct sockaddr_in));
    temp->server = malloc(sizeof(struct sockaddr_in));
    /* temp->server->sin_addr->s_addr = malloc(sizeof(struct in_addr)); */

    temp->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(temp->buffer, 0, MAXBUFLEN);

    if (temp->socket_fd == SOCKET_ERROR) {
        fprintf(stderr, "server_create(): could not create socket\n");
        return NULL;
    }

    temp->server->sin_family = AF_INET;
    temp->server->sin_addr.s_addr = INADDR_ANY;
    temp->server->sin_port = htons(8080);

    if (bind(temp->socket_fd,
             (struct sockaddr *)temp->server,
             sizeof(struct sockaddr_in)) < 0) {
        fprintf(stderr,
                "server_create(): bind error %s %d\n",
                strerror(errno), errno);
        return NULL;
    }

    listen(temp->socket_fd, 3);
    fcntl(temp->socket_fd, F_SETFD, O_NONBLOCK);

    return temp;
}

int server_read(Mock_Server *server)
{
    int sockaddr_size = sizeof(struct sockaddr_in);
    char hello_msg[] = "Hello from server";
    int hello_msg_len = strlen(hello_msg);

    /* server->socket_client = */
    /*     accept4(server->socket_fd, */
    /*            (struct sockaddr *)server->client, */
    /*             (socklen_t *)&sockaddr_size, SOCK_NONBLOCK);  */

    /* if (server->socket_client == SOCKET_ERROR) { */
    /*     fprintf(stderr, */
    /*             "server_read(): socket error %s %d\n", */
    /*             strerror(errno), errno); */
    /*     return ERROR; */
    /* } */
    int bytes = 0;
    while ((server->socket_client = accept(server->socket_fd,
                                         (struct sockaddr *)server->client,
                                         (socklen_t *)&sockaddr_size)) >= 0) {
        fcntl(server->socket_client, F_SETFD, O_NONBLOCK);

        int debug_result = send(server->socket_client, hello_msg, hello_msg_len, 0);
        if (debug_result == -1) {
            fprintf(stderr, "%d %s", errno, strerror(errno));
        }

        do {
            memset(server->buffer, 0, MAXBUFLEN);
            bytes = recv(server->socket_client, server->buffer, MAXBUFLEN, 0);

            for (int i = 0; i < bytes; i++) {
                printf("%c", server->buffer[i]);
            }

            if (bytes > 0) {
                send(server->socket_client, server->buffer, bytes, 0);
            }
        } while (bytes > 0);
    }

    close(server->socket_client);

    /* printf("Accepted connection\n"); */
    /* send(server->socket_client, hello_msg, hello_msg_len, 0); */
        
    /* int bytes = 0; */
    /* do { */
    /*     memset(server->buffer, 0, MAXBUFLEN); */
    /*     bytes = recv(server->socket_client, server->buffer, MAXBUFLEN, 0); */

    /*     for (int i = 0; i < bytes; i++) { */
    /*         printf("%c", server->buffer[i]); */
    /*     } */

    /*     if (bytes > 0) { */
    /*         send(server->socket_client, server->buffer, bytes, 0); */
    /*     } */
    /* } while (bytes > 0); */

    /* if (bytes == 0) { */
    /*     printf("Connection closed.\n"); */
    /*     close(server->socket_client); */
    /* } else if (bytes < 0) { */
    /*     fprintf(stderr, "server_read(): recv error %s %d\n", strerror(errno), errno); */
    /*     return ERROR; */
    /* } */

    return OK;
}
