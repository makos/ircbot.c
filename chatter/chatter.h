#ifndef CHATTER_H_
#define CHATTER_H_

#define MAX_ARRAY_LEN 512
#define PREFIX_DELIMITER_OFFSET 1

struct IRC_Bot;

typedef struct Message {
    char servername[MAX_ARRAY_LEN];
    char nickname[MAX_ARRAY_LEN];
    char user[MAX_ARRAY_LEN];
    char host[MAX_ARRAY_LEN];
    char command[MAX_ARRAY_LEN];
    char parameters[MAX_ARRAY_LEN];
    // What's left of the message after all technical stuff is stripped.
    // Usually just plain-text message from another client.
    char trailing[MAX_ARRAY_LEN];
} Message;

typedef struct ChatterStatus {
    int motd_finished;
    int connected;
} ChatterStatus;

static void parse_incoming_data(IRC_Bot *bot);
static const char *get_prefix_chunk(const char *prefix, const char *delimiter_one,
                              const char *delimiter_two);
static void activate_command(const char *command);

#endif // CHATTER_H_
