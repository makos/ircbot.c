#ifndef IRCBOT_CLIENT_CLIENT_H_
#define IRCBOT_CLIENT_CLIENT_H_
#define MAXBUFLEN 512
#define ERROR 0
#define OK 1

typedef struct IRC_Server IRC_Server;
typedef unsigned int UINT_PTR;
typedef UINT_PTR SOCKET;

typedef struct IRC_Client {
    char recvbuf[MAXBUFLEN + 1];
    SOCKET connect_socket;
} IRC_Client;

IRC_Client *client_create();

int client_connect(IRC_Client *client, IRC_Server *server);
int client_send(IRC_Client *client, const char msg[]);
int client_receive(IRC_Client *client);
void client_disconnect(IRC_Client *client);

#endif // IRCBOT_CLIENT_CLIENT_H_
