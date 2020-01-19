/**
 * @file commands.h
 * @author Mateusz Makowski
 * @date 16.09.2018
 * @brief File containing bot command functions.
 */
#ifndef IRCBOT_BOT_COMMANDS_H_
#define IRCBOT_BOT_COMMANDS_H_

#define MAX_ARRAY_LEN 512

struct IRC_Bot;

typedef struct Command_Switches {
    char version[MAX_ARRAY_LEN];
    char say_hello[MAX_ARRAY_LEN];
} Command_Switches;

/**
 * Send an ident response to the server.
 * @param bot pointer to IRC_Bot structure
 * @return 0 on failure, 1 on success
 */
int commands_ident(struct IRC_Bot *bot);

/**
 * Send a PONG message to the server.
 * @param bot pointer to IRC_Bot structure
 * @return 0 on failure, 1 on success
 */
int commands_pong(struct IRC_Bot *bot);

/*
 * Send a VERSION response.
 * @param bot pointer to IRC_Bot structure
 * @return 0 on failure, 1 on success
 */
int commands_privmsg(struct IRC_Bot *bot, const char *origin_username,
                     const char *message);

void setup_commands(struct IRC_Bot *bot);
#endif // IRCBOT_BOT_COMMANDS_H_