#ifndef CHATTER_H_
#define CHATTER_H_

#define MAX_ARRAY_LEN 512

typedef struct Message {
    char servername[MAX_ARRAY_LEN];
    char nickname[MAX_ARRAY_LEN];
    char user[MAX_ARRAY_LEN];
    char host[MAX_ARRAY_LEN];
    char command[MAX_ARRAY_LEN];
    char parameters[MAX_ARRAY_LEN];
} Message;

#endif // CHATTER_H_