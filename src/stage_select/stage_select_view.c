#include <gb/gb.h>
#include <stdio.h>
#include "stage_select/stage_select_view.h"
#include "graphics/stage_select_tilemap.h"
#include "graphics/hud_tileset.h"
#include "graphics/hud_tilemap.h"
#include "sound/sound.h"
#include "puzzle/puzzle.h"
#include "view.h"
#include "util.h"

#define CURSOR_STATE_HI 0
#define CURSOR_STATE_MASHI 1
#define CURSOR_STATE_IIKOCHAN 2
#define CURSOR_STATE_NICE 3

static unsigned char gCursorState = PUZZLE_ID_HI;
static BOOLEAN gCursorUpdated = FALSE;

void initStageSelectView(void) {
    HIDE_BKG;
    HIDE_SPRITES;
    
    set_bkg_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
    set_bkg_tiles(0, 0, STAGE_SELECT_TILEMAP_WIDTH, STAGE_SELECT_TILEMAP_HEIGHT, STAGE_SELECT_TILEMAP);
    
    if (isMoegiEnabled()) {
        unsigned char moegiTiles[5] = { 0x16, 0x00, 0x0E, 0x10, 0x12 };
        for (int i = 0; i < 5; ++i) {
            set_tile_xy(7 + i, 10, moegiTiles[i]);
        }
    }
     
    set_sprite_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
    set_sprite_tile(0, 0x26);
    gCursorState = PUZZLE_ID_HI;
    gCursorUpdated = TRUE;

    SHOW_BKG;
    SHOW_SPRITES;
}

int updateStageSelectView(void) {
    unsigned char padInput = joypad();
    if (padInput & J_A || padInput & J_START) {
        waitpadup();
        setSound(0, SOUND_TYPE_MENU_SELECT, DEFAULT_SOUND_DURATION);
        setPuzzleId(gCursorState);
        return VIEW_ID_PUZZLE;
    } else if (padInput & J_B) {
        waitpadup();
        setSound(0, SOUND_TYPE_MENU_CANCEL, DEFAULT_SOUND_DURATION);
        return VIEW_ID_TITLE;
    } else if (padInput & J_UP) {
        if (gCursorState > PUZZLE_ID_HI) {
            waitpadup();
            setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
            --gCursorState;
        }
    } else if (padInput & J_DOWN) {
        if (isMoegiEnabled() && gCursorState < PUZZLE_ID_MOEGI) {
            waitpadup();
            setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
            ++gCursorState;
        } else if (gCursorState < PUZZLE_ID_NICE) {
            waitpadup();
            setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
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