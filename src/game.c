#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <assert.h>

typedef enum piece_types {
    SPY = 0x001,
    SCOUT = 0x002,
    MINER = 0x004,
    SERGEANT = 0x008,
    LIEUTENANT = 0x010,
    CAPTAIN = 0x020,
    MAJOR = 0x040,
    COLONEL = 0x080,
    GENERAL = 0x100,
    MARSHAL = 0x200,
    FLAG = 0x400,
    BOMB = 0x800
} piece_types_t;

typedef enum team {
    BLUE = 0x1000,
    RED = 0x2000
} stratego_team_t;

typedef enum game_mode {
    INTRO_SCREEN,
    MENU,
    PLAY,
    EXIT,
} game_mode_t;

typedef struct game_data {
    int changed;
    game_mode_t mode;
    uint steps;
    int tiles[100];
} game_data_t;

void render_game(WINDOW*, game_data_t*);
void render_intro_screen(WINDOW*, game_data_t*);
void render_main_menu(WINDOW*, game_data_t*);
void render_play(WINDOW*, game_data_t*);

init_result_t
init(void) {
    init_result_t res = {
        .wnd = NULL,
        .err = 0
    };
    if(res.wnd == NULL) { 
        res.wnd = initscr();
        if(res.wnd == NULL) {
            res.err = 1;
            return res;
        }
    }

    if(cbreak() != 0) {
        res.err = 2;
        return res;
    }

    if(noecho() != 0) {
        res.err = 3;
        return res;
    }

    if(keypad(res.wnd, 1) != 0) {
        res.err = 4;
        return res;
    }

    if(nodelay(res.wnd, 1) != 0) {
        res.err = 5;
        return res;
    }

    if(curs_set(0) == ERR) {
        res.err = 6;
        return res;
    }

    if(start_color()) {
        res.err = 7;
        return res;
    }

    if(clear() != 0) {
        res.err = 8;
        return res;
    }

    if(refresh() != 0) {
        res.err = 9;
        return res;
    }

    return res;
}

void
run(WINDOW* wnd) {
    game_data_t data = {
        .changed = 1,
        .mode = INTRO_SCREEN,
        .steps = 0,
        .tiles = {0}
    };

    while(data.mode != EXIT) {
        render_game(wnd, &data);
    }
}

void
close(void) {
    endwin();
}

/* RENDERING STUFF */

void
render_game(WINDOW* wnd, game_data_t* data) {
    if(data->changed == 0) {
        return;
    }
    data->changed = 0;

    clear();

    attron(A_BOLD);
    box(wnd, 0, 0);
    attroff(A_BOLD);

    switch(data->mode) {
        case INTRO_SCREEN:
            render_intro_screen(wnd, data);
            break;
        case MENU:
            render_main_menu(wnd, data);
            break;
        case PLAY:
            render_play(wnd, data);
    }

    refresh();
}

void
render_intro_screen(WINDOW* wnd, game_data_t* data) {
    
}

void
render_main_menu(WINDOW* wnd, game_data_t* data) {

}

void
render_play(WINDOW* wnd, game_data_t* data) {

}
