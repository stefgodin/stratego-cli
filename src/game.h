#include <ncurses.h>

#ifndef STRATEGO_GAME_H
#define STRATEGO_GAME_H

typedef struct init_result {
    WINDOW* wnd;
    int err;
} init_result_t;

init_result_t init(void);
void run(WINDOW*);
void close(void);

#endif /* STRATEGO_GAME_H */
