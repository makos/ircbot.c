/**
 * @file debug.h
 * @author Mateusz Makowski
 * @date 21.01.2020
 * @brief File containing the debug library functions.
 */
#ifndef DEBUG_H__
#define DEBUG_H__

#ifndef MAX_ARRAY_LEN
/** Maximum size of an array (in bytes). */
#define MAX_ARRAY_LEN 512
#endif

/** 
 * Log desired information into pre-set file descriptor. 
 * @param string string to be formatted and output to designated file descriptor
 * @param ... expressions to be formatted into the string, using standard
 * printf() formatting style. To be frank, the exact arguments are passed into
 * vfprintf().
 * */
void debug_log(const char *string, ...);
/** 
 * Set where debug_log will redirect the information to. 
 * @param fd_out file descriptor name as a null-terminated string. For default
 * file descriptors, use `stdout` or `stderr`. For anything else, a file will be
 * created.
 * */
void debug_set_out(const char *fd_out);
/**
 * Enable logging in a program.
 */
void debug_enable();
/**
 * Disable logging in a program.
 */
void debug_disable();
/**
 * @return 0 if logging is disabled, 1 if enabled.
 */
int debug_is_enabled();
/**
 * @return null-terminated string that was passed to debug_set_out(const char
 * *fd_out)
 */
char *debug_get_fd_str();

#endif // DEBUG_H__
