#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "game.h"
#include "game_error.h"

int 
main(void) {
    screen_state_t* screen = NULL;
    game_err_t err = init_game(&screen);
    if(err != GMERR_OK) {
        close_game(screen);
        printf("Error %d: %s\n", err, game_err_msg(err));
        return 1;
    }

    assert(screen != NULL && "Initialization did not fail but returned an empty screen state pointer");

    err = run_game(screen);
    if(err != GMERR_OK){
        close_game(screen);
        printf("Error %d: %s\n", err, game_err_msg(err));
        return 1;
    }

    close_game(screen);
    return 0;
}

