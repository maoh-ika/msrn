#include <gb/gb.h>
#include <stdio.h>
#include "stage_select/stage_select_view.h"
#include "graphics/stage_select_tilemap.h"
#include "graphics/hud_tileset.h"
#include "graphics/hud_tilemap.h"
#include "puzzle/puzzle.h"
#include "view.h"
#include "util.h"

#define CURSOR_STATE_HI 0
#define CURSOR_STATE_MASHI 1
#define CURSOR_STATE_IIKOCHAN 2
#define CURSOR_STATE_NICE 3

static unsigned char gCursorState = CURSOR_STATE_HI;
static BOOLEAN gCursorUpdated = FALSE;

void initStageSelectView(void) {
    HIDE_BKG;
    HIDE_SPRITES;
    
    set_bkg_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
    set_bkg_tiles(0, 0, STAGE_SELECT_TILEMAP_WIDTH, STAGE_SELECT_TILEMAP_HEIGHT, STAGE_SELECT_TILEMAP);
    
    set_sprite_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
    set_sprite_tile(0, 0x26);
    gCursorState = CURSOR_STATE_HI;
    gCursorUpdated = TRUE;

    SHOW_BKG;
    SHOW_SPRITES;
}

int updateStageSelectView(void) {
    unsigned char padInput = joypad();
    if (padInput & J_A || padInput & J_START) {
        waitpadup();
        return VIEW_ID_PUZZLE;
    } else if (padInput & J_UP) {
        waitpadup();
        if (gCursorState > CURSOR_STATE_HI) {
            --gCursorState;
        }
    } else if (padInput & J_DOWN) {
        waitpadup();
        if (gCursorState < CURSOR_STATE_NICE) {
            ++gCursorState;
        }
    }

    return VIEW_ID_STAGE_SELECT;
}

void drawStageSelectView(void) {
    if (gCursorUpdated) {
        int x = 48;
        int y = 48 + gCursorState * 8;
        toScreenXY(&x, &y);
        move_sprite(0, x, y);
    }
}

void finalizeStageSelectView(void) {
}