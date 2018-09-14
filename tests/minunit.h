#ifndef IRCBOT_TESTS_MINUNIT_H_
#define IRCBOT_TESTS_MINUNIT_H_

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define COLOR_END "\033[0m"

#define mu_assert(message, test)                                               \
    do {                                                                       \
        if (!(test))                                                           \
            return message;                                                    \
    } while (0)
#define mu_run_test(name, test)                                                \
    do {                                                                       \
        printf(YELLOW "TEST: %s\n" COLOR_END, name);                           \
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

#endif // IRCBOT_TESTS_MINUNIT_H_
