#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include <types.h>

#include "graphics/title_tileset.h"
#include "graphics/title_tilemap.h"
#include "graphics/stage_tileset.h"
#include "graphics/stage_tilemap.h"
#include "graphics/puzzle_hi_tileset.h"
#include "graphics/puzzle_hi_tilemap.h"
#include "title/title_view.h"
#include "puzzle/puzzle_view.h"
#include "view.h"

#define MAIN_STATE_VIEW_CHANGED 0
#define MAIN_STATE_VIEW_ACTIVE 1

void main( void ) {
    HIDE_BKG;

    void (*initViewFuncs[])(void) = { initTitleView, initPuzzleView };
    int (*updateViewFuncs[])(void) = { updateTitleView, updatePuzzleView };
    void (*drawViewFuncs[])(void) = { drawTitleView, drawPuzzleView };
    unsigned char currentViewIid = VIEW_ID_TITLE;
    unsigned char mainState = MAIN_STATE_VIEW_CHANGED;
    
    SHOW_BKG;
    SHOW_SPRITES;

    BOOLEAN isRunning = TRUE;
    
    while (isRunning) {
        if (mainState == MAIN_STATE_VIEW_CHANGED) {
            initViewFuncs[currentViewIid]();
            mainState = MAIN_STATE_VIEW_ACTIVE;
        } else if (mainState == MAIN_STATE_VIEW_ACTIVE) {
            const int newxView = updateViewFuncs[currentViewIid]();
            if (newxView != currentViewIid) {
                currentViewIid = newxView;
                mainState = MAIN_STATE_VIEW_CHANGED;
            } else {
                drawViewFuncs[currentViewIid]();
            }
        }
        wait_vbl_done();
    }
}