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

/** Function pointer used in Bot_Command to execute custom functions. */
typedef int (*fp_cmd_t)(struct IRC_Bot *);
/** Maximum amount of Bot_Command structures in IRC_Bot. */
#define BOT_MAX_COMMANDS 128
/** Maximum length of stored message received from server. */
#define BOT_MAX_MSGLEN 512
/** Max characters in channel name. */
#define BOT_MAX_CHANNEL_LEN 64
/** Max channels the bot can join. */
#define BOT_MAX_CHANNEL_AMT 128

/** Struct used to add custom commands to the bot. */
typedef struct Bot_Command {
    /** Name of the command, by which it is invoked. */
    char *name;
    /** Function pointer to the function to be executed. */
    fp_cmd_t callback;
} Bot_Command;

/** Struct holding relevant data to the actual bot. */
typedef struct IRC_Bot {
    /** Nick visible on the IRC server. */
    char *nick;
    /** List of channels currently connected to. */
    char **channels;
    /** Pointer to the IRC_Connection structure. */
    struct IRC_Connection *connection;
    /** Array of pointers to Bot_Command structures. */
    Bot_Command *commands[BOT_MAX_COMMANDS];
    /**
     * Specifies the ID of the last command added, not to be used outside of
     * `bot_` functions.
     */
    int newest_cmd;
    /**
     * Last received message.
     * @see BOT_MAX_MSGLEN.
     */
    char last_msg[BOT_MAX_MSGLEN];
    /** Index of the last channel joined. */
    int last_channel_id;
} IRC_Bot;

/**
 * Create and populate a new IRC_Bot structure.
 * @param nick visible nickname on the IRC server
 * @return pointer to the newly created IRC_Bot object or null pointer on
 * failure
 */
IRC_Bot *bot_create(const char nick[]);
/**
 * Add Bot_Command to the IRC_Bot.
 * @param bot pointer to IRC_Bot object
 * @param name command name
 * @param callback function pointer to be called when executing this command
 * @return 0 on failure, 1 on success
 */
int bot_add_command(IRC_Bot *bot, const char name[], fp_cmd_t callback);
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
 * Call (execute) a Bot_Command.
 * @param bot pointer to IRC_Bot object
 * @param cmd command name to be called
 * @return 0 on failure, 1 on success
 */
int bot_call(IRC_Bot *bot, const char cmd[]);

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
 * @return negative numbers on socket errors, 0 - connection closed, positive
 * numbers denote bytes read
 * @see last_msg
 */
int bot_read(IRC_Bot *bot);

/**
 * Join an IRC channel.
 * @param bot pointer to IRC_Bot object
 * @param channel channel to join
 * @return 0 on failure, 1 on success
 */
int bot_join(IRC_Bot *bot, const char channel[]);

/**
 * Leave an IRC channel.
 * @param bot pointer to IRC_Bot object
 * @param channel channel to leave
 * @return 0 on failure, 1 on success
 */
int bot_leave(IRC_Bot *bot, const char channel[]);

#endif // IRCBOT_BOT_BOT_H_
