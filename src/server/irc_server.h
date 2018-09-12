#ifndef IRCBOT_SERVER_IRC_SERVER_H_
#define IRCBOT_SERVER_IRC_SERVER_H_
struct addrinfo;

typedef struct IRC_Server {
    const char *address;
    const char *port;
    struct addrinfo *ai_result;
    struct addrinfo *ai_hints;
} IRC_Server;

IRC_Server *server_create(const char address[], const char port[]);

void server_connected();

#endif // IRCBOT_SERVER_IRC_SERVER_H_
