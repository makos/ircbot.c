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
 * Set where debug_log will redirect the information to. Use "stdout" and
 * "stderr" for standard output and error. Anything else will be treated as a
 * file and appended to, or created if necessary.
 * @param fd_out file descriptor name as a null-terminated string.
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
 * Check if logging is enabled or not.
 * @return 0 if logging is disabled, 1 if enabled.
 */
_Bool debug_is_enabled();
/**
 * Get null-terminated string describing the file descriptor in use.
 * @return null-terminated string that was passed to debug_set_out()
 */
char *debug_get_fd_str();

#endif // DEBUG_H__
