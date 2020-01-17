// Yes MS, I know those functions can cause memory errors.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "bot.h"
#include "chatter.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

static void parse_incoming_data(IRC_Bot *bot)
{
    // http://www.networksorcery.com/enp/protocol/irc.htm
    // "The presence of a prefix is indicated with a single leading ASCII colon
    // character", hence this check.
    int has_prefix = FALSE;
    if (strncmp(bot->last_msg, ":", 1) == 0)
        has_prefix = TRUE;

    Message *incoming_msg = malloc(sizeof(Message));

    strcpy(incoming_msg->msg_copy, bot->last_msg);
    char *tok = NULL;
    
    if (has_prefix) {
        int prefix_len = strcspn(bot->last_msg, " ");
        int nick_end_index = strcspn(bot->last_msg, "!");
        // If position of "!" character is before the total length of prefix,
        // we have full "nick!user@host" prefix. Otherwise, it's only a server
        // address (e.g. "irc.rizon.no"). If strcspn() doesn't find a match,
        // it returns the full string length (equivalent to strlen()).
        if (nick_end_index > prefix_len) {
            tok = strtok(incoming_msg->msg_copy + PREFIX_DELIMITER_OFFSET, " ");
            strcpy(incoming_msg->servername, tok);
            //strcpy(incoming_msg->servername,
                   //get_prefix_chunk(bot->last_msg, ":", " "));
            // DEBUG:
            printf("DEBUG incoming_msg->servername: |%s|\n",
                    incoming_msg->servername);
        } else {
            // Copy relevant info from prefix. I don't have use for this data
            // yet.
            tok = strtok(incoming_msg->msg_copy + PREFIX_DELIMITER_OFFSET, "!");
            strcpy(incoming_msg->nickname, tok);

            tok = strtok(NULL, "@");
            strcpy(incoming_msg->user, tok);

            tok = strtok(NULL, " ");
            strcpy(incoming_msg->host, tok);
            //strcpy(incoming_msg->nickname,
                   //get_prefix_chunk(bot->last_msg, ":", "!"));
            //strcpy(incoming_msg->user,
                   //get_prefix_chunk(bot->last_msg, "!", "@"));
            //strcpy(incoming_msg->host,
                   //get_prefix_chunk(bot->last_msg, "@", " "));

            // DEBUG:
            printf("DEBUG incoming_msg: |%s| |%s| |%s|\n",
                  incoming_msg->nickname, incoming_msg->user,
                  incoming_msg->host);
        }
        // Now let's get the command and its parameters.
        tok = strtok(NULL, " ");
        strcpy(incoming_msg->command, tok);
        //strcpy(incoming_msg->command,
               //get_prefix_chunk(bot->last_msg, " ", " "));
        // DEBUG:
        //printf("DEBUG command: |%s|\n", incoming_msg->command);
       
        // Grab a copy of the message for strtok().
        //char msg_copy[MAX_ARRAY_LEN];
        //strcpy(msg_copy, bot->last_msg);

        //char *tok = strtok(
                //msg_copy + prefix_len + strlen(incoming_msg->command) + 1,
                //" ");
        
        for (int i = 0; i < MAX_ARRAY_LEN; i++) {
            incoming_msg->parameters[i] = '\0'; 
        }

        while (strncmp(tok, ":", 1) != 0) {
            strcat(incoming_msg->parameters, tok);
            strcat(incoming_msg->parameters, " ");
            tok = strtok(NULL, " ");
        }

        strcpy(incoming_msg->trailing, tok);
        
        //while (tok) {
            //// DEBUG
            ////printf("DEBUG tok: %s\n", tok);
            //if (strncmp(tok, ":", 1) != 0) {
                //strcat(incoming_msg->parameters, tok);
                //strcat(incoming_msg->parameters, " ");
                //tok = strtok(NULL, " ");
            //} else {
                //// Above code adds a space after every parameter, we don't
                //// want it after the last one.
                //int param_len = strlen(incoming_msg->parameters);
                //incoming_msg->parameters[param_len - 1] = '\0';
                //tok = NULL;
            //}
        //}
        
        //strcpy(incoming_msg->trailing,
               //get_prefix_chunk(bot->last_msg + prefix_len, ":", "\r\n"));
        printf("[%s]: %s\r\n", incoming_msg->nickname, incoming_msg->trailing);
        // DEBUG:
        // printf("DEBUG parameters: |%s|\n", incoming_msg->parameters);
    } else {
        // No prefix; probably PING message.
        if (strncmp(bot->last_msg, "PING", 4) == 0) {
            commands_pong(bot);
        }
    }

    free(incoming_msg);
}

int main()
{
    char *channels[] = {"#dailyprog", "#ircbot_ctest"};
    ChatterStatus *status = malloc(sizeof(ChatterStatus));
    status->connected = FALSE;
    status->motd_finished = FALSE;

    IRC_Bot *chatter = bot_create("chatterbot");

    // DEBUG: Localhost server testing.
    if (bot_connect(chatter, "127.0.0.1", "8080") == TRUE)
        status->connected = TRUE;

    // if (bot_connect(chatter, "irc.rizon.net", "6660") == TRUE)
    //     status->connected = TRUE;

    bot_send(chatter, "USER chatterbot chatterbot chatterbot chatterbot");
    bot_send(chatter, "NICK chatterbot");

    while (1) {
        bot_read(chatter);

        printf("%s", chatter->last_msg);

        parse_incoming_data(chatter);
    }

    bot_disconnect(chatter);
    return 0;
}
