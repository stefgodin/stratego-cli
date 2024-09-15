#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "game.h"
#include "game_error.h"

int 
main(void) {
    WINDOW* wnd = NULL;
    game_err_t err = init(&wnd);
    if(err != GMERR_OK) {
        close();
        printf("Error %d: %s\n", err, game_err_msg(err));
        return 1;
    }

    assert(wnd != NULL && "Initialization did not fail but returned an empty window pointer");

    err = run(wnd);
    if(err != GMERR_OK){
        close();
        printf("Error %d: %s\n", err, game_err_msg(err));
        return 1;
    }

    close();
    return 0;
}

