#ifndef CHATTER_WINDOW_H_
#define CHATTER_WINDOW_H_

#ifdef __linux__
typedef struct _win_st WINDOW;
#else
typedef struct _win WINDOW; // PDcurses
#endif

typedef struct Window {
    WINDOW *pointer;
    int posy, posx;
    int sizey, sizex;
} Window;

Window *window_create(int posy, int posx, int sizey, int sizex);

#endif // CHATTER_WINDOW_H_
