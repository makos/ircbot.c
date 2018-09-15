#ifndef IRCBOT_TESTS_MINUNIT_H_
#define IRCBOT_TESTS_MINUNIT_H_

#define mu_assert(message, test)                                               \
    do {                                                                       \
        if (!(test))                                                           \
            return message;                                                    \
    } while (0)
#define mu_run_test(name, test)                                                \
    do {                                                                       \
        printf("%sTEST: %s%s\n", YELLOW, name, COLOR_END);                     \
        char *message = test();                                                \
        tests_run++;                                                           \
        if (message) {                                                         \
            strcpy(messages[tests_failed], message);                           \
            tests_failed++;                                                    \
        }                                                                      \
    } while (0)

extern int tests_run;
extern int tests_failed;
extern char **messages;
extern char RED[];
extern char GREEN[];
extern char YELLOW[];
extern char COLOR_END[];

#endif // IRCBOT_TESTS_MINUNIT_H_
