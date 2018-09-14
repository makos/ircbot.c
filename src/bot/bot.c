#include "bot.h"
#include "client/client.h"
#include "server/irc_server.h"
#include <stdlib.h>
#include <string.h>

static void bot_init_channels(IRC_Bot *bot)
{
    bot->channel_list = malloc(sizeof(Bot_Channels));
    bot->channel_list->prev = NULL;
    bot->channel_list->next = NULL;
    bot->channel_list->channel_name = NULL;
}

static void bot_init_commands(IRC_Bot *bot)
{
    bot->commands = malloc(sizeof(Bot_Commands));
    bot->commands->pong = malloc(5 * sizeof(char));
    bot->commands->version = malloc(16 * sizeof(char));
    bot->commands->whois = malloc(52 * sizeof(char));
    strcpy(bot->commands->pong, "PONG");
    strcpy(bot->commands->version, "ircbot.c v0.1.0");
    strcpy(bot->commands->whois,
           "ircbot.c v0.1.0 by makos <github.com/makos/ircbot.c");
}

IRC_Bot *bot_create(const char address[], const char port[],
                    const char bot_nick[])
{
    IRC_Bot *temp = malloc(sizeof(IRC_Bot));
    bot_init_channels(temp);
    bot_init_commands(temp);

    temp->client = client_create();
    IRC_Server *server = server_create(server, port);
}
