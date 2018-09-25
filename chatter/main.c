#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bot.h"
#include "commands.h"

static void parse_input(IRC_Bot* bot)
{
    if (strncmp(bot->last_msg, "PING", 4) == 0) {
        commands_pong(bot);
    } else if (strncmp(bot->last_msg, "Hello bot", 10) == 0) {
        bot_send(bot, "MSG #ircbot_ctest Hello");
    }
}
// This segfaults???
int main()
{
    char *channels[] = {
        "#dailyprog",
        "#ircbot_ctest"
    };

    IRC_Bot *chatter = bot_create("chatterbot");
    /* bot_connect(chatter, "irc.rizon.net", "6660"); */

    /* bot_send(chatter, "USER chatterbot chatterbot chatterbot chatterbot"); */
    /* bot_send(chatter, "NICK chatterbot"); */

    int joined = 0;
    /* while (1) { */
    /*     bot_read(chatter); */

    /*     printf("%s", chatter->last_msg); */

    /*     parse_input(chatter); */

    /*     /\* if (!joined) { *\/ */
    /*     /\*     bot_join(chatter, "#ircbot_ctest"); *\/ */
    /*     /\*     joined = 1; *\/ */
    /*     /\* } *\/ */
    /* } */

    /* bot_disconnect(chatter); */
    return 0;
}
