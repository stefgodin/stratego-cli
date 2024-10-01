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
    size_t length = 0;
    asset_t asset = {
        .id = id,
        .buffer = NULL,
        .length = 0,
        .width = 0,
        .height = 0
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

    asset.length = length + 1;
    asset.buffer = malloc(asset.length);
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
        
        size_t width = 0;
        for(size_t i = 0; i < length; i++){
            if(asset.buffer[i] == '\n'){
                asset.height++;
                if(asset.width < width){
                    asset.width = width;
                }
                width = 0;
            }
            else{
                width++;
            }
        }
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
    asset->length = 0;
    asset->width = 0;
    asset->height = 0;
}
