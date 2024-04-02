#include <gb/gb.h>
#include "puzzle/puzzle_view.h"
#include "puzzle/puzzle_stage.h"

void initView(void) {
    initStage();
}

void updateView(void) {
    unsigned char padInput = joypad();

    if (padInput & J_A) {
        BOOLEAN up, down, left, right;
        getDirectionCandidates(&up, &down, &left, &right);
        moveSelectedPiece();
    }
}

void drawView(void) {
    drawStage();
}