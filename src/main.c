#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int 
main(void) {
    init_result_t init_result;

    init_result = init();
    if(init_result.err != 0) {
        close();
        printf("ERROR %u: Failed to properly initialize terminal.\n", init_result.err);
        return 1;
    }

    assert(init_result.wnd != NULL && "Initialization did not fail but returned an empty window pointer");
    run(init_result.wnd);

    close();
    return 0;
}

