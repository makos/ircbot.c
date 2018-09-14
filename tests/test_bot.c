#include "bot/bot.h"
#include "minunit.h"
#define NULL ((void *)0)

static char *test_bot_create_success()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_create_success(): test_bot is NULL", test_bot != NULL);
    return 0;
}
