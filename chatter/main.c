#include "bot.h"
#include "window.h"
#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

static void start_ui()
{
    setlocale(LC_ALL, "");
    initscr();
    /* halfdelay(25); */
    cbreak();
    noecho();
    clear();
    refresh();
}

static void end_ui()
{
    endwin();
}

static void pong(IRC_Bot *bot)
{
    char *ping_id = strtok(bot->last_msg, ":");
    ping_id = strtok(NULL, ":");
    char *pong_msg = malloc(32 * sizeof(char));
    strcpy(pong_msg, "PONG ");
    strcat(pong_msg, ping_id);
    bot_send(bot, pong_msg);
}

int main()
{
    start_ui();
    IRC_Bot *chatter = bot_create("chatterbot");
    bot_connect(chatter, "irc.rizon.net", "6660");

    bot_send(chatter, "USER chatterbot chatterbot chatterbot chatterbot");
    bot_send(chatter, "NICK chatterbot");

    // out_win spans almost the whole screen, in_win occupies the rest -
    // the very last row at the bottom.
    Window *out_win = window_create(0, 0, LINES - 1, COLS);
    Window *in_win = window_create(LINES - 1, 0, 1, COLS);
    wrefresh(out_win->pointer);
    wrefresh(in_win->pointer);
    wtimeout(in_win->pointer, 100);
    /* /\* nodelay(in_win->pointer, 0); *\/ */
    clear();
    refresh();
    bot_join(chatter, "#ircbot_ctest");

    char ch = 0;
    int bytes;
    while (ch != 'q') {
        ch = 0;
        werase(out_win->pointer);
        ch = wgetch(in_win->pointer);
        /* wscanw(in_win->pointer, "%s"); */

        /* erase(); */

        /* if (strncmp(ch, "join", 4)) { */
        /*     bot_join(chatter, "#ircbot_ctest"); */
        /* } */

        bytes = bot_read(chatter);

        if (strncmp(chatter->last_msg, "PING", 4) == 0) {
            pong(chatter);
        }

        /* refresh(); */
        wrefresh(out_win->pointer);
        wrefresh(in_win->pointer);
        /* wclear(out_win->pointer); */
        /* wclear(in_win->pointer); */
    }

    bot_disconnect(chatter);

    end_ui();
    return 0;
}
