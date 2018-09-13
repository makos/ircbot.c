#ifndef IRCBOT_BOT_BOT_H_
#define IRCBOT_BOT_BOT_H_
typedef struct IRC_Client IRC_Client;

typedef struct Bot_Channel {
    Bot_Channels *prev;
    char *channel_name;
    Bot_Channels *next;
} Bot_Channels;

typedef struct Bot_Commands {
    const char *pong;
    const char *version;
    const char *whois;
} Bot_Commands;

typedef struct IRC_Bot {
    IRC_Client *client;
    Bot_Channels *channel_list;
    Bot_Commands *commands;
    char *nick;
} IRC_Bot;

IRC_Bot *bot_create(const char server[], const char port[],
                    const char bot_nick[]);
void bot_parse_commands(IRC_Bot *bot);

#endif // IRCBOT_BOT_BOT_H_
