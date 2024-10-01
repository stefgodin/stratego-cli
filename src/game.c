#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "screen.h"
#include "game.h"
#include "asset.h"
#include "game_error.h"
#include "deltatime.h"

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
    unsigned long deltatime;
    long introscreen_acc;
} game_data_t;

void handle_input_events(screen_state_t*, game_data_t*);
void update_screen_size(screen_state_t*, game_data_t*);
void render_game(screen_state_t*, game_data_t*);
void render_intro_screen(screen_state_t*, game_data_t*);
void render_main_menu(screen_state_t*, game_data_t*);
void render_play(screen_state_t*, game_data_t*);
void render_borders(screen_state_t*);

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
        .deltatime = 0,
        .introscreen_acc = 0,
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

    unsigned long last_time = 0;
    unsigned long current_time = get_microtime();
    last_time = current_time;
    while(data.mode != EXIT) {
        microsleep(20000);
        current_time = get_microtime();
        data.deltatime = current_time - last_time;

        handle_input_events(screen, &data);
        render_game(screen, &data);

        last_time = current_time;
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

    render_borders(screen);

    render_screen(screen);
}

void
render_intro_screen(screen_state_t* screen, game_data_t* data) {
    asset_t* logo = &data->assets[ASSET_LOGO];
    clear_screen(screen);
    set_screen_chars_at(screen, logo->buffer, 5, (screen->cols - logo->width) / 2); 
}

void
render_main_menu(screen_state_t* screen, game_data_t* data) {

}

void
render_play(screen_state_t* screen, game_data_t* data) {

}

void
render_border(screen_state_t* screen) {
    //
    // TODO: Make terminal unicode friendly
    //
    for(size_t i = 1; i < screen->cols - 1; i++){
        set_screen_char_at(screen, '─', 0, i);
    }
    for(size_t i = 1; i < screen->lines - 1; i++){
        set_screen_char_at(screen, '│', i, 0);
    }

    set_screen_char_at(screen, '┌', 0, 0);
    set_screen_char_at(screen, '┐', 0, screen->cols - 1);
    set_screen_char_at(screen, '└', screen->lines - 1, 0);
    set_screen_char_at(screen, '┘', screen->lines - 1, screen->cols - 1);
}
