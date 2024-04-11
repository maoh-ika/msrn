#include <gb/gb.h>
#include <stdio.h>
#include <string.h>
#include "gallery/gallery_view.h"
#include "graphics/hud_tileset.h"
#include "graphics/gallery_keyop_tilemap.h"
#include "sound/sound.h"
#include "view.h"
#include "util.h"
#include "puzzle/puzzle.h"

static unsigned char gCurrentImg = PUZZLE_ID_HI;
static BOOLEAN gIsUpdated;
            
const unsigned char nameTiles[20];

void initGalleryView(void) {
    HIDE_SPRITES;
    HIDE_BKG;

    set_bkg_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
    
    // clear prev bkg 
    unsigned char blank[360];
    memset(blank, 0x29, 360);
    set_bkg_tiles(0, 0, 20, 18, blank);

    gCurrentImg = PUZZLE_ID_HI;
    gIsUpdated = TRUE;

    // hide window below keyop instruction
    STAT_REG = 0x40;
    LYC_REG = 8;
    add_LCD(hideWindow);
    add_VBL(showWindow);
    enable_interrupts();
    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    move_win(7, 0);

    SHOW_BKG;
    SHOW_SPRITES;
}

int updateGalleryView(void) {
    unsigned char padInput = joypad();

    if (padInput & J_B) {
        waitpadup();
        setSound(0, SOUND_TYPE_MENU_CANCEL, DEFAULT_SOUND_DURATION);
        return VIEW_ID_TITLE;
    } else if (padInput & J_LEFT) {
        waitpadup();
        setSound(0, SOUND_TYPE_MENU_SELECT, DEFAULT_SOUND_DURATION);
        if (PUZZLE_ID_HI < gCurrentImg) {
            --gCurrentImg;
        } else {
            gCurrentImg = PUZZLE_ID_LAST;
        }
        gIsUpdated = TRUE;
    } else if (padInput & J_RIGHT) {
        waitpadup();
        setSound(0, SOUND_TYPE_MENU_SELECT, DEFAULT_SOUND_DURATION);
        if (gCurrentImg < PUZZLE_ID_LAST) {
            ++gCurrentImg;
        } else {
            gCurrentImg = PUZZLE_ID_HI;
        }
        gIsUpdated = TRUE;
    }

    return VIEW_ID_GALLERY;
}

void drawGalleryView(void) {
    if (!gIsUpdated) {
        return;
    }
    
    // clear prev bkg 
    unsigned char blank[360];
    memset(blank, 0x29, 360);
    set_bkg_tiles(0, 0, 20, 18, blank);

    // clear prev sprites. img name(20) + unlock msg(15)
    for (int i = 0; i < 35; ++i) {
        hide_sprite(i);
    }
    
    unsigned char tileIndices[20];
    BOOLEAN isUnlocked = FALSE;
    
    if (gCurrentImg == PUZZLE_ID_HI) {
        unsigned char tiles[20] = { 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x27, 0x29, 0x11, 0x12, 0x29, 0x26, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29 };
        memcpy(tileIndices, tiles, 20);
        isUnlocked = gClearFlags & (1 << PUZZLE_ID_HI);
    } else if (gCurrentImg == PUZZLE_ID_MASHI) {
        unsigned char tiles[20] = { 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x27, 0x29, 0x16, 0x0A, 0x1B, 0x11, 0x12, 0x29, 0x26, 0x29, 0x29, 0x29, 0x29, 0x29 };
        memcpy(tileIndices, tiles, 20);
        isUnlocked = gClearFlags & (1 << PUZZLE_ID_MASHI);
    } else if (gCurrentImg == PUZZLE_ID_IIKOCHAN) {
        unsigned char tiles[20] = { 0x29, 0x29, 0x29, 0x29, 0x27, 0x29, 0x12, 0x12, 0x14, 0x00, 0x0C, 0x11, 0x0A, 0x17, 0x29, 0x26, 0x29, 0x29, 0x29, 0x29 };
        memcpy(tileIndices, tiles, 20);
        isUnlocked = gClearFlags & (1 << PUZZLE_ID_IIKOCHAN);
    } else if (gCurrentImg == PUZZLE_ID_NICE) {
        unsigned char tiles[20] = { 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x27, 0x29, 0x17, 0x12, 0x0C, 0x0E, 0x29, 0x26, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29 };
        memcpy(tileIndices, tiles, 20);
        isUnlocked = gClearFlags & (1 << PUZZLE_ID_NICE);
    } else if (gCurrentImg == PUZZLE_ID_MOEGI) {
        unsigned char tiles[20] = { 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x27, 0x29, 0x16, 0x00, 0x0E, 0x10, 0x12, 0x29, 0x26, 0x29, 0x29, 0x29, 0x29, 0x29};
        memcpy(tileIndices, tiles, 20);
        isUnlocked = gClearFlags & (1 << PUZZLE_ID_MOEGI);
    } else if (gCurrentImg == PUZZLE_ID_PUIPUI) {
        unsigned char tiles[20] = { 0x29, 0x29, 0x29, 0x29, 0x29, 0x27, 0x29, 0x18, 0x1D, 0x12, 0x18, 0x1D, 0x12, 0x29, 0x26, 0x29, 0x29, 0x29, 0x29, 0x29};
        memcpy(tileIndices, tiles, 20);
        isUnlocked = gClearFlags & (1 << PUZZLE_ID_PUIPUI);
    } else if (gCurrentImg == PUZZLE_ID_KYURUN) {
        unsigned char tiles[20] = { 0x29, 0x29, 0x29, 0x29, 0x29, 0x27, 0x29, 0x14, 0x21, 0x1D, 0x1A, 0x1D, 0x17, 0x29, 0x26, 0x29, 0x29, 0x29, 0x29, 0x29};
        memcpy(tileIndices, tiles, 20);
        isUnlocked = gClearFlags & (1 << PUZZLE_ID_KYURUN);
    } else if (gCurrentImg == PUZZLE_ID_NYAN) {
        unsigned char tiles[20] = { 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x27, 0x29, 0x17, 0x21, 0x0A, 0x17, 0x29, 0x26, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29};
        memcpy(tileIndices, tiles, 20);
        isUnlocked = gClearFlags & (1 << PUZZLE_ID_NYAN);
    }

    set_win_tiles(0, 0, 20, 1, tileIndices);

    // draw image or unlock message
    if (isUnlocked) {
        HIDE_BKG;
        finalizePuzzle();
        setPuzzleId(gCurrentImg);
        initPuzzle(HUD_TILESET_TILE_COUNT, 20, 20, 1, 2, FALSE);
        drawPuzzle();
        SHOW_BKG;
    } else {
        HIDE_BKG;
        unsigned char msgTiles[15] = {
            // Play to unlock!
            0x18, 0x15, 0x0A, 0x21, 0x29, 0x1C, 0x00, 0x29, 0x1D, 0x17, 0x15, 0x00, 0x0C, 0x14, 0x28
        };
        for (int i = 0; i < 15; ++i) {
            unsigned char tile[16];
            memcpy(tile, &HUD_TILESET[msgTiles[i] * 16], 16);
            set_bkg_data(i + HUD_TILESET_TILE_COUNT, 1, tile);
            set_tile_xy(3 + i, 3, i + HUD_TILESET_TILE_COUNT);
        }
        SHOW_BKG;
    } 

    gIsUpdated = FALSE;
}

void finalizeGalleryView(void) {
    finalizePuzzle();
    for (int i = 0; i < 20; ++i) {
        hide_sprite(i);
    }
    remove_LCD(hideWindow);
    remove_VBL(showWindow);
    HIDE_WIN;
}