#include "bot.h"
#include "commands.h"
#include "chatter.h"
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
        int prefix_delimiter_offset = 1; // This is always 1; maybe should be a constant?
        int only_servername = FALSE;
        int prefix_len = strcspn(bot->last_msg, " ");
        int nickname_len = strcspn(bot->last_msg, "!");
        // If position of "!" character is before the total length of prefix,
        // we have full "nick!user@host" prefix. Otherwise, it's only a server
        // address (e.g. "irc.rizon.no"). If strcspn() doesn't find a match,
        // it returns the full string length (equivalent to strlen()).
        if (nickname_len > prefix_len) {
            strncpy(incoming_msg->servername, bot->last_msg + prefix_delimiter_offset, prefix_len - prefix_delimiter_offset);
            // DEBUG
            printf("DEBUG: incoming_msg->servername: |%s|\n", incoming_msg->servername);
        } else {
            int user_len = strcspn(bot->last_msg, "@");
        }
    }
}

int main()
{
    char *channels[] = {"#dailyprog", "#ircbot_ctest"};

    IRC_Bot *chatter = bot_create("chatterbot");
    bot_connect(chatter, "irc.rizon.net", "6660");

    bot_send(chatter, "USER chatterbot chatterbot chatterbot chatterbot");
    bot_send(chatter, "NICK chatterbot");

    int joined = 0;
    while (1) {
        bot_read(chatter);

        printf("%s", chatter->last_msg);

        parse_incoming_data(chatter);

        // if (!joined) {
        //     bot_join(chatter, "#ircbot_ctest");
        //     joined = 1;
        // }
    }

    bot_disconnect(chatter);
    return 0;
}
