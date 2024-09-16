#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <assert.h>

#include "game.h"
#include "asset.h"
#include "game_error.h"

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
    asset_t* assets;
} game_data_t;

void render_game(WINDOW*, game_data_t*);
void render_intro_screen(WINDOW*, game_data_t*);
void render_main_menu(WINDOW*, game_data_t*);
void render_play(WINDOW*, game_data_t*);

game_err_t
init(WINDOW** ret) {
    WINDOW* wnd = initscr();
    if(wnd == NULL) {
        return GMERR_WINDOW_INIT_FAIL;
    }

    if(cbreak() != 0) {
        return GMERR_CBREAK_FAIL;
    }

    if(noecho() != 0) {
        return GMERR_NOECHO_FAIL;
    }

    if(keypad(wnd, 1) != 0) {
        return GMERR_KEYPAD_FAIL;
    }

    if(nodelay(wnd, 1) != 0) {
        return GMERR_NODELAY_FAIL;
    }

    if(curs_set(0) == ERR) {
        return GMERR_CURSOR_INVISIBLE_FAIL;
    }

    if(start_color()) {
        return GMERR_COLOR_SET_FAIL;
    }

    if(clear() != 0) {
        return GMERR_CLEAR_FAIL;
    }

    if(refresh() != 0) {
        return GMERR_REFRESH_FAIL;
    }

    (*ret) = wnd;
    return GMERR_OK;
}

game_err_t
run(WINDOW* wnd) {
    game_err_t err = GMERR_OK;
    game_data_t data = {
        .changed = 1,
        .mode = INTRO_SCREEN,
        .steps = 0,
        .tiles = {0},
        .assets = NULL
    };

    data.assets = malloc(ASSET_COUNT * sizeof(asset_t));
    if(data.assets == NULL) {
        return GMERR_ASSET_LIST_FAIL;
    }

    for(int i = ASSET_UNKNOWN + 1; i < ASSET_COUNT; i++) {
        asset_t asset;
        err = load_asset(&asset, i);
        if(err != GMERR_OK){
            return err;
        }
        assert(asset.id == i && asset.buffer != NULL);
        data.assets[i] = asset;
    } 

    while(data.mode != EXIT) {
        render_game(wnd, &data);
    }

    if(data.assets != NULL) {
        for(size_t i = ASSET_UNKNOWN + 1; i < ASSET_COUNT; i++) {
            unload_asset(&data.assets[i]);
        } 
        free(data.assets);
        data.assets = NULL;
    }

    return err;
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
            break;
        case EXIT:
            break;
    }

    refresh();
}

void
render_intro_screen(WINDOW* wnd, game_data_t* data) {
    move(10, 5);
    int line = 0;
    asset_t* logo = &data->assets[ASSET_LOGO];
    for(size_t i = 0; i < logo->length; i++) {
        char c = logo->buffer[i]; 
        if(c == '\n') {
            line++;
            move(10 + line, 5);
        }
        else if(c != '\0') {
            addch(c);
        }
    }
}

void
render_main_menu(WINDOW* wnd, game_data_t* data) {

}

void
render_play(WINDOW* wnd, game_data_t* data) {

}
