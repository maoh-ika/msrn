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

#define GALLERY_VIEW_STATE_IMG 0
#define GALLERY_VIEW_STATE_QR 1

static unsigned char gCurrentImg = GARRALERY_IMG_HI;
static unsigned char gViewState = GALLERY_VIEW_STATE_IMG;
static BOOLEAN gIsUpdated;
            
const unsigned char nameTiles[20];

void initGalleryView(void) {
    HIDE_SPRITES;
    HIDE_BKG;

    set_bkg_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);
    unsigned char blank[360];
    memset(blank, 0x29, 360);
    set_bkg_tiles(0, 0, 20, 18, blank);

    gViewState = GALLERY_VIEW_STATE_IMG;
    gCurrentImg = GARRALERY_IMG_HI;
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
    } else if (padInput & J_UP) {
        waitpadup();
    } else if (padInput & J_DOWN) {
        waitpadup();
    } else if (padInput & J_LEFT) {
        waitpadup();
    } else if (padInput & J_RIGHT) {
        waitpadup();
    }

    return VIEW_ID_GALLERY;
}

void drawGalleryView(void) {
    if (!gIsUpdated) {
        return;
    }
    
    unsigned char tileIndices[20];
    unsigned char tileLen = 0;
    if (gViewState == GALLERY_VIEW_STATE_IMG) {
        set_win_tiles(0, 0, GALLERY_KEYOP_TILEMAP_WIDTH, 1, &GALLERY_KEYOP_TILEMAP[gViewState * 16]);
        if (gCurrentImg == GARRALERY_IMG_HI) {
            tileLen = 8;
            unsigned char tiles[8] = {0x27, 0x29, 0x16, 0x1B, 0x1A, 0x17, 0x29, 0x26};
            memcpy(tileIndices, tiles, tileLen);

            initPuzzle(HUD_TILESET_TILE_COUNT, tileLen, tileLen, 1, 2);
            drawPuzzle();
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