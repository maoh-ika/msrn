#include <gb/gb.h>
#include <gbdk/platform.h>
#include <stdio.h>
#include "title/title_view.h"
#include "graphics/title_tileset.h"
#include "graphics/title_tilemap.h"
#include "graphics/puzzle_keyop_tilemap.h"
#include "graphics/hud_tileset.h"
#include "graphics/hud_tilemap.h"
#include "sound/sound.h"
#include "view.h"
#include "util.h"

#define CURSOR_STATE_START 0
#define CURSOR_STATE_GALLERY 1
static unsigned char gCursorState = CURSOR_STATE_START;
static BOOLEAN gCursorUpdated = FALSE;

void initTitleView(void) {
    HIDE_BKG;
    HIDE_SPRITES;

    SWITCH_ROM(BANK(TITLE_TILESET));
    set_bkg_data(0, TITLE_TILESET_TILE_COUNT, TITLE_TILESET);
    set_bkg_tiles(0, 0, TITLE_TILEMAP_WIDTH, TITLE_TILEMAP_HEIGHT, TITLE_TILEMAP);

    // cursor    
    set_sprite_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
    set_sprite_tile(0, 0x26);
    gCursorState = CURSOR_STATE_START;
    gCursorUpdated = TRUE;

    // version
    const unsigned char version[6] = {0x1E, 0x01, 0x2C, 0x01, 0x2C, 0x00};
    const int baseX = 120;
    for (int i = 0; i < 6; ++i) {
        set_sprite_tile(i + 1, version[i]);
        move_sprite(i + 1, baseX + 8 * i, 151);
    }

    SHOW_BKG; 
    SHOW_SPRITES;
}

int updateTitleView(void) {
    unsigned char padInput = joypad();
    if (padInput & J_A || padInput & J_START) {
        waitpadup();
        setSound(0, SOUND_TYPE_MENU_SELECT, DEFAULT_SOUND_DURATION);
        if (gCursorState == CURSOR_STATE_START) {
            return VIEW_ID_STAGE_SELECT;
        } else if (gCursorState == CURSOR_STATE_GALLERY) {
            return VIEW_ID_GALLERY;
        }
    } else if (padInput & J_UP) {
        if (CURSOR_STATE_START < gCursorState) {
            waitpadup();
            setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
            --gCursorState;
        }
    } else if (padInput & J_DOWN) {
        if (gCursorState < CURSOR_STATE_GALLERY) {
            waitpadup();
            setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
            ++gCursorState;
        }
    }

    return VIEW_ID_TITLE;
}

void drawTitleView(void) {
    if (gCursorUpdated) {
        int x = 50;
        int y = 113 + 12 * gCursorState;
        toScreenXY(&x, &y);
        move_sprite(0, x, y);
    }
}

void finalizeTitleView(void) {
    for (int i = 0; i < 7; ++i) {
        hide_sprite(i);
    }
}