#ifndef IRCBOT_BOT_BOT_H_
#define IRCBOT_BOT_BOT_H_
typedef struct IRC_Client IRC_Client;

typedef struct Bot_Channel {
    struct Bot_Channel *prev;
    char *channel_name;
    struct Bot_Channel *next;
} Bot_Channels;

typedef struct Bot_Commands {
    char *pong;
    char *version;
    char *whois;
} Bot_Commands;

typedef struct IRC_Bot {
    IRC_Client *client;
    Bot_Channels *channel_list;
    Bot_Commands *commands;
    char *nick;
} IRC_Bot;

IRC_Bot *bot_create(const char address[], const char port[],
                    const char bot_nick[]);
void bot_parse_commands(IRC_Bot *bot);

#endif // IRCBOT_BOT_BOT_H_
