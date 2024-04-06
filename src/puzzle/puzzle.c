#include <gb/gb.h>
#include <gbdk/platform.h>
#include <string.h>
#include <stdio.h>
#include "graphics/puzzle_hi_tileset.h"
#include "graphics/puzzle_hi_tilemap.h"
#include "graphics/puzzle_mashi_tileset.h"
#include "graphics/puzzle_mashi_tilemap.h"
#include "graphics/puzzle_border_tileset.h"
#include "graphics/puzzle_border_tilemap.h"
#include "puzzle/puzzle.h"
#include "util.h"

Piece gPieces[PUZZLE_PIECE_COUNT];
PuzzleContext gPuzzleContext = {0};

unsigned char gCurrentPuzzleId = PUZZLE_ID_HI;

void calcPieceXYPixel(int* x, int* y, const Piece* piece) {
    *x = piece->x * 16; // 16 = 8px x [tiles in stage object(=2)]
    *y = piece->y * 16;
    toScreenXY(x, y);
}

Piece* getPieceByXY(const unsigned char x, const unsigned char y) {
    for (int i = 0; i < PUZZLE_PIECE_COUNT; ++i) {
        if (x < gPieces[i].x || gPieces[i].x + PIECE_WIDTH_OBJECT - 1 < x) {
            continue;
        }
        if (y < gPieces[i].y || gPieces[i].y + PIECE_HEIGHT_OBJECT - 1 < y) {
            continue;
        }
        return &gPieces[i];
    }
    return NULL;
}

void getPuzzleTileData(
    const unsigned char** tileset,
    int* tilesetCount,
    const unsigned char** tilemap
) {
        if (gCurrentPuzzleId == PUZZLE_ID_HI) {
            SWITCH_ROM(BANK(PUZZLE_HI_TILESET));
            *tileset = PUZZLE_HI_TILESET;
            *tilesetCount = PUZZLE_HI_TILESET_TILE_COUNT;
            *tilemap = PUZZLE_HI_TILEMAP;
        } else if (gCurrentPuzzleId == PUZZLE_ID_MASHI) {
            SWITCH_ROM(BANK(PUZZLE_MASHI_TILESET));
            *tileset = PUZZLE_MASHI_TILESET;
            *tilesetCount = PUZZLE_MASHI_TILESET_TILE_COUNT;
            *tilemap = PUZZLE_MASHI_TILEMAP;
        }
}

Piece* initPuzzle(
    const int bgTileIdxOffset,
    const int spriteTileIdxOffset,
    const int spriteIdOffset,
    const unsigned char startX,
    const unsigned char startY) {

    unsigned char* puzzleTileset;
    int puzzleTileCount;
    unsigned char* puzzleTilemap;
    int puzzleTilemapWidth = PUZZLE_HI_TILEMAP_WIDTH; // all puzzle tilemap has same width height
    int puzzleTilemapHeight = PUZZLE_HI_TILEMAP_HEIGHT;
    getPuzzleTileData(&puzzleTileset, &puzzleTileCount, &puzzleTilemap);
    
    set_bkg_data(bgTileIdxOffset, puzzleTileCount, puzzleTileset);
    
    // shift tile refs in tilemap by offset
    unsigned char shiftedPuzzleTilemap[PUZZLE_HI_TILEMAP_WIDTH * PUZZLE_HI_TILEMAP_HEIGHT];
    shift(shiftedPuzzleTilemap, puzzleTilemap, puzzleTilemapWidth * puzzleTilemapHeight, bgTileIdxOffset);
    
    // init pieces
    for (int pieceY = 0; pieceY < PUZZLE_HEIGHT; ++pieceY) {
        for (int pieceX = 0; pieceX < PUZZLE_WIDTH; ++pieceX) {
            const int pieceIndex = pieceX + pieceY * PUZZLE_WIDTH;
            Piece* piece = &gPieces[pieceIndex];
            // to stage coordinate
            piece->x = (pieceX * PIECE_WIDTH_OBJECT) + startX;
            piece->y = (pieceY * PIECE_HEIGHT_OBJECT) + startY;
            // to tile coordinate
            const int mapIdxTopLeft = pieceX * PIECE_WIDTH_TILES + pieceY * PIECE_HEIGHT_TILES * puzzleTilemapWidth;
            for (int ty = 0; ty < PIECE_HEIGHT_TILES; ++ty) {
                for (int tx = 0; tx < PIECE_WIDTH_TILES; ++tx) {
                    const int tilePos = tx + ty * PIECE_WIDTH_TILES;
                    const int mapIdx = (tx + ty * puzzleTilemapWidth) + mapIdxTopLeft;
                    piece->tileIndices[tilePos] = shiftedPuzzleTilemap[mapIdx];
                }
            }
            piece->isUpated = TRUE;
        }
    }

    // init context
    for (int i = 0; i < PIECE_TILE_COUNT; ++i) {
        gPuzzleContext.borderObjectIds[i] = spriteIdOffset + i;
    }
    gPuzzleContext.bgTileIdxOffset = bgTileIdxOffset;

    // load border tiles 
    set_sprite_data(spriteTileIdxOffset, PUZZLE_BORDER_TILESET_TILE_COUNT, PUZZLE_BORDER_TILESET);

    unsigned char borderTileIndices[PIECE_TILE_COUNT];
    shift(borderTileIndices, PUZZLE_BORDER_TILEMAP, PIECE_TILE_COUNT, spriteTileIdxOffset);
    for (int i = 0; i < PIECE_TILE_COUNT; ++i) {
        set_sprite_tile(gPuzzleContext.borderObjectIds[i], borderTileIndices[i]);
    }

    gPuzzleContext.selectedPiece = NULL;

    return &gPieces[0];
}

void drawPuzzle(void) {
    Piece* selectedPiece = getSelectedPiece();
    
    // draw not selected pieces as background to avoid 40 sprites limit.
    for (int i = 0; i < PUZZLE_PIECE_COUNT; ++i) {
        Piece* piece = &gPieces[i];
        if (!piece->isUpated) {
            continue;
        }
        // to tile coordinate
        const int tileXTopLeft = (piece->x * 2);
        const int tileYTopLeft = (piece->y * 2);
        for (int ty = 0; ty < PIECE_HEIGHT_TILES; ++ty) {
            for (int tx = 0; tx < PIECE_WIDTH_TILES; ++tx) {
                const int idx = tx + ty * PIECE_WIDTH_TILES;
                set_tile_xy(tx + tileXTopLeft, ty + tileYTopLeft, piece->tileIndices[idx]);
            }
        }
        if (piece == selectedPiece) {
            // draw selected piece and border as sprite
            int selectedX;
            int selectedY;
            calcPieceXYPixel(&selectedX, &selectedY, selectedPiece);
            for (int ty = 0; ty < PIECE_HEIGHT_TILES; ++ty) {
                for (int tx = 0; tx < PIECE_WIDTH_TILES; ++tx) {
                    const int objIdx = tx + ty * PIECE_WIDTH_TILES;
                    const int tilePixelX = selectedX + tx * 8;
                    const int tilePixelY = selectedY + ty * 8;
                    move_sprite(gPuzzleContext.borderObjectIds[objIdx], tilePixelX, tilePixelY);
                }
            }
        }
        piece->isUpated = FALSE;
    }
}

void finalizePuzzle(void) {
    for (int i = 0; i < PIECE_TILE_COUNT; ++i) {
        hide_sprite(gPuzzleContext.borderObjectIds[i]);
    }
}

Piece* getSelectedPiece(void) {
   return gPuzzleContext.selectedPiece;
}

BOOLEAN isCompleted(void) {
    int expectedX = 0;
    int expectedY = 0;
    for (int pieceY = 0; pieceY < PUZZLE_HEIGHT; ++pieceY) {
        expectedX = gPieces[0].x;
        expectedY = gPieces[0].y + pieceY * PIECE_HEIGHT_OBJECT;
        for (int pieceX = 0; pieceX < PUZZLE_WIDTH; ++pieceX) {
            const int pieceIndex = pieceX + pieceY * PUZZLE_WIDTH;
            Piece* piece = &gPieces[pieceIndex];
            if (piece->x != expectedX || piece->y != expectedY) {
                return FALSE;
            } 
            expectedX += PIECE_WIDTH_OBJECT;
        }
    }
    return TRUE;
}

void setPuzzleId(const unsigned char id) {
    gCurrentPuzzleId = id;
}

BOOLEAN selectPiece(const unsigned char x, const unsigned char y) {
    Piece* target = getPieceByXY(x, y);
    if (!target) {
        return FALSE;
    }

    target->isUpated = TRUE;    
    gPuzzleContext.selectedPiece = target;
    return TRUE;
}

void moveUp(const unsigned char x, const unsigned char y) {
    Piece* piece = getPieceByXY(x, y);
    if (piece != NULL) {
        piece->y -= PIECE_HEIGHT_OBJECT;
        piece->isUpated = TRUE;
    }
}

void moveDown(const unsigned char x, const unsigned char y) {
    Piece* piece = getPieceByXY(x, y);
    if (piece != NULL) {
        piece->y += PIECE_HEIGHT_OBJECT;
        piece->isUpated = TRUE;
    }
}

void moveLeft(const unsigned char x, const unsigned char y) {
    Piece* piece = getPieceByXY(x, y);
    if (piece != NULL) {
        piece->x -= PIECE_WIDTH_OBJECT;
        piece->isUpated = TRUE;
    }
}

void moveRight(const unsigned char x, const unsigned char y) {
    Piece* piece = getPieceByXY(x, y);
    if (piece != NULL) {
        piece->x += PIECE_WIDTH_OBJECT;
        piece->isUpated = TRUE;
    }
}
