/**
 * @file commands.h
 * @author Mateusz Makowski
 * @date 16.09.2018
 * @brief File containing bot command functions.
 */
#ifndef IRCBOT_BOT_COMMANDS_H_
#define IRCBOT_BOT_COMMANDS_H_
struct IRC_Bot;
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
int commands_pong(struct IRC_Bot *bot, const char pong_id[]);
#endif // IRCBOT_BOT_COMMANDS_H_
