/**
 * @file chatter.h
 * @author Mateusz Makowski
 * @date 27.01.2020
 * @brief File containing the Chatter data structures and definitions.
 */
#ifndef CHATTER_H_
#define CHATTER_H_

/**
 * Maximum size of an array in bytes. IRC specification mandates all messages
 * sent to be 512 bytes in size, including ending CR-LF characters (`\r\n`).
 */
#define MAX_ARRAY_LEN 512
/**
 * Index of the first ":" in an incoming IRC message, that announces a prefix is
 * present.
 */
#define PREFIX_DELIMITER_OFFSET 1

struct IRC_Bot;
/**
 * Struct holding data relevant to the current status of the bot.
 */
typedef struct ChatterStatus {
    /**
     * Boolean value representing whether numeric command `RPL_ENDOFMOTD` 
     * (`376`) was received. 
     */
    
    _Bool motd_finished;
    /**
     * Boolean value representing whether the bot is currently connected to a
     * server.
     */
    _Bool connected;
} ChatterStatus;

#endif // CHATTER_H_
