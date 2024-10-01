#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>
#include "screen.h"
#include "game_error.h"

game_err_t spawn_screen(screen_state_t*);
game_err_t despawn_screen(screen_state_t*);
int get_char_index(screen_state_t*, unsigned int, unsigned int);

game_err_t
spawn_screen(screen_state_t* screen) {
    assert(screen != NULL && "Screen state must not be null at this point");

    if(screen->wnd != NULL){
        return GMERR_OK; 
    }

    screen->wnd = initscr();
    if(screen->wnd == NULL) {
        return GMERR_WINDOW_INIT_FAIL;
    }

    if(cbreak() != 0) {
        return GMERR_CBREAK_FAIL;
    }

    if(noecho() != 0) {
        return GMERR_NOECHO_FAIL;
    }

    if(keypad(screen->wnd, 1) != 0) {
        return GMERR_KEYPAD_FAIL;
    }

    if(nodelay(screen->wnd, 1) != 0) {
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

    if(wresize(screen->wnd, screen->lines, screen->cols) != OK) {
        return GMERR_RESIZE_FAIL;
    }

    return GMERR_OK;
}

game_err_t
despawn_screen(screen_state_t* screen)
{
    endwin();
    screen->wnd = NULL;
    return GMERR_OK;
}

game_err_t
init_screen(screen_state_t** ret, unsigned int lines, unsigned int cols) {
    assert(ret != NULL && "Return pointer pointer should not be initialized to NULL");
    
    screen_state_t* screen = NULL;
    screen = malloc(sizeof(screen_state_t));
    if(screen == NULL){
        return GMERR_SCREEN_ALLOC;
    }

    screen->wnd = NULL;
    screen->cols = cols;
    screen->lines = lines;
    screen->screen_chars = malloc(sizeof(char) * cols * lines + 1);
    screen->max_index = lines * cols;

    for(unsigned int i = 0; i < cols * lines; i++){
        screen->screen_chars[i] = ' ';
    }
    screen->screen_chars[cols * lines - 1] = '\0';

    if(screen->screen_chars == NULL){
        end_screen(screen);
        return GMERR_SCREEN_CHARS_ALLOC;
    }

    game_err_t err = GMERR_OK;
    if((err = spawn_screen(screen)) != GMERR_OK){
        end_screen(screen);
        return err;
    }

    (*ret) = screen;
    return GMERR_OK;
}

game_err_t
end_screen(screen_state_t* screen) {
    if(screen == NULL){
        return GMERR_OK;
    }

    game_err_t err;
    if((err = despawn_screen(screen)) != GMERR_OK){
        return err;
    }

    free(screen->screen_chars);
    free(screen);

    return GMERR_OK;
}

game_err_t
render_screen(screen_state_t* screen) {
    if(screen->needs_redraw == 0){
        return GMERR_OK;
    }

    screen->needs_redraw = 0;

    //wclear(screen->wnd);
    for(unsigned int i = 0; i < screen->max_index; i++) {
        move(i / screen->cols, i % screen->lines);
        addch(screen->screen_chars[i]);
    }
    wrefresh(screen->wnd);

    return GMERR_OK;
}

game_err_t
set_screen_char_at(screen_state_t* screen, char c, unsigned int line, unsigned int col) {
    assert(screen != NULL && "Screen state should not be null at this point");

    unsigned int index = get_char_index(screen, line, col);
    if(index == -1){
        return GMERR_SCREEN_CHAR_OUT_OF_BOUNDS;
    }

    screen->screen_chars[index] = c;
    screen->needs_redraw = 1;

    return GMERR_OK;
}

game_err_t
set_screen_chars_at(screen_state_t* screen, char* chars, unsigned int line, unsigned int col) {
    assert(screen != NULL && "Screen state should not be null at this point");

    size_t i = 0;
    unsigned int line_acc = 0;
    unsigned int col_acc = 0;
    while(chars[i] != '\0'){
        char c = chars[i]; 
        if(c == '\n') {
            line_acc++;
            col_acc = 0;
        }
        else{
            set_screen_char_at(screen, c, line + line_acc, col + col_acc);
            col_acc++;
        }
        i++;
    }
    screen->needs_redraw = 1;

    return GMERR_OK;
}

int get_char_index(screen_state_t* screen, unsigned int line, unsigned int col) {
    assert(screen != NULL && "Screen state should not be null at this point");
    if(line >= screen->lines || col >= screen->cols){
        return -1;
    }

    return line * screen->cols + col;
}

game_err_t
clear_screen(screen_state_t* screen) {
    assert(screen != NULL && "Screen state should not be null at this point");
    for(unsigned int i = 0; i < screen->max_index; i++) {
        screen->screen_chars[i] = ' ';
    }
    screen->needs_redraw = 1;

    return GMERR_OK;
}
