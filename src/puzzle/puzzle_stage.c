#include <gb/gb.h>
#include <rand.h>
#include "graphics/stage_tileset.h"
#include "graphics/stage_tilemap.h"
#include "puzzle/puzzle_stage.h"
#include "puzzle/puzzle.h"

StageObject gStage[STAGE_WIDTH * STAGE_HEIGHT];
int gCurrentPitIdx = -1;
unsigned char gRandomCount = 0;

#define TILE_INDEX_WALL 0
#define TILE_INDEX_RESERVED 2
#define TILE_INDEX_PIECE 5
#define TILE_INDEX_PIT 6

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
            gStage[objectIdx].x = x;
            gStage[objectIdx].y = y;
            const int mapTileIdx = (x << 1) + (y << 1) * STAGE_TILEMAP_WIDTH;
            switch (STAGE_TILEMAP[mapTileIdx]) {
                case TILE_INDEX_WALL: gStage[objectIdx].type = STAGE_OBJECT_WALL; break;
                case TILE_INDEX_RESERVED: gStage[objectIdx].type = STAGE_OBJECT_RESERVED; break;
                case TILE_INDEX_PIT: {
                    gStage[objectIdx].type = STAGE_OBJECT_PIT;
                    gCurrentPitIdx = objectIdx;
                    break;
                }
                case TILE_INDEX_PIECE: {
                    if (pieceStartX == 255) {
                        pieceStartX = x;
                        pieceStartY = y;
                    }
                    gStage[objectIdx].type = STAGE_OBJECT_PIECE;
                    gStage[objectIdx].isUpdated = TRUE;
                    break;
                }
                default: gStage[objectIdx].type = STAGE_OBJECT_RESERVED; break;
            }
        }
    }
    
    initPuzzle(STAGE_TILESET_TILE_COUNT, 0, 0, pieceStartX, pieceStartY);
    selectDownPiece();
    gRandomCount = 0;
}

void prepareStage(void) {
    if (isStageReady()) {
        return;
    }
    random();
    ++gRandomCount;
}

BOOLEAN isStageReady() {
    return gRandomCount >= 200;
}

void drawStage(void) {
    for (int i = 0; i < STAGE_WIDTH * STAGE_HEIGHT; ++i) {
        StageObject* obj = &gStage[i];
        if (obj->isUpdated) {
            const int xTopLeft = (obj->x << 1);
            const int yTopLeft = (obj->y << 1);
            const int tileIndex = obj->type == STAGE_OBJECT_PIT ? TILE_INDEX_PIT : TILE_INDEX_RESERVED;
            set_tile_xy(xTopLeft, yTopLeft,  tileIndex); // top left
            set_tile_xy(xTopLeft + 1, yTopLeft, tileIndex); // top right
            set_tile_xy(xTopLeft, yTopLeft + 1, tileIndex); // bottom left
            set_tile_xy(xTopLeft + 1, yTopLeft + 1, tileIndex); // bottom right
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
        StageObject* upObject = &gStage[getObjectIdx(selectedPiece->x, selectedPiece->y - PIECE_HEIGHT_OBJECT)];
        if (upObject->type == STAGE_OBJECT_PIT) {
            *up = TRUE;
        }
    }
    if (selectedPiece->y < STAGE_HEIGHT - 1) {
        StageObject* downObject = &gStage[getObjectIdx(selectedPiece->x, selectedPiece->y + PIECE_HEIGHT_OBJECT)];
        if (downObject->type == STAGE_OBJECT_PIT) {
            *down = TRUE;
        }
    }
    if (selectedPiece->x > 0) {
        StageObject* leftObject = &gStage[getObjectIdx(selectedPiece->x - PIECE_WIDTH_OBJECT, selectedPiece->y)];
        if (leftObject->type == STAGE_OBJECT_PIT) {
            *left = TRUE;
        }
    }
    if (selectedPiece->x < STAGE_WIDTH - 1) {
        StageObject* rightObject = &gStage[getObjectIdx(selectedPiece->x + PIECE_WIDTH_OBJECT, selectedPiece->y)];
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
    for (int py = 0; py < PIECE_HEIGHT_OBJECT; ++py) {
        for (int px = 0; px < PIECE_WIDTH_OBJECT; ++px) {
            StageObject* orgObject = &gStage[getObjectIdx(pieceX + px, pieceY + py)];
            orgObject->type = STAGE_OBJECT_PIT;
            orgObject->isUpdated = TRUE;
            StageObject* dstObject = &gStage[getObjectIdx(selectedPiece->x + px, selectedPiece->y + py)];
            dstObject->type = STAGE_OBJECT_PIECE;
            dstObject->isUpdated = TRUE;
        }
    }
    gCurrentPitIdx = getObjectIdx(pieceX, pieceY);

    return TRUE;
}

BOOLEAN selectUpPiece(void) {
    StageObject* pit = &gStage[gCurrentPitIdx];
    return selectPiece(pit->x, pit->y - PIECE_HEIGHT_OBJECT);
}

BOOLEAN selectDownPiece(void) {
    StageObject* pit = &gStage[gCurrentPitIdx];
    return selectPiece(pit->x, pit->y + PIECE_HEIGHT_OBJECT);
}

BOOLEAN selectLeftPiece(void) {
    StageObject* pit = &gStage[gCurrentPitIdx];
    return selectPiece(pit->x - PIECE_WIDTH_OBJECT, pit->y);
}

BOOLEAN selectRightPiece(void) {
    StageObject* pit = &gStage[gCurrentPitIdx];
    return selectPiece(pit->x + PIECE_WIDTH_OBJECT, pit->y);
}

void random() {
    BOOLEAN (*funcs[])(void) = {selectUpPiece, selectDownPiece, selectLeftPiece, selectRightPiece};
    for (int i = 3; i > 0; --i) {
        const int j = rand() % (i + 1);
        BOOLEAN (*temp)(void) = funcs[i];
        funcs[i] = funcs[j];
        funcs[j] = temp;
    }

    for (int i = 0; i < 4; ++i) {
        if ((*funcs[i])()) {
            moveSelectedPiece();
            break;
        }
    }
}