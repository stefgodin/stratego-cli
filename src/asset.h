#include "game_error.h"

#ifndef ASSET_H
#define ASSET_H

typedef enum asset_id {
    ASSET_UNKNOWN = -1,
    ASSET_LOGO,
    ASSET_COUNT
} asset_id_t;

typedef struct asset {
    asset_id_t id;
    char* buffer;
    size_t length;
} asset_t;

game_err_t load_asset(asset_t* ret, asset_id_t id);
void unload_asset(asset_t* asset);

#endif /* ASSET_H */
