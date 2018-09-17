/**
 * @file bot.c
 * @author Mateusz Makowski
 * @date 16.09.2018
 * @brief Implementation of bot.h.
 */
#include "bot.h"
#include "connection/connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define ERROR 0

static Bot_Command *bot_find_command(IRC_Bot *bot, const char cmd[])
{
    if (!bot) {
        return NULL;
    }

    for (int i = 0; i <= bot->newest_cmd; i++) {
        if (strcmp(bot->commands[i]->name, cmd) == 0) {
            return bot->commands[i];
        }
    }

    return NULL;
}

IRC_Bot *bot_create(const char nick[])
{
    if (strlen(nick) < 1) {
        return NULL;
    }

    IRC_Bot *temp = malloc(sizeof(IRC_Bot));
    temp->connection = NULL;
    temp->nick = malloc((strlen(nick) + 1) * sizeof(char));
    strcpy(temp->nick, nick);

    for (int i = 0; i < BOT_MAX_COMMANDS; i++) {
        temp->commands[i] = malloc(sizeof(Bot_Command));
        temp->commands[i]->name = NULL;
        temp->commands[i]->callback = NULL;
    }

    temp->newest_cmd = -1;

    return temp;
}

int bot_add_command(IRC_Bot *bot, const char name[], fp_cmd_t callback)
{
    if (!bot) {
        printf("bot_add_command(): bot is NULL\n");
        return ERROR;
    }

    if (!callback) {
        printf("bot_add_command(): callback is NULL\n");
        return ERROR;
    }
    bot->newest_cmd++;

    bot->commands[bot->newest_cmd]->callback = callback;
    bot->commands[bot->newest_cmd]->name =
        malloc((strlen(name) + 1) * sizeof(char));
    strcpy(bot->commands[bot->newest_cmd]->name, name);

    return OK;
}

int bot_connect(IRC_Bot *bot, const char address[], const char port[])
{
    if (!bot) {
        printf("bot_connect(): bot is NULL\n");
        return ERROR;
    }

    if (strlen(address) > 32 || strlen(port) > 8) {
        printf("bot_connect(): address or port are too long\n");
        return ERROR;
    }

    bot->connection = connection_create(address, port);

    if (!bot->connection) {
        return ERROR;
    }

    if (connection_connect(bot->connection) == ERROR) {
        return ERROR;
    }

    return OK;
}

int bot_disconnect(IRC_Bot *bot)
{
    return connection_disconnect(bot);
}

int bot_call(IRC_Bot *bot, const char cmd[])
{
    if (!bot) {
        printf("bot_call(): bot is NULL\n");
        return ERROR;
    }

    Bot_Command *found = bot_find_command(bot, cmd);

    if (!found) {
        printf("bot_call(): %s not found\n", cmd);
        return ERROR;
    }

    return found->callback(bot);
}

int bot_send(IRC_Bot *bot, const char msg[])
{
    if (!bot) {
        fprintf(stderr, "bot_send(): bot is NULL\n");
        return ERROR;
    }
    if (!bot->connection) {
        fprintf(stderr, "bot_send(): connection is not initialized\n");
        return ERROR;
    }

    return connection_send(bot->connection, msg);
}
