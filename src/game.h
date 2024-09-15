#ifndef STRATEGO_GAME_H
#define STRATEGO_GAME_H

#include <ncurses.h>
#include "game_error.h"


game_err_t init(WINDOW** ret);
game_err_t run(WINDOW*);
void close(void);

#endif /* STRATEGO_GAME_H */
