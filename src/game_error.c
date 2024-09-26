#include "game_error.h"

const char* game_err_msg(game_err_t code) {
    switch(code) {
        case GMERR_NODELAY_FAIL:
            return "Failed to make character input non-blocking";
        case GMERR_ASSET_NOT_FOUND:
            return "No filename exists for given asset";
        case GMERR_RESIZE_FAIL:
            return "Failed to resize window";
        case GMERR_SCREEN_ALLOC:
            return "Failed to allocate screen state memory";
        case GMERR_SCREEN_CHARS_ALLOC:
            return "Failed to allocate screen state chars memory";
        case GMERR_SCREEN_CHAR_OUT_OF_BOUNDS:
            return "Attempted to set character outside of screen bounds.";
        case GMERR_WINDOW_INIT_FAIL:
            return "Failed to initialize ncurses window";
        case GMERR_CBREAK_FAIL:
            return "Failed to enable instant character typing";
        case GMERR_NOECHO_FAIL:
            return "Failed to disable echoing of typed characters";
        case GMERR_KEYPAD_FAIL:
            return "Failed to enable keypad function keys";
        case GMERR_REFRESH_FAIL:
            return "Failed to refresh terminal";
        case GMERR_CLEAR_FAIL:
            return "Failed to clear terminal";
        case GMERR_COLOR_SET_FAIL:
            return "Failed to enable terminal color";
        case GMERR_CURSOR_INVISIBLE_FAIL:
            return "Failed to render cursor invisible";
        case GMERR_ASSET_OPEN_FAIL:
            return "Failed to open asset file";
        case GMERR_ASSET_SEEK_FAIL:
            return "Failed to seek end of asset file";
        case GMERR_ASSET_LEN_FAIL:
            return "Failed to retrieve asset file content length";
        case GMERR_ASSET_ALLOC_FAIL:
            return "Failed to allocate asset file content buffer";
        case GMERR_ASSET_READ_FAIL:
            return "Failed to read asset file content to buffer";
        case GMERR_ASSET_CLOSE_FAIL:
            return "Failed to close asset file";
        case GMERR_ASSET_LIST_FAIL:
            return "Failed to allocate game assets array";
        default:
            return "Unknown error";
    }
}
