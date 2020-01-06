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
#define RECV_ERROR -1

static int bot_recv(IRC_Bot *bot)
{
    if (!bot) {
        return ERROR;
    }
    memset(bot->last_msg, 0, BOT_MAX_MSGLEN);

    int bytes = connection_read(bot->connection);
    if (bytes > 0) {
        for (int i = 0; i < bytes; i++) {
            bot->last_msg[i] = bot->connection->recvbuf[i];
        }

        if (bytes < BOT_MAX_MSGLEN) {
            bot->last_msg[bytes] = '\0';
        } else {
            return RECV_ERROR;
        }
    }

    return bytes;
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

    // temp->channels = malloc(BOT_MAX_CHANNEL_AMT);
    // for (int i = 0; i < BOT_MAX_CHANNEL_AMT; i++) {
    //     temp->channels[i] = malloc(BOT_MAX_CHANNEL_LEN * sizeof(char));
    //     strcpy(temp->channels[i], "");
    // }

    temp->last_channel_id = -1;

    return temp;
}

int bot_connect(IRC_Bot *bot, const char address[], const char port[])
{
    if (!bot) {
        fprintf(stderr, "bot_connect(): bot is NULL\n");
        return ERROR;
    }

    if (strlen(address) > 32 || strlen(port) > 8) {
        fprintf(stderr, "bot_connect(): address or port are too long\n");
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
    return connection_disconnect(bot->connection);
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

int bot_read(IRC_Bot *bot)
{
    if (!bot) {
        return ERROR;
    }

    return bot_recv(bot);
}

// int bot_join(IRC_Bot *bot, const char channel[])
// {
//     if (!bot || !bot->connection) {
//         fprintf(stderr, "bot_join(): bot or connection are not initialized\n");
//         return ERROR;
//     }

//     bot->last_channel_id++;

//     strcpy(bot->channels[bot->last_channel_id], channel);

//     // +5 chars because of "JOIN "
//     char *join_msg = malloc((BOT_MAX_CHANNEL_LEN + 5) * sizeof(char));
//     strcpy(join_msg, "JOIN ");
//     strcat(join_msg, channel);

//     if (bot_send(bot, join_msg) == ERROR) {
//         fprintf(stderr, "bot_join(): bot_send() returned ERROR\n");
//         return ERROR;
//     }

//     return OK;
// }

int bot_leave(IRC_Bot *bot, const char channel[])
{
    if (!bot) {
        fprintf(stderr, "bot_leave(): bot is not initialized\n");
        return ERROR;
    }

    char *leave_msg = malloc((strlen(channel) + 7) * sizeof(char));
    strcpy(leave_msg, "LEAVE ");
    strcat(leave_msg, channel);

    return bot_send(bot, leave_msg);
}

void bot_process_forever(IRC_Bot *bot)
{
}
