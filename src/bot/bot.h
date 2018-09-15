#ifndef IRCBOT_BOT_BOT_H_
#define IRCBOT_BOT_BOT_H_

typedef int (*fp_cmd_t)();
struct IRC_Connection;

#define BOT_MAX_COMMANDS 128

typedef struct Bot_Command {
    char *name;
    fp_cmd_t callback;
} Bot_Command;

typedef struct IRC_Bot {
    char *nick;
    struct IRC_Connection *connection;
    Bot_Command *commands[BOT_MAX_COMMANDS];
    int newest_cmd;
} IRC_Bot;

IRC_Bot *bot_create(const char nick[]);
int bot_add_command(IRC_Bot *bot, const char name[], fp_cmd_t callback);
int bot_connect(IRC_Bot *bot, const char address[], const char port[]);

#endif // IRCBOT_BOT_BOT_H_
