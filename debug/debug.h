#ifndef DEBUG_H__
#define DEBUG_H__

#ifndef MAX_ARRAY_LEN
#define MAX_ARRAY_LEN 512
#endif

// Log desired information into pre-set file descriptor.
void debug_log(const char *string, ...);
// Set where debug_log will redirect the information to.
void debug_set_out(const char *fd_out);
void debug_enable();
void debug_disable();
#endif
