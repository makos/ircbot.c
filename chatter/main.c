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
            int servername_len = prefix_len - PREFIX_DELIMITER_OFFSET;
            strncpy(incoming_msg->servername,
                    bot->last_msg + PREFIX_DELIMITER_OFFSET, servername_len);
            incoming_msg->servername[servername_len] = '\0';
            // DEBUG:
            // printf("DEBUG incoming_msg->servername: |%s|\n",
            //    incoming_msg->servername);
        } else {
            // TODO: Those three copy actions can be generalized and put into a
            // neat function, I just don't know how yet.

            // Copy nickname first.
            int nick_len = nick_end_index - PREFIX_DELIMITER_OFFSET;
            strncpy(incoming_msg->nickname,
                    bot->last_msg + PREFIX_DELIMITER_OFFSET, nick_len);
            incoming_msg->nickname[nick_len] = '\0';

            // Now copy username.
            int user_end_index = strcspn(bot->last_msg, "@");
            int user_len = user_end_index - (nick_end_index + 1);
            strncpy(incoming_msg->user, bot->last_msg + nick_end_index + 1,
                    user_len);
            incoming_msg->user[user_len] = '\0';

            // And the host.
            int host_len = prefix_len - (user_end_index + 1);
            strncpy(incoming_msg->host, bot->last_msg + user_end_index + 1,
                    host_len);
            incoming_msg->host[host_len] = '\0';

            // DEBUG:
            // printf("DEBUG incoming_msg: |%s| |%s| |%s|\n",
            // incoming_msg->nickname, incoming_msg->user, incoming_msg->host);

            // Now let's get the command and its parameters.
            int command_string_index = prefix_len + 1;
            int command_string_len =
                strcspn(bot->last_msg + command_string_index, " ");

            strncpy(incoming_msg->command, bot->last_msg + command_string_index,
                    command_string_len);
            incoming_msg->command[command_string_len] = '\0';
            // DEBUG:
            // printf("DEBUG command: |%s|\n", incoming_msg->command);

            // TODO: Parameter grabbing (IRC allows up to 15 parameters per
            // command)
        }
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
