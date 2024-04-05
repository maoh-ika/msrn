#include <gb/gb.h>
#include <stdio.h>
#include "puzzle/puzzle_view.h"
#include "puzzle/puzzle_stage.h"
#include "graphics/hud_tileset.h"
#include "graphics/keyop_tilemap.h"
#include "view.h"
#include "util.h"

#define PUZZLE_VIEW_STATE_SHUTDOWN 0
#define PUZZLE_VIEW_STATE_PREVIEW 1
#define PUZZLE_VIEW_STATE_PREPARING 2
#define PUZZLE_VIEW_STATE_PLAYING 3
#define PUZZLE_VIEW_STATE_COMPLETED 4

unsigned char gViewState = PUZZLE_VIEW_STATE_SHUTDOWN;
unsigned char gPreviewFrameCount = 0;

void initPuzzleView(void) {
    HIDE_BKG;
    
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
    
    SHOW_BKG;

    gViewState = PUZZLE_VIEW_STATE_PREVIEW;
}

int updatePuzzleView() {
    if (gViewState == PUZZLE_VIEW_STATE_PREVIEW) {
        if (gPreviewFrameCount < 180) {
            ++gPreviewFrameCount;
        } else {
            gViewState = PUZZLE_VIEW_STATE_PREPARING;
        }
    } else if (gViewState == PUZZLE_VIEW_STATE_PREPARING) {
        if (!isStageReady()) {
            prepareStage();
        } else {
            gViewState = PUZZLE_VIEW_STATE_PLAYING;
        }
    } else if (gViewState == PUZZLE_VIEW_STATE_PLAYING) {
        unsigned char padInput = joypad();

        if (padInput & J_A || padInput & J_START) {
            waitpadup();
            moveSelectedPiece();
            if (isCompleted()) {
                return VIEW_ID_TITLE;
            }
        } else if (padInput & J_UP) {
            waitpadup();
            selectUpPiece();
        } else if (padInput & J_DOWN) {
            waitpadup();
            selectDownPiece();
        } else if (padInput & J_LEFT) {
            waitpadup();
            selectLeftPiece();
        } else if (padInput & J_RIGHT) {
            waitpadup();
            selectRightPiece();
        }
    }

    return VIEW_ID_PUZZLE;
}

void drawPuzzleView(void) {
    drawStage();
}