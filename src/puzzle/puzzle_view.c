#include <gb/gb.h>
#include <stdio.h>
#include "puzzle/puzzle_view.h"
#include "puzzle/puzzle_stage.h"

void initView(void) {
    initStage();
}

void updateView(const unsigned int frame) {
    if (!isStageReady()) {
        prepareStage();
    }

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

void drawView(void) {
    drawStage();
}