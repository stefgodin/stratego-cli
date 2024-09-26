#ifndef STRATEGO_GAME_H
#define STRATEGO_GAME_H

#include "screen.h"
#include "game_error.h"

game_err_t init_game(screen_state_t** ret);
game_err_t run_game(screen_state_t* screen);
void close_game(screen_state_t* screen);

#endif /* STRATEGO_GAME_H */
