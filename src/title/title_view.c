#include <gb/gb.h>
#include <stdio.h>
#include "title/title_view.h"
#include "graphics/title_tileset.h"
#include "graphics/title_tilemap.h"
#include "graphics/keyop_tilemap.h"
#include "graphics/hud_tileset.h"
#include "graphics/hud_tilemap.h"
#include "view.h"
#include "util.h"

#define CURSOR_STATE_START 0
#define CURSOR_STATE_GALLERY 1
unsigned char gCursorState = CURSOR_STATE_START;
BOOLEAN gCursorUpdated = FALSE;

void cursorScreenXY(int* x, int* y) {
    if (gCursorState == CURSOR_STATE_START) {
        *x = 50;
        *y = 113;
    } else if (gCursorState == CURSOR_STATE_GALLERY) {
        *x = 50;
        *y = 123;
    }
    toScreenXY(x, y);
}

void initTitleView(void) {
    set_bkg_data(0, TITLE_TILESET_TILE_COUNT, TITLE_TILESET);
    set_bkg_tiles(0, 0, TITLE_TILEMAP_WIDTH, TITLE_TILEMAP_HEIGHT, TITLE_TILEMAP);
    
    set_sprite_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
    set_sprite_tile(0, 0x26);
    gCursorState = CURSOR_STATE_START;
    gCursorUpdated = TRUE;
}

int updateTitleView(void) {
    unsigned char padInput = joypad();
    if (padInput & J_A || padInput & J_START) {
        waitpadup();
        if (gCursorState == CURSOR_STATE_START) {
            return VIEW_ID_PUZZLE;
        }
    } else if (padInput & J_UP) {
        waitpadup();
        gCursorState = CURSOR_STATE_START;
    } else if (padInput & J_DOWN) {
        waitpadup();
        gCursorState = CURSOR_STATE_GALLERY;
    }

    return VIEW_ID_TITLE;
}

void drawTitleView(void) {
    if (gCursorUpdated) {
        int x = 0;
        int y = 0;
        cursorScreenXY(&x, &y);
        move_sprite(0, x, y);
    }
}