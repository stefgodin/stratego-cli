#include <stdio.h>
#include <stdlib.h>

#include "asset.h"
#include "game_error.h"

const char* get_asset_filename(asset_id_t id) {
    switch(id) {
        case ASSET_LOGO:
            return "assets/logo.txt";
        default:
            return NULL;
    }
}

game_err_t 
load_asset(asset_t* ret, asset_id_t id) {
    const char* filename = NULL;
    FILE* file = NULL;
    long length = 0;
    asset_t asset = {
        .id = id,
        .buffer = NULL
    };


    filename = get_asset_filename(id);
    if(filename == NULL) {
        return GMERR_ASSET_NOT_FOUND;
    }

    file = fopen(filename, "rb");
    if(file == NULL) {
        return GMERR_ASSET_OPEN_FAIL;
    }

    if(fseek(file, 0, SEEK_END) != 0) {
        return GMERR_ASSET_SEEK_FAIL;
    }

    length = ftell(file);
    if(length == -1L) {
        return GMERR_ASSET_LEN_FAIL;
    }

    if(fseek(file, 0, SEEK_SET) != 0) {
        return GMERR_ASSET_SEEK_FAIL;
    }

    asset.buffer = malloc(length + 1);
    if(asset.buffer == NULL) {
        return GMERR_ASSET_ALLOC_FAIL;
    }
    
    if(asset.buffer) {
        fread(asset.buffer, 1, length, file);
        if(ferror(file)) {
            unload_asset(&asset);
            return GMERR_ASSET_READ_FAIL;
        }
        asset.buffer[length] = '\0';
    }

    if(fclose(file) == EOF) {
        unload_asset(&asset);
        return GMERR_ASSET_CLOSE_FAIL;
    }
    
    *ret = asset;
    return GMERR_OK;
}

void
unload_asset(asset_t* asset) {
    free(asset->buffer);
    asset->buffer = NULL;
}
