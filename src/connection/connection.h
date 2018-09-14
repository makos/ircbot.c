#ifndef IRCBOT_CONNECTION_CONNECTION_H_
#define IRCBOT_CONNECTION_CONNECTION_H_

#ifdef _WIN32
typedef unsigned int UINT_PTR;
typedef UINT_PTR SOCKET;
#else
typedef int SOCKET;
#define INVALID_SOCKET ((SOCKET)(~0))
#define SOCKET_ERROR (-1)
#endif

typedef struct IRC_Connection {
    SOCKET socket;
    struct addrinfo *ai_result;
    struct addrinfo *ai_hints;
    char address[32];
    char port[8];
    char *recvbuf;
    int recvbuflen;
} IRC_Connection;

IRC_Connection *connection_create(const char address[], const char port[]);

int connection_connect(IRC_Connection *connection);
int connection_send(IRC_Connection *connection, const char msg[]);
int connection_read(IRC_Connection *connection);
int connection_disconnect(IRC_Connection *connection);

#endif // IRCBOT_CONNECTION_CONNECTION_H_
