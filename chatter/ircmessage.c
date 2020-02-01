#include "ircmessage.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PREFIX_DELIMITER_OFFSET 1

char *TOK = NULL;
char *MSG = NULL;
bool FIRST_TOKEN = true;

static Message *construct_message()
{
    return calloc((size_t)1, sizeof(Message));
}

static int check_prefix(const char *msg)
{
    if (strncmp(msg, ":", 1) == 0)
        return true;
    return false;
}

static void backup_msg(char *dest, const char *source)
{
    strcpy(dest, source);
}

static int get_prefix_len(const char *msg)
{
    return strcspn(msg, " ");
}

static int get_nick_end_index(const char *msg)
{
    return strcspn(msg, "!");
}

static void extract_chunk(char *dest, const char *delim)
{
    if (TOK == NULL && FIRST_TOKEN == true) {
        TOK = strtok(MSG + PREFIX_DELIMITER_OFFSET, delim);
    } else {
        TOK = strtok(NULL, delim);
    }

    FIRST_TOKEN = false;
    strcpy(dest, TOK);
}

static void extract_parameters(char *dest)
{
    for (int i = 0; i < MAX_ARRAY_LEN; i++) {
        dest[i] = '\0';
    }

    while ((TOK = strtok(NULL, " "))) {
        if (strncmp(TOK, ":", 1) == 0)
            break;
        strcat(dest, TOK);
        strcat(dest, " ");
    }
    // Remove trailing space.
    int param_len = strlen(dest) - 1;
    dest[param_len] = '\0';
}

static void extract_trailing(char *dest)
{
    // Strip the leading ':'.
    strcat(dest, TOK + 1);
    strcat(dest, " ");
    TOK = strtok(NULL, "");
    // Check for NULL, because if there's only one word after the trailing
    // ':' it will segfault.
    if (TOK != NULL) {
        strcat(dest, TOK);
    }
}

Message *split_message(const char *msg)
{
    // http://www.networksorcery.com/enp/protocol/irc.htm
    // "The presence of a prefix is indicated with a single leading ASCII colon
    // character", hence this check.
    int has_prefix = check_prefix(msg);
    Message *temp = construct_message();
    backup_msg(temp->msg_copy, msg);

    // TODO: MSG is ugly, find another way to initialize strtok() without code
    // duplication.
    MSG = temp->msg_copy;
    FIRST_TOKEN = true;

    if (has_prefix) {
        int prefix_len = get_prefix_len(msg);
        int nick_end_index = get_nick_end_index(msg);

        // If position of "!" character is before the total length of prefix,
        // we have full "nick!user@host" prefix. Otherwise, it's only a server
        // address (e.g. "irc.rizon.no"). If get_nick_end_index() doesn't find a
        // match, it returns the full string length (equivalent to strlen()).
        if (nick_end_index > prefix_len) {
            extract_chunk(temp->servername, " ");

            debug_log("DEBUG temp->servername: |%s|\n", temp->servername);
        } else {
            extract_chunk(temp->nickname, "!");
            extract_chunk(temp->user, "@");
            extract_chunk(temp->host, " ");

            debug_log("DEBUG temp: |%s| |%s| |%s|\n", temp->nickname,
                      temp->user, temp->host);
        }
        // Now let's get the command and its parameters.
        extract_chunk(temp->command, " ");
        debug_log("DEBUG command: |%s|\n", temp->command);

        extract_parameters(temp->parameters);
        debug_log("DEBUG parameters: |%s|\n", temp->parameters);

        extract_trailing(temp->trailing);
        debug_log("DEBUG trailing: |%s|", temp->trailing);

        debug_log("[%s]: %s", temp->nickname, temp->trailing);
    }
    TOK = NULL;

    return temp;
}
