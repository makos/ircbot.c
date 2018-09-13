#include "client/client.h"
#include "server/irc_server.h"
#include <stdio.h>
#include <winsock2.h>

/* TODO: list of things I'd like to have:
 * command-line for the bot (send message, whisper, join/leave channels)
 * UTF-8 printing
 * cross-platform
 * multi-channel capability
 * automatic response to common IRC queries
 * some ice cream
 */


int init_winsock()
{
    WSADATA wsa_data;
    int result = 0;

    result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        printf("WSAStartup error: %d\n", result);
        return 0;
    }
    return 1;
}

int main()
{
    if (init_winsock() == 0) {
        return 1;
    }

    IRC_Server *server = server_create("irc.rizon.net", "6660");
    if (server == NULL) {
        WSACleanup();
        return 1;
    }

    IRC_Client *client = client_create();
    if (client == NULL) {
        WSACleanup();
        return 1;
    }

    client_connect(client, server);
    client_send(client, "USER ircbotc ircbotc ircbotc ircbotc\n");
    client_send(client, "NICK ircbot_c\n");

    int bytes = 0;
    do {
        bytes = client_receive(client);

        if (bytes < 0) {
            client_disconnect(client);
            WSACleanup();
            return 1;
        }

        for (int i = 0; i < bytes; i++) {
            printf("%c", client->recvbuf[i]);
        }

        client_parse(client);
        client_clear_buffer(client);
    } while (bytes > 0);

    client_disconnect(client);

    WSACleanup();
    return 0;
}
