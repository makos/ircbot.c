// Yes MS, I know those functions can cause memory errors.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "bot.h"
#include "chatter.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

static const char *get_prefix_chunk(const char *prefix, const char *delimiter_one,
                              const char *delimiter_two)
{
    int chunk_start_index = strcspn(prefix, delimiter_one);
    int chunk_end_index = strcspn(prefix + chunk_start_index + 1, delimiter_two)
                          + chunk_start_index;
    // printf(
    //     "DEBUG delimiter_one: |%s| delimiter_two: |%s| chunk_end_index:
    //     %i\n", delimiter_one, delimiter_two, chunk_end_index);
    int chunk_len = chunk_end_index - chunk_start_index;

    char *chunk = malloc(MAX_ARRAY_LEN * sizeof(char));

    strncpy(chunk, prefix + chunk_start_index + 1, chunk_len);
    chunk[chunk_len] = '\0';

    return chunk;
}

static void parse_incoming_data(IRC_Bot *bot)
{
    // http://www.networksorcery.com/enp/protocol/irc.htm
    // "The presence of a prefix is indicated with a single leading ASCII colon
    // character", hence this check.
    int has_prefix = FALSE;
    if (strncmp(bot->last_msg, ":", 1) == 0)
        has_prefix = TRUE;

    Message *incoming_msg = malloc(sizeof(Message));

    if (has_prefix) {
        int prefix_len = strcspn(bot->last_msg, " ");
        int nick_end_index = strcspn(bot->last_msg, "!");
        // If position of "!" character is before the total length of prefix,
        // we have full "nick!user@host" prefix. Otherwise, it's only a server
        // address (e.g. "irc.rizon.no"). If strcspn() doesn't find a match,
        // it returns the full string length (equivalent to strlen()).
        if (nick_end_index > prefix_len) {
            strcpy(incoming_msg->servername,
                   get_prefix_chunk(bot->last_msg, ":", " "));
            // DEBUG:
            // printf("DEBUG incoming_msg->servername: |%s|\n",
            //    incoming_msg->servername);
        } else {
            // Copy relevant info from prefix. I don't have use for this data
            // yet.
            strcpy(incoming_msg->nickname,
                   get_prefix_chunk(bot->last_msg, ":", "!"));
            strcpy(incoming_msg->user,
                   get_prefix_chunk(bot->last_msg, "!", "@"));
            strcpy(incoming_msg->host,
                   get_prefix_chunk(bot->last_msg, "@", " "));

            // DEBUG:
            printf("DEBUG incoming_msg: |%s| |%s| |%s|\n",
                   incoming_msg->nickname, incoming_msg->user,
                   incoming_msg->host);
        }
        // Now let's get the command and its parameters.
        strcpy(incoming_msg->command,
               get_prefix_chunk(bot->last_msg, " ", " "));
        // DEBUG:
        printf("DEBUG command: |%s|\n", incoming_msg->command);

        // TODO: Parameter grabbing (IRC allows up to 15 parameters per
        // command)
        // int parameters_index = strcspn(bot->last_msg + prefix_len + 1, " ") +
        // prefix_len; DEBUG: printf("DEBUG parameters_index: %i\n",
        // parameters_index);

        // strcpy(incoming_msg->parameters,
        //        get_prefix_chunk(bot->last_msg, " ", ":", parameters_index));

        // DEBUG:
        // printf("DEBUG parameters: |%s|\n", incoming_msg->parameters);
    } else {
        // No prefix; probably PING message.
        if (strncmp(bot->last_msg, "PING", 4) == 0) {
            commands_pong(bot);
        }
    }
}

int main()
{
    char *channels[] = {"#dailyprog", "#ircbot_ctest"};
    ChatterStatus *status = malloc(sizeof(ChatterStatus));
    status->connected = FALSE;
    status->motd_finished = FALSE;

    IRC_Bot *chatter = bot_create("chatterbot");

    // DEBUG: Localhost server testing.
    if (bot_connect(chatter, "127.0.0.1", "8080") == TRUE)
        status->connected = TRUE;

    // if (bot_connect(chatter, "irc.rizon.net", "6660") == TRUE)
    //     status->connected = TRUE;

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
