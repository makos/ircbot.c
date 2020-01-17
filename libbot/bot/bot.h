/**
 * @file bot.h
 * @author Mateusz Makowski
 * @date 15.09.2018
 * @brief File containing the bot data structure and related functions.
 */
#ifndef IRCBOT_BOT_BOT_H_
#define IRCBOT_BOT_BOT_H_
struct IRC_Bot;
struct IRC_Connection;

/** Maximum length of stored message received from server. */
#define BOT_MAX_MSGLEN 512
/** Max characters in channel name. */
#define BOT_MAX_CHANNEL_LEN 64
/** Max channels the bot can join. */
#define BOT_MAX_CHANNEL_AMT 128

/** Struct holding relevant data to the actual bot. */
typedef struct IRC_Bot {
    /** Nick visible on the IRC server. */
    char *nick;
    /** Pointer to the IRC_Connection structure. */
    struct IRC_Connection *connection;
    /**
     * Last received message. Note that this is NOT a null-terminated string
     * (yet), so be cautious when using `printf()`.
     */
    char last_msg[BOT_MAX_MSGLEN];
} IRC_Bot;

/**
 * Create and populate a new IRC_Bot structure.
 * @param nick visible nickname on the IRC server
 * @return pointer to the newly created IRC_Bot object or null pointer on
 * failure
 */
IRC_Bot *bot_create(const char nick[]);

/**
 * Connect IRC_Bot to the server.
 * @param bot pointer to IRC_Bot object
 * @param address IRC server address to connect to
 * @param port IRC port number (as a string)
 * @return 0 on failure, 1 on success
 */
int bot_connect(IRC_Bot *bot, const char address[], const char port[]);

/**
 * Disconnect IRC_Bot from the server.
 * @param bot pointer to IRC_Bot object
 * @return 0 on failure, 1 on success
 */
int bot_disconnect(IRC_Bot *bot);

/**
 * Send a text message to connected server.
 * @param bot pointer to IRC_Bot object
 * @param msg message string to be sent (doesn't have to include trailing
 * newline)
 * @return 0 on failure, 1 on success
 */
int bot_send(IRC_Bot *bot, const char msg[]);

/**
 * Read incoming messages from server, one message at a time.
 * @param bot pointer to IRC_Bot object
 * @return negative numbers on socket errors, 1 - connection closed, positive
 * numbers denote bytes read
 * @see last_msg
 */
int bot_read(IRC_Bot *bot);

#endif // IRCBOT_BOT_BOT_H_
