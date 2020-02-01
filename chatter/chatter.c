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
#include <stdbool.h>

static ChatterStatus *construct_status()
{
    ChatterStatus *temp = malloc(sizeof(ChatterStatus));
    temp->connected = false;
    temp->motd_finished = false;

    return temp;
}

int main()
{
    // char *channels[] = {"#dailyprog", "#ircbot_ctest"};
    debug_set_out("stderr");
    //debug_enable();

    ChatterStatus *status = construct_status();

    IRC_Bot *chatter = bot_create("chatterbot");

    Message *current_msg = NULL;

    // Localhost server testing.
    if (bot_connect(chatter, "127.0.0.1", "8080") == true)
        status->connected = true;

    /*if (bot_connect(chatter, "chat.freenode.org", "6665") == true)
        status->connected = true;*/
    
    bot_send(chatter, "USER chatterbot chatterbot chatterbot chatterbot");
    bot_send(chatter, "NICK chatterbot");

    while (1) {
        bot_read(chatter);

        current_msg = split_message(chatter->last_msg);

        printf("%s", chatter->last_msg);

        handle_command(chatter, current_msg);
    }

    bot_disconnect(chatter);
    return 0;
}
