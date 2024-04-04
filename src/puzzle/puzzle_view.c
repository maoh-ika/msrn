#include <gb/gb.h>
#include <stdio.h>
#include "puzzle/puzzle_view.h"
#include "puzzle/puzzle_stage.h"
#include "graphics/hud_tileset.h"
#include "graphics/keyop_tilemap.h"

#define PUZZLE_VIEW_STATE_SHUTDOWN 0
#define PUZZLE_VIEW_STATE_PREVIEW 1
#define PUZZLE_VIEW_STATE_PREPARING 2
#define PUZZLE_VIEW_STATE_PLAYING 3
#define PUZZLE_VIEW_STATE_COMPLETED 4

unsigned char gViewState = PUZZLE_VIEW_STATE_SHUTDOWN;
unsigned char gPreviewFrameCount = 0;

void hideWindow() {
    while(STAT_REG & 3); // wait for HBlank
    HIDE_WIN;
}

void showWindow() {
    SHOW_WIN;
}

void initView(void) {
    if (gViewState != PUZZLE_VIEW_STATE_SHUTDOWN) {
        return;
    }
    set_bkg_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);

    initStage(HUD_TILESET_TILE_COUNT);

    // hide window below keyop instruction
    STAT_REG = 0x40;
    LYC_REG = 8 * KEYOP_TILEMAP_HEIGHT - 1;
    add_LCD(hideWindow);
    add_VBL(showWindow);
    enable_interrupts();
    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    set_win_tiles(0, 0, KEYOP_TILEMAP_WIDTH, KEYOP_TILEMAP_HEIGHT, KEYOP_TILEMAP);
    move_win(7, 0);

    gViewState = PUZZLE_VIEW_STATE_PREVIEW;
}

void updateView(const unsigned int frame) {
    if (gViewState == PUZZLE_VIEW_STATE_PREVIEW) {
        if (gPreviewFrameCount < 180) {
            ++gPreviewFrameCount;
        } else {
            gViewState = PUZZLE_VIEW_STATE_PREPARING;
        }
        return;
    } else if (gViewState == PUZZLE_VIEW_STATE_PREPARING) {
        if (!isStageReady()) {
            prepareStage();
        } else {
            gViewState = PUZZLE_VIEW_STATE_PLAYING;
        }
    } else if (gViewState == PUZZLE_VIEW_STATE_PLAYING) {
        unsigned char padInput = joypad();

        if (padInput & J_A && frame % 7 == 0) {
            BOOLEAN up, down, left, right;
            getDirectionCandidates(&up, &down, &left, &right);
            moveSelectedPiece();
        } else if (padInput & J_UP) {
            selectUpPiece();
        } else if (padInput & J_DOWN) {
            selectDownPiece();
        } else if (padInput & J_LEFT) {
            selectLeftPiece();
        } else if (padInput & J_RIGHT) {
            selectRightPiece();
        }
    }

}

void drawView(void) {
    drawStage();
}