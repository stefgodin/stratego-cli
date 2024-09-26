#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "screen.h"
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
    unsigned int steps;
    int tiles[100];
    asset_t* assets;
    int screen_x;
    int screen_y;
} game_data_t;

void handle_input_events(screen_state_t*, game_data_t*);
void update_screen_size(screen_state_t*, game_data_t*);
void render_game(screen_state_t*, game_data_t*);
void render_intro_screen(screen_state_t*, game_data_t*);
void render_main_menu(screen_state_t*, game_data_t*);
void render_play(screen_state_t*, game_data_t*);

game_err_t
init_game(screen_state_t** ret)
{
    if(ret == NULL){
        return GMERR_OK;
    }
    
    return init_screen(ret, 120, 120);
}

game_err_t
run_game(screen_state_t* screen) {
    game_err_t err = GMERR_OK;
    game_data_t data = {
        .changed = 1,
        .mode = INTRO_SCREEN,
        .steps = 0,
        .tiles = {0},
        .assets = NULL,
        .screen_x = 0,
        .screen_y = 0,
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
        handle_input_events(screen, &data);
        render_game(screen, &data);
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
close_game(screen_state_t* screen) {
    end_screen(screen);
}

/* EVENT STUFF */
void
handle_input_events(screen_state_t* screen, game_data_t* data) {
}

/* RENDERING STUFF */

void
render_game(screen_state_t* screen, game_data_t* data) {
    if(data->changed == 0) {
        return;
    }
    data->changed = 0;

    switch(data->mode) {
        case INTRO_SCREEN:
            render_intro_screen(screen, data);
            break;
        case MENU:
            render_main_menu(screen, data);
            break;
        case PLAY:
            render_play(screen, data);
            break;
        case EXIT:
            break;
    }

    render_screen(screen);
}

void
render_intro_screen(screen_state_t* screen, game_data_t* data) {
    unsigned int col_offset = 5;
    unsigned int line_offset = 10;
    unsigned int line = 0;
    unsigned int col = 0;
    asset_t* logo = &data->assets[ASSET_LOGO];
    for(size_t i = 0; i < logo->length; i++) {
        char c = logo->buffer[i]; 
        if(c == '\n') {
            line++;
            col = 0;
        }
        else if(c != '\0') {
            set_screen_char_at(screen, c, line + line_offset, col + col_offset);
            col++;
        }
    }
}

void
render_main_menu(screen_state_t* screen, game_data_t* data) {

}

void
render_play(screen_state_t* screen, game_data_t* data) {

}
