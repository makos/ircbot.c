#include "bot/bot.h"
#include "bot/commands.h"
#include "minunit.h"
#include <string.h>

static char *test_bot_ident_success()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_ident_success(): test_bot is NULL", test_bot);

    bot_add_command(test_bot, "ident", commands_ident);

    bot_connect(test_bot, "irc.rizon.net", "6660");

    mu_assert_or_disconnect(
        test_bot, "test_bot_ident_success(): bot_call() returned ERROR",
        bot_call(test_bot, "ident") != 0);

    bot_disconnect(test_bot);
    return 0;
}
