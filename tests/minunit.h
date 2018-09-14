#ifndef IRCBOT_TESTS_MINUNIT_H_
#define IRCBOT_TESTS_MINUNIT_H_

#define mu_assert(message, test)                                               \
    do {                                                                       \
        if (!(test))                                                           \
            return message;                                                    \
    } while (0)
#define mu_run_test(test)                                                      \
    do {                                                                       \
        char *message = test();                                                \
        tests_run++;                                                           \
        if (message)                                                           \
            return message;                                                    \
    } while (0)
extern int tests_run;

#endif // IRCBOT_TESTS_MINUNIT_H_
