#include "window.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

Window *window_create(int posy, int posx, int sizey, int sizex)
{
    /* WINDOW *temp = newwin(sizey, sizex, posy, posx); */
    Window *temp = malloc(sizeof(Window));
    temp->pointer = newwin(sizey, sizex, posy, posx);
    temp->posy = posy;
    temp->posx = posx;
    temp->sizey = sizey;
    temp->sizex = sizex;

    if (!temp->pointer) {
        fprintf(stderr, "window_create(): newwin() returned NULL\n");
        return NULL;
    }

    wrefresh(temp->pointer);
    return temp;
}
