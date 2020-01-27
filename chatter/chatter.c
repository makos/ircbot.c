/**
 * @file chatter.c
 * @author Mateusz Makowski
 * @date 27.01.2020
 * @brief Implementation of chatter.h along with many `static` functions.
 */
// Yes MS, I know those functions can cause memory errors.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "chatter.h"
#include "bot.h"
#include "commands.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

static Message *construct_message()
{
    return calloc((size_t)1, sizeof(Message));
}

static int check_prefix(const char *msg)
{
    if (strncmp(msg, ":", 1) == 0)
        return TRUE;
    return FALSE;
}

static void backup_msg(char *dest, const char *source)
{
    strcpy(dest, source);
}

static int get_prefix_len(const char *msg)
{
    return strcspn(msg, " ");
}

static int get_nick_end_index(const char *msg)
{
    return strcspn(msg, "!");
}

static void parse_incoming_data(IRC_Bot *bot)
{
    // http://www.networksorcery.com/enp/protocol/irc.htm
    // "The presence of a prefix is indicated with a single leading ASCII colon
    // character", hence this check.
    int has_prefix = check_prefix(bot->last_msg);
    Message *incoming_msg = construct_message();
    backup_msg(incoming_msg->msg_copy, bot->last_msg);

    char *tok = NULL;

    if (has_prefix) {
        int prefix_len = get_prefix_len(bot->last_msg);
        int nick_end_index = get_nick_end_index(bot->last_msg);

        debug_log("DEBUG prefix_len: %i nick_end_index: %i\n", prefix_len,
                  nick_end_index);
        debug_log("DEBUG strlen(bot->last_msg): %i\n", strlen(bot->last_msg));
        // If position of "!" character is before the total length of prefix,
        // we have full "nick!user@host" prefix. Otherwise, it's only a server
        // address (e.g. "irc.rizon.no"). If strcspn() doesn't find a match,
        // it returns the full string length (equivalent to strlen()).
        if (nick_end_index > prefix_len) {
            tok = strtok(incoming_msg->msg_copy + PREFIX_DELIMITER_OFFSET, " ");
            strcpy(incoming_msg->servername, tok);

            debug_log("DEBUG incoming_msg->servername: |%s|\n",
                      incoming_msg->servername);
        } else {
            // Copy relevant info from prefix. I don't have use for this data
            // yet.
            tok = strtok(incoming_msg->msg_copy + PREFIX_DELIMITER_OFFSET, "!");
            strcpy(incoming_msg->nickname, tok);

            tok = strtok(NULL, "@");
            strcpy(incoming_msg->user, tok);

            tok = strtok(NULL, " ");
            strcpy(incoming_msg->host, tok);

            debug_log("DEBUG incoming_msg: |%s| |%s| |%s|\n",
                      incoming_msg->nickname, incoming_msg->user,
                      incoming_msg->host);
        }
        // Now let's get the command and its parameters.
        tok = strtok(NULL, " ");
        strcpy(incoming_msg->command, tok);

        debug_log("DEBUG command: |%s|\n", incoming_msg->command);

        for (int i = 0; i < MAX_ARRAY_LEN; i++) {
            incoming_msg->parameters[i] = '\0';
        }

        while ((tok = strtok(NULL, " "))) {
            if (strncmp(tok, ":", 1) == 0)
                break;
            strcat(incoming_msg->parameters, tok);
            strcat(incoming_msg->parameters, " ");
        }

        // Remove trailing space.
        int param_len = strlen(incoming_msg->parameters) - 1;
        incoming_msg->parameters[param_len] = '\0';

        debug_log("DEBUG parameters: |%s|\n", incoming_msg->parameters);

        // TODO: This is ugly, prettify it somehow.
        strcat(incoming_msg->trailing, tok + 1);
        strcat(incoming_msg->trailing, " ");
        tok = strtok(NULL, "");
        // Check for NULL, because if there's only one word after the trailing
        // ':' it will segfault.
        if (tok != NULL)
            strcat(incoming_msg->trailing, tok);

        // TODO: Check if it was a server message or other client message and
        // format it accordingly. This is done to avoid empty nickname fields
        // when it's a server message.
        // debug_log("[%s]: %s\r\n", incoming_msg->nickname,
        // incoming_msg->trailing);

        // TODO: Send the parsed command and parameters to some handler.
        // e.g.
        // handle_command(incoming_msg->command, incoming_msg->parameters);
    } else {
        // No prefix; probably PING message.
        if (strncmp(bot->last_msg, "PING", 4) == 0) {
            commands_pong(bot);
        }
    }

    free(incoming_msg);
}

int main()
{
    // char *channels[] = {"#dailyprog", "#ircbot_ctest"};
    debug_set_out("stderr");
    debug_enable();

    ChatterStatus *status = malloc(sizeof(ChatterStatus));
    status->connected = FALSE;
    status->motd_finished = FALSE;

    IRC_Bot *chatter = bot_create("chatterbot");

    // Localhost server testing.
    if (bot_connect(chatter, "127.0.0.1", "8080") == TRUE)
        status->connected = TRUE;

    /*if (bot_connect(chatter, "chat.freenode.org", "6665") == TRUE)
        status->connected = TRUE;*/

    bot_send(chatter, "USER chatterbot chatterbot chatterbot chatterbot");
    bot_send(chatter, "NICK chatterbot");

    while (1) {
        bot_read(chatter);

        printf("%s", chatter->last_msg);

        parse_incoming_data(chatter);
    }

    bot_disconnect(chatter);
    return 0;
}
