/**
 * @file minunit.h
 * @author Mateusz Makowski
 * @date 14.09.2018
 * @brief Custom minimal unit testing header file. Inspired by
 * http://www.jera.com/techinfo/jtns/jtn002.html
 */
#ifndef IRCBOT_TESTS_MINUNIT_H_
#define IRCBOT_TESTS_MINUNIT_H_

/**
 * Assert truth or return an error message.
 * @param message string to be returned on failure
 * @param test an expression that evaluates to 0 (false) or any other number
 * (true)
 */
#define mu_assert(message, test)                                               \
    do {                                                                       \
        if (!(test))                                                           \
            return message;                                                    \
    } while (0)
/**
 * Same as mu_assert, but disconnects the bot before returning on failure.
 * @param bot pointer to IRC_Bot
 * @param message string to be returned on failure
 * @param test an expression that evaluates to 0 (false) or any other number
 * (true)
 */
#define mu_assert_or_disconnect(bot, message, test)                            \
    do {                                                                       \
        if (!(test)) {                                                         \
            bot_disconnect(bot);                                               \
            return message;                                                    \
        }                                                                      \
    } while (0)

/**
 * Execute a test case.
 * @param name string containing the test name, can be anything
 * @param test function pointer to the test case
 */
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

/** Total number of tests executed. */
extern int tests_run;
/** Number of tests that failed. */
extern int tests_failed;
/** List of messages from failed tests, for printing. */
extern char **messages;
/**
 * @name ANSI color codes
 * Color codes or empty strings, determined by `isatty()` in `main()`.
 */
//@{
extern char RED[];
extern char GREEN[];
extern char YELLOW[];
extern char COLOR_END[];
//}@

#endif // IRCBOT_TESTS_MINUNIT_H_
