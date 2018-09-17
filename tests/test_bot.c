#include "bot/bot.h"
#include "minunit.h"
#include <string.h>

static void bot_pong(IRC_Bot *bot)
{
    int ponged = 0;
    int bytes = 0;
    do {
        bytes = bot_read(bot);

        if (strncmp(bot->last_msg, "PING", 4) == 0) {
            ponged = 1;
            char *id = strtok(bot->last_msg, ":");
            id = strtok(NULL, ":");
            char *pong_msg = malloc(32 * sizeof(char));
            strcpy(pong_msg, "PONG ");
            strcat(pong_msg, id);
            bot_send(bot, pong_msg);
        }
    } while (!ponged);
}

static int test_callback(IRC_Bot *bot)
{
    return 1;
}

static char *test_bot_create_success()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_create_success(): test_bot is NULL", test_bot);
    mu_assert("test_bot_create_success(): test_bot->nick != \"ircbot\"",
              strcmp(test_bot->nick, "ircbot") == 0);
    return 0;
}

static char *test_bot_create_failure()
{
    IRC_Bot *test_bot = bot_create("");
    mu_assert("test_bot_create_failure(): test_bot is not NULL", !test_bot);
    return 0;
}

static char *test_bot_add_command_success()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_add_command_success(): test_bot is NULL", test_bot);

    fp_cmd_t test_cmd = test_callback;

    mu_assert("test_bot_add_command_success(): bot_add_command() failed",
              bot_add_command(test_bot, "test_cmd", test_cmd) != 0);

    mu_assert(
        "test_bot_add_command(): test_bot->commands[0]->name != \"test_cmd\"",
        strcmp(test_bot->commands[0]->name, "test_cmd") == 0);

    mu_assert("test_bot_add_command(): test_bot->commands[0]->callback() != 1",
              test_bot->commands[0]->callback(test_bot) != 0);
    return 0;
}

static char *test_bot_add_command_failure()
{
    mu_assert(
        "test_bot_add_command_failure(): bot_add_command() returned OK (1)",
        bot_add_command(NULL, "test", test_callback) == 0);
    return 0;
}

static char *test_bot_connect_success()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_connect_success(): test_bot is NULL", test_bot);
    int result = bot_connect(test_bot, "irc.rizon.net", "6660");
    bot_disconnect(test_bot);
    mu_assert("test_bot_connect_success(): bot_connect() failed", result != 0);
    return 0;
}

static char *test_bot_connect_failure()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_connect_failure(): test_bot is NULL", test_bot);
    int result = bot_connect(test_bot, "garbage", "this is over 8 chars");
    bot_disconnect(test_bot);
    mu_assert("test_bot_connect_failure(): bot_connect() succeeded",
              result == 0);
    return 0;
}

static char *test_bot_send_success()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_send_success(): test_bot is NULL\n", test_bot);
    bot_connect(test_bot, "irc.rizon.net", "6660");
    int result = bot_send(test_bot, "USER ircbot ircbot ircbot ircbot");
    bot_disconnect(test_bot);
    mu_assert("test_bot_send_success(): bot_send() returned ERROR\n",
              result != 0);
    return 0;
}

static char *test_bot_send_failure()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_send_success(): test_bot is NULL\n", test_bot);
    int result = bot_send(test_bot, "USER ircbot ircbot ircbot ircbot");
    mu_assert("test_bot_send_success(): bot_send() returned ERROR\n",
              result == 0);
    return 0;
}

static char *test_bot_read_success()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_read_success(): test_bot is NULL\n", test_bot);
    bot_connect(test_bot, "irc.rizon.net", "6660");
    int result = bot_read(test_bot);
    mu_assert("test_bot_read_success(): bot_read() returned 0 or ERROR",
              result > 0);
    bot_disconnect(test_bot);
    return 0;
}

static char *test_bot_read_failure()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_read_failure(): test_bot is NULL\n", test_bot);
    int result = bot_read(test_bot);
    mu_assert(
        "test_bot_read_failure(): bot_read() returned positive numbers (not ERROR)",
        result <= 0);
    return 0;
}

static char *test_bot_join_success()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_join_success(): test_bot is NULL\n", test_bot);

    bot_connect(test_bot, "irc.rizon.net", "6660");

    bot_send(test_bot, "USER ircbot ircbot ircbot ircbot");
    bot_send(test_bot, "NICK ircbot_c");

    bot_pong(test_bot);

    int result = bot_join(test_bot, "#ircbot_ctest");
    mu_assert("test_bot_join_success(): bot_join returned ERROR\n",
              result != 0);

    return 0;
}

static char *test_bot_join_failure()
{
    IRC_Bot *test_bot = bot_create("ircbot");
    mu_assert("test_bot_join_failure(): test_bot is NULL\n", test_bot);

    int result = bot_join(test_bot, "#ircbot_ctest");
    mu_assert("test_bot_join_failure(): bot_join returned OK\n", result == 0);

    return 0;
}
