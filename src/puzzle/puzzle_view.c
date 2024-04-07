#include <gb/gb.h>
#include <stdio.h>
#include <string.h>
#include "puzzle/puzzle_view.h"
#include "puzzle/puzzle_stage.h"
#include "graphics/hud_tileset.h"
#include "graphics/puzzle_keyop_tilemap.h"
#include "graphics/stage_clear_tilemap.h"
#include "sound/sound.h"
#include "view.h"
#include "util.h"
#include "puzzle/puzzle.h"

#define PUZZLE_VIEW_STATE_PREVIEW 0
#define PUZZLE_VIEW_STATE_PREPARING 1
#define PUZZLE_VIEW_STATE_PLAYING 2
#define PUZZLE_VIEW_STATE_GIVEUP 3
#define PUZZLE_VIEW_STATE_CLEAR 4
#define PUZZLE_VIEW_STATE_COMPLETED 5

unsigned char gViewState = PUZZLE_VIEW_STATE_COMPLETED;

void initPuzzleView(void) {
    HIDE_SPRITES;
    HIDE_BKG;
    
    set_bkg_data(0, HUD_TILESET_TILE_COUNT, HUD_TILESET);

    initStage(HUD_TILESET_TILE_COUNT);

    // hide window below keyop instruction
    STAT_REG = 0x40;
    LYC_REG = 8 * PUZZLE_KEYOP_TILEMAP_HEIGHT - 1;
    add_LCD(hideWindow);
    add_VBL(showWindow);
    enable_interrupts();
    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    set_win_tiles(0, 0, PUZZLE_KEYOP_TILEMAP_WIDTH, PUZZLE_KEYOP_TILEMAP_HEIGHT, PUZZLE_KEYOP_TILEMAP);
    move_win(7, 0);

    SHOW_BKG;
    SHOW_SPRITES;

    gViewState = PUZZLE_VIEW_STATE_PREVIEW;
}

int updatePuzzleView(void) {
    if (gViewState == PUZZLE_VIEW_STATE_PREVIEW) {
        unsigned char padInput = joypad();
        if (padInput & J_A || padInput & J_START) {
            waitpadup();
            setSound(0, SOUND_TYPE_MENU_SELECT, DEFAULT_SOUND_DURATION);
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
        
        if (!(padInput & J_B)) {
            showAnswer(FALSE);
        }

        if (padInput & J_B && padInput & J_START) {
            gViewState = PUZZLE_VIEW_STATE_GIVEUP;
            setSound(0, SOUND_TYPE_MENU_SELECT, DEFAULT_SOUND_DURATION);
        } else if (padInput & J_A || padInput & J_START) {
            moveSelectedPiece();
            if (isCompleted()) {
                finalizeStage();
                remove_LCD(hideWindow);
                remove_VBL(showWindow);
                gViewState = PUZZLE_VIEW_STATE_CLEAR;
            }
            waitpadup();
            setSound(0, SOUND_TYPE_MENU_SELECT, DEFAULT_SOUND_DURATION);
        } else if (padInput & J_B) {
            showAnswer(TRUE);
        } else if (padInput & J_UP) {
            waitpadup();
            if (selectUpPiece()) {
                setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
            }
        } else if (padInput & J_DOWN) {
            waitpadup();
            if (selectDownPiece()) {
                setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
            }
        } else if (padInput & J_LEFT) {
            waitpadup();
            if (selectLeftPiece()) {
                setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
            }
        } else if (padInput & J_RIGHT) {
            waitpadup();
            if (selectRightPiece()) {
                setSound(0, SOUND_TYPE_MENU_MOVE, DEFAULT_SOUND_DURATION);
            }
        }
    } else if (gViewState == PUZZLE_VIEW_STATE_GIVEUP) {
        correctStage();
        if (isCompleted()) {
            finalizeStage();
            remove_LCD(hideWindow);
            remove_VBL(showWindow);
            gViewState = PUZZLE_VIEW_STATE_CLEAR;
        }
    } else if (gViewState == PUZZLE_VIEW_STATE_CLEAR) {
        gClearFlags |= (1 << getPuzzleId());
        gViewState = PUZZLE_VIEW_STATE_COMPLETED;
    } else if (gViewState == PUZZLE_VIEW_STATE_COMPLETED) {
        unsigned char padInput = joypad();
        if (padInput & J_A || padInput & J_START) {
            setSound(0, SOUND_TYPE_MENU_SELECT, DEFAULT_SOUND_DURATION);
            return VIEW_ID_TITLE;
        }
    }

    return VIEW_ID_PUZZLE;
}

void drawPuzzleView(void) {
    if (gViewState <= PUZZLE_VIEW_STATE_GIVEUP) {
        drawStage();
    } else if (gViewState == PUZZLE_VIEW_STATE_CLEAR) {
        set_bkg_tiles(0, 0, STAGE_CLEAR_TILEMAP_WIDTH, STAGE_CLEAR_TILEMAP_HEIGHT, STAGE_CLEAR_TILEMAP);
        HIDE_WIN;
    }
}

void finalizePuzzleView(void) {
}