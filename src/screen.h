#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include "game_error.h"

typedef struct screen_state {
    WINDOW* wnd;
    unsigned int cols;
    unsigned int lines;
    char* screen_chars;
    int needs_redraw;
} screen_state_t;

game_err_t init_screen(screen_state_t** ret, unsigned int lines, unsigned int cols);
game_err_t end_screen(screen_state_t* screen);
game_err_t render_screen(screen_state_t* screen);
game_err_t set_screen_char_at(screen_state_t* screen, char c, unsigned int line, unsigned int col);

#endif /* SCREEN_H */
