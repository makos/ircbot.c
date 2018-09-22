#include "bot.h"
#include <curses.h>

int main()
{
    initscr();
    clear();
    mvprintw(5, 5, "Hello");
    refresh();
    char ch = getch();

    endwin();
    return 0;
}
