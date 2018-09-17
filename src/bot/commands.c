/**
 * @file commands.c
 * @author Mateusz Makowski
 * @date 16.09.2018
 * @brief Implementation of commands.h.
 */
#include "commands.h"
#include "bot.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define ERROR 0

int commands_ident(IRC_Bot *bot)
{
    if (!bot) {
        return ERROR;
    }
    // 9 characters more, because 4 spaces, \0 and USER are added.
    size_t mem_size = (strlen(bot->nick) * 4) + 9;
    char *msg = malloc(mem_size * sizeof(char));
    strcpy(msg, "USER");
    for (int i = 0; i < 4; i++) {
        strcat(msg, " ");
        strcat(msg, bot->nick);
    }

    // Send the "USER nick nick nick nick" message.
    int result = bot_send(bot, msg);
    if (!result) {
        return ERROR;
    }

    // Send the "NICK nick" message.
    memset(msg, 0, mem_size);
    strcpy(msg, "NICK ");
    strcat(msg, bot->nick);
    return bot_send(bot, msg);
}

int commands_pong(IRC_Bot *bot)
{
    if (!bot) {
        return ERROR;
    }

    return ERROR;
}
