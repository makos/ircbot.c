// Yes MS, I know those functions can cause memory errors.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "chatter.h"
#include "bot.h"
#include "commands.h"
#include "debug.h"
#include "ircmessage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

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

        split_message(chatter->last_msg);
    }

    bot_disconnect(chatter);
    return 0;
}
