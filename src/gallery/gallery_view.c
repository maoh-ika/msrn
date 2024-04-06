#include <gb/gb.h>
#include <stdio.h>
#include <string.h>
#include "gallery/gallery_view.h"
#include "graphics/hud_tileset.h"
#include "graphics/gallery_keyop_tilemap.h"
#include "view.h"
#include "util.h"
#include "puzzle/puzzle.h"

#define GARRALERY_IMG_HI 0
#define GARRALERY_IMG_MASHI 1
#define GARRALERY_IMG_IIKOCHAN 2
#define GARRALERY_IMG_NICE 3
#define GARRALERY_IMG_MOEGI 4

static unsigned char gCurrentImg = PUZZLE_ID_HI;
static BOOLEAN gIsUpdated;
            
const unsigned char nameTiles[20];

void initGalleryView(void) {
    HIDE_SPRITES;
    HIDE_BKG;

    set_bkg_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
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

    if (padInput & J_A || padInput & J_START) {
        waitpadup();
    } else if (padInput & J_B) {
        waitpadup();
        return VIEW_ID_TITLE;
    } else if (padInput & J_LEFT) {
        waitpadup();
        if (PUZZLE_ID_HI < gCurrentImg) {
            --gCurrentImg;
        } else {
            gCurrentImg = PUZZLE_ID_MOEGI;
        }
        gIsUpdated = TRUE;
    } else if (padInput & J_RIGHT) {
        waitpadup();
        if (gCurrentImg < PUZZLE_ID_MOEGI) {
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
    
    unsigned char tileIndices[20];
    unsigned char tileLen = 0;
    set_win_tiles(0, 0, GALLERY_KEYOP_TILEMAP_WIDTH, 1, GALLERY_KEYOP_TILEMAP);
    
    if (gCurrentImg == PUZZLE_ID_HI) {
        tileLen = 6;
        unsigned char tiles[6] = {0x27, 0x29, 0x11, 0x12, 0x29, 0x26};
        memcpy(tileIndices, tiles, tileLen);
    } else if (gCurrentImg == PUZZLE_ID_MASHI) {
        tileLen = 9;
        unsigned char tiles[9] = {0x27, 0x29, 0x16, 0x0A, 0x1B, 0x11, 0x12, 0x29, 0x26};
        memcpy(tileIndices, tiles, tileLen);
    } else if (gCurrentImg == PUZZLE_ID_IIKOCHAN) {
        tileLen = 11;
        unsigned char tiles[11] = {0x27, 0x29, 0x12, 0x12, 0x14, 0x00, 0x0C, 0x0A, 0x17, 0x29, 0x26};
        memcpy(tileIndices, tiles, tileLen);
    } else if (gCurrentImg == PUZZLE_ID_NICE) {
        tileLen = 8;
        unsigned char tiles[8] = {0x27, 0x29, 0x17, 0x12, 0x0C, 0x0E, 0x29, 0x26};
        memcpy(tileIndices, tiles, tileLen);
    } else if (gCurrentImg == PUZZLE_ID_MOEGI) {
        tileLen = 9;
        unsigned char tiles[9] = {0x27, 0x29, 0x16, 0x00, 0x0E, 0x10, 0x12, 0x29, 0x26};
        memcpy(tileIndices, tiles, tileLen);
    }

    HIDE_BKG;
    finalizePuzzle();
    setPuzzleId(gCurrentImg);
    initPuzzle(HUD_TILESET_TILE_COUNT, tileLen, tileLen, 1, 2, FALSE);
    drawPuzzle();
    SHOW_BKG;

    // turn off prev sprites
    for (int i = 0; i < 20; ++i) {
        hide_sprite(i);
    }

    unsigned char baseX = (160 - (8 * tileLen)) / 2;
    for (int i = 0; i < tileLen; ++i) {
        unsigned char tile[16];
        memcpy(tile, &HUD_TILESET[tileIndices[i] * 16], 16);
        set_sprite_data(i, 1, tile);
        set_sprite_tile(i, i);
        const unsigned int x = baseX + i * 8;
        const unsigned int y = 8;
        toScreenXY(&x, &y);
        move_sprite(i, x, y);
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