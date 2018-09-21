/**
 * @file test_server.h
 * @author Mateusz Makowski
 * @date 21.09.2018
 * @brief Mock server for running tests that require remote connection.
 */
#ifndef IRCBOT_TESTS_TEST_SERVER_H_
#define IRCBOT_TESTS_TEST_SERVER_H_

#ifdef _WIN32
typedef unsigned int UINT_PTR;
typedef UINT_PTR SOCKET;
#else
typedef int SOCKET;
#define INVALID_SOCKET ((SOCKET)(~0))
#define SOCKET_ERROR (-1)
#endif

#define MAXBUFLEN 512

struct sockaddr_in;

typedef struct Mock_Server {
    SOCKET socket_fd;
    SOCKET socket_client;
    char buffer[MAXBUFLEN];
    struct sockaddr_in *server;
    struct sockaddr_in *client;
} Mock_Server;

Mock_Server *server_create();
int server_read(Mock_Server* server);
int server_send(Mock_Server* server, const char msg[]);

#endif // IRCBOT_TESTS_TEST_SERVER_H_
