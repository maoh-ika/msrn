#include <gb/gb.h>
#include "graphics/stage_tileset.h"
#include "graphics/stage_tilemap.h"
#include "puzzle/puzzle_stage.h"
#include "puzzle/puzzle.h"

StageObject stage[STAGE_WIDTH * STAGE_HEIGHT];

unsigned char getObjectIdx(const unsigned char x, const unsigned char y) {
    return x + y * STAGE_WIDTH;
}

void initStage(void) {
    set_bkg_data(0, STAGE_TILESET_TILE_COUNT, STAGE_TILESET);
    set_bkg_tiles(0, 0, STAGE_TILEMAP_WIDTH, STAGE_TILEMAP_HEIGHT, STAGE_TILEMAP);

    unsigned char pieceStartX = 255;
    unsigned char pieceStartY = 255;
    for (int y = 0; y < STAGE_HEIGHT; ++y) {
        for (int x = 0; x < STAGE_WIDTH; ++x) {
            const int objectIdx = getObjectIdx(x, y);
            stage[objectIdx].x = x;
            stage[objectIdx].y = y;
            const int mapTileIdx = (x << 1) + (y << 1) * STAGE_TILEMAP_WIDTH;
            switch (STAGE_TILEMAP[mapTileIdx]) {
                case 0x00: stage[objectIdx].type = STAGE_OBJECT_WALL; break;
                case 0x02: stage[objectIdx].type = STAGE_OBJECT_RESERVED; break;
                case 0x05: stage[objectIdx].type = STAGE_OBJECT_PIT; break;
                case 0x06: {
                    if (pieceStartX == 255) {
                        pieceStartX = x;
                        pieceStartY = y;
                    }
                    stage[objectIdx].type = STAGE_OBJECT_PIECE;
                    break;
                }
                default: stage[objectIdx].type = STAGE_OBJECT_RESERVED; break;
            }
        }
    }
    
    initPuzzle(STAGE_TILESET_TILE_COUNT, 0, 0, pieceStartX, pieceStartY);
}

void drawStage(void) {
    for (int i = 0; i < STAGE_WIDTH * STAGE_HEIGHT; ++i) {
        StageObject* obj = &stage[i];
        if (obj->isUpdated && obj->type == STAGE_OBJECT_PIT) {
            const int xTopLeft = (obj->x << 1);
            const int yTopLeft = (obj->y << 1);
            set_tile_xy(xTopLeft, yTopLeft, 5); // top left
            set_tile_xy(xTopLeft + 1, yTopLeft, 5); // top right
            set_tile_xy(xTopLeft, yTopLeft + 1, 5); // bottom left
            set_tile_xy(xTopLeft + 1, yTopLeft + 1, 5); // bottom right
        }
        obj->isUpdated = FALSE;
    }

    drawPuzzle();
}

void getDirectionCandidates(BOOLEAN* up, BOOLEAN* down, BOOLEAN* left, BOOLEAN* right) {
    *up = FALSE;
    *down = FALSE;
    *left = FALSE;
    *right = FALSE;

    Piece* selectedPiece = getSelectedPiece();
    if (!selectedPiece) {
        return;
    }
    const int objectIdx = selectedPiece->x + selectedPiece->y * STAGE_WIDTH;
    if (selectedPiece->y > 0) {
        StageObject* upObject = &stage[getObjectIdx(selectedPiece->x, selectedPiece->y - 1)];
        if (upObject->type == STAGE_OBJECT_PIT) {
            *up = TRUE;
        }
    }
    if (selectedPiece->y < STAGE_HEIGHT - 1) {
        StageObject* downObject = &stage[getObjectIdx(selectedPiece->x, selectedPiece->y + 1)];
        if (downObject->type == STAGE_OBJECT_PIT) {
            *down = TRUE;
        }
    }
    if (selectedPiece->x > 0) {
        StageObject* leftObject = &stage[getObjectIdx(selectedPiece->x - 1, selectedPiece->y)];
        if (leftObject->type == STAGE_OBJECT_PIT) {
            *left = TRUE;
        }
    }
    if (selectedPiece->x < STAGE_WIDTH - 1) {
        StageObject* rightObject = &stage[getObjectIdx(selectedPiece->x + 1, selectedPiece->y)];
        if (rightObject->type == STAGE_OBJECT_PIT) {
            *right = TRUE;
        }
    }
}

BOOLEAN moveSelectedPiece(void) {
    Piece* selectedPiece = getSelectedPiece();
    if (!selectedPiece) {
        return FALSE;
    }

    // update piece position
    BOOLEAN up, down, left, right;
    getDirectionCandidates(&up, &down, &left, &right);
    const unsigned char pieceX = selectedPiece->x;
    const unsigned char pieceY = selectedPiece->y;
    if (up) {
        moveUp(pieceX, pieceY);
    } else if (down) {
        moveDown(pieceX, pieceY);
    } else if (left) {
        moveLeft(pieceX, pieceY);
    } else if (right) {
        moveRight(pieceX, pieceY);
    } else {
        return FALSE;
    }

    // update object map
    StageObject* orgObject = &stage[getObjectIdx(pieceX, pieceY)];
    orgObject->type = STAGE_OBJECT_PIT;
    orgObject->isUpdated = TRUE;
    StageObject* dstObject = &stage[getObjectIdx(selectedPiece->x, selectedPiece->y)];
    dstObject->type = STAGE_OBJECT_PIECE;
    dstObject->isUpdated = TRUE;

    return TRUE;
}
