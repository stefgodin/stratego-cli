#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

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
} stratego_team;

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

void render_game(WINDOW* wnd, game_data_t*);
void render_intro_screen(WINDOW* wnd, game_data_t*);
void render_main_menu(WINDOW* wnd, game_data_t*);
void render_play(WINDOW* wnd, game_data_t*);

void close(void);


int 
main() {
    int err;
    WINDOW* wnd;
    game_data_t data = {
        .changed = 1,
        .mode = INTRO_SCREEN,
        .steps = 0,
        .tiles = {0}
    };

    wnd = initscr();
    if(wnd == NULL) {
        printf("ERROR: Failed to init ncurses window.\n");
        exit(1);
    }

    err = cbreak();
    err = err | noecho();
    err = err | clear() >> 1;
    err = err | refresh() << 2;
    err = err | keypad(wnd, 1) << 3;
    err = err | nodelay(wnd, 1) << 4;
    if(curs_set(0) == ERR) {
       err = err | 1 << 5; 
    }
    err = err | start_color() << 6;
    if(err != 0) {
        close();
        printf("ERROR %u: Failed to properly initialize terminal.\n", err);
        exit(1);
    }

    while(data.mode != EXIT) {
        render_game(wnd, &data);
    }

    close();
    return 0;
}

void
close() {
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
