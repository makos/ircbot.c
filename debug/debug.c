#include "debug.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

static FILE *FD_OUT;
static int ENABLED = FALSE;
static char FD_STR[MAX_ARRAY_LEN] = { '\0' };

static void close_fd_out()
{
    fclose(FD_OUT);
}

static char *str_to_lower(const char *out)
{
    int out_len = strlen(out);
    char c = 'a';
    char *str = calloc((size_t)out_len + 1, sizeof(char));
    const char *sp = out;

    for (int i = 0; i < out_len; i++) {
        c = tolower(*sp);
        str[i] = c;
        sp++;
    }

    return str;
}

void debug_set_out(const char *out)
{
    if (strlen(out) > MAX_ARRAY_LEN) {
        fprintf(stderr,
                "debug_set_out: error, fd_out is too long! It needs to \
                be less than %i characters!",
                MAX_ARRAY_LEN);
        return;
    }

    atexit(close_fd_out);

    char *out_lower = str_to_lower(out);

    if (strcmp(out_lower, "stdout") == 0)
        FD_OUT = stdout;
    else if (strcmp(out_lower, "stderr") == 0)
        FD_OUT = stderr;
    else
        FD_OUT = fopen(out, "a");

    strcpy(FD_STR, out);

    free(out_lower);
}

void debug_log(const char *string, ...)
{
    if (ENABLED) {
        // Let's add some convenience and automatically do a line-break.
        int l = strlen(string);
        char *scp = calloc((size_t)l + 2, sizeof(char));
        strcpy(scp, string);
        scp[l] = '\n';

        va_list args;
        va_start(args, string);
        vfprintf(FD_OUT, scp, args);
        va_end(args);

        free(scp);
    }
}

void debug_enable()
{
    if (!ENABLED)
        ENABLED = TRUE;
}

void debug_disable()
{
    if (ENABLED)
        ENABLED = FALSE;
}

int debug_is_enabled()
{
    return ENABLED;
}

char *debug_get_fd_str()
{
    return FD_STR;
}
