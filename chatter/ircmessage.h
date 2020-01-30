/**
 * @file ircmessage.h
 * @author Mateusz Makowski
 * @date 28.01.2020
 * @brief File containing @ref Message data structure and related functions.
 *        Refer to RFC 1459 section 2.3 for more details on message structure.
 */
#ifndef CHATTER_IRCMESSAGE_H__
#define CHATTER_IRCMESSAGE_H__

#ifndef MAX_ARRAY_LEN
#define MAX_ARRAY_LEN 512
#endif

/**
 * Struct holding parts of the incoming IRC message. The message can take two
 * formats:
 * - `:nick!user@host COMMAND [parameters ...] :[trailing content]` for client
 * messages
 * - `:servername COMMAND [parameters ...] :[trailing content]` for server
 * messages
 */
typedef struct Message {
    // Copy the message so we don't change bot's last_msg value.
    /**
     * Copy of the message taken from struct IRC_Bot last_msg[]. It is copied so
     * we don't alter the original via repeated calls to `strtok()`.
     */
    char msg_copy[MAX_ARRAY_LEN];
    /**
     * Extracted server name / address. Present in messages originating from a
     * server.
     */
    char servername[MAX_ARRAY_LEN];
    /**
     * Extracted nickname from the incoming message. Present in messages
     * originating from a client.
     */
    char nickname[MAX_ARRAY_LEN];
    /**
     * Extracted username from the incoming message. Present in messages
     * originating from a client.
     */
    char user[MAX_ARRAY_LEN];
    /**
     * Extracted hostname from the incoming message. Usually IP address,
     * hostmask or custom vhost.
     */
    char host[MAX_ARRAY_LEN];
    /**
     * Extracted command from the incoming message. Something like PRIVMSG,
     * VERSION, or a numeric equivalent such as `375` (`RPL_MOTDSTART`).
     */
    char command[MAX_ARRAY_LEN];
    /**
     * Extracted parameters of the issued command. Can be empty. IRC protocol
     * suggests sending up to 15 parameters in a single message.
     */
    char parameters[MAX_ARRAY_LEN];
    /**
     * What's left of the message after all technical stuff is stripped.
     * Usually just plain-text message from another client.
     */
    char trailing[MAX_ARRAY_LEN];
} Message;

Message *split_message(const char *msg);

#endif // CHATTER_IRCMESSAGE_H__
