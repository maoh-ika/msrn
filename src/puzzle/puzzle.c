#include <gb/gb.h>
#include "graphics/puzzle_hi_tileset.h"
#include "graphics/puzzle_hi_tilemap.h"
#include "graphics/puzzle_border_tileset.h"
#include "graphics/puzzle_border_tilemap.h"
#include "puzzle/puzzle.h"

Piece pieces[PUZZLE_PIECE_COUNT];
PuzzleState puzzleState = {0};

void shift(unsigned char* out, const unsigned char* in, const int len, const int offset) {
    for (int i = 0; i < len; ++i) {
        out[i] = in[i] + offset;
    }
}

void calcPieceXYPixel(int* x, int* y, const Piece* piece) {
    *x = piece->x * PIECE_WIDTH + 8;
    *y = piece->y * PIECE_HEIGHT + 16;
}

Piece* getPieceByXY(const unsigned char x, const unsigned char y) {
    for (int i = 0; i < PUZZLE_PIECE_COUNT; ++i) {
        if (pieces[i].x == x && pieces[i].y == y) {
            return &pieces[i];
        }
    }
    return NULL;
}

Piece* initPuzzle(
    const int bgTileIdxOffset,
    const int spriteTileIdxOffset,
    const int spriteIdOffset,
    const unsigned char startX,
    const unsigned char startY) {
    set_bkg_data(bgTileIdxOffset, PUZZLE_HI_TILESET_TILE_COUNT, PUZZLE_HI_TILESET);
    set_sprite_data(spriteTileIdxOffset, PUZZLE_BORDER_TILESET_TILE_COUNT, PUZZLE_BORDER_TILESET);
    
    // shift tile refs in tilemap by offset
    unsigned char puzzleTileMap[PUZZLE_HI_TILEMAP_WIDTH * PUZZLE_HI_TILEMAP_HEIGHT];
    shift(puzzleTileMap, PUZZLE_HI_TILEMAP, PUZZLE_HI_TILEMAP_WIDTH * PUZZLE_HI_TILEMAP_HEIGHT, bgTileIdxOffset);
    
    shift(puzzleState.borderTileIds, PUZZLE_BORDER_TILEMAP, 4, spriteTileIdxOffset);

    // init pieces
    for (int y = 0; y < PUZZLE_HEIGHT; ++y) {
        for (int x = 0; x < PUZZLE_WIDTH; ++x) {
            const int pieceIdx = x + y * PUZZLE_WIDTH;
            pieces[pieceIdx].x = x + startX;
            pieces[pieceIdx].y = y + startY;
            const int mapIdxTopLeft = (x << 1) + (y << 1) * PUZZLE_HI_TILEMAP_WIDTH;
            pieces[pieceIdx].tileIds[0] = puzzleTileMap[mapIdxTopLeft]; // top left
            pieces[pieceIdx].tileIds[1] = puzzleTileMap[mapIdxTopLeft + 1]; // top right
            pieces[pieceIdx].tileIds[2] = puzzleTileMap[mapIdxTopLeft + PUZZLE_HI_TILEMAP_WIDTH]; // bottom left
            pieces[pieceIdx].tileIds[3] = puzzleTileMap[mapIdxTopLeft + PUZZLE_HI_TILEMAP_WIDTH + 1]; // bottom right
        }
    }

    // init state
    puzzleState.selectedPiece = &pieces[0];
    puzzleState.borderObjectIds[0] = spriteIdOffset;
    puzzleState.borderObjectIds[1] = spriteIdOffset + 1;
    puzzleState.borderObjectIds[2] = spriteIdOffset + 2;
    puzzleState.borderObjectIds[3] = spriteIdOffset + 3;
    
    set_sprite_tile(puzzleState.borderObjectIds[0], puzzleState.borderTileIds[0]); // top left
    set_sprite_tile(puzzleState.borderObjectIds[1], puzzleState.borderTileIds[1]); // top right
    set_sprite_tile(puzzleState.borderObjectIds[2], puzzleState.borderTileIds[2]); // bottom left
    set_sprite_tile(puzzleState.borderObjectIds[3], puzzleState.borderTileIds[3]); // bottom right

    return &pieces[0];
}

void drawPuzzle(void) {
    // draw pieces
    for (int i = 0; i < PUZZLE_PIECE_COUNT; ++i) {
        Piece* piece = &piece[i];
        if (pieces->isUpated) {
            const int xTopLeft = (pieces->x << 1);
            const int yTopLeft = (piece->y << 1);
            set_tile_xy(xTopLeft, yTopLeft, piece->tileIds[0]); // top left
            set_tile_xy(xTopLeft + 1, yTopLeft, piece->tileIds[1]); // top right
            set_tile_xy(xTopLeft, yTopLeft + 1, pieces->tileIds[2]); // bottom left
            set_tile_xy(xTopLeft + 1, yTopLeft + 1, pieces->tileIds[3]); // bottom right
        }
        piece->isUpated = FALSE;
    }

    // draw selected piece border
    const Piece* selectedPiece = getSelectedPiece();

    int selectedX;
    int selectedY;
    calcPieceXYPixel(&selectedX, &selectedY, selectedPiece);
    move_sprite(puzzleState.borderObjectIds[0], selectedX, selectedY);
    move_sprite(puzzleState.borderObjectIds[1], selectedX + 8, selectedY);
    move_sprite(puzzleState.borderObjectIds[2], selectedX, selectedY + 8);
    move_sprite(puzzleState.borderObjectIds[3], selectedX + 8, selectedY + 8);
}

Piece* getSelectedPiece(void) {
   return puzzleState.selectedPiece;
}

void selectPiece(const unsigned char x, const unsigned char y) {
    Piece* target = getPieceByXY(x, y);
    if (!target) {
        return;
    } 
}

void moveUp(const unsigned char x, const unsigned char y) {
    Piece* piece = getPieceByXY(x, y);
    if (piece != NULL) {
        --piece->y;
        piece->isUpated = TRUE;
    }
}

void moveDown(const unsigned char x, const unsigned char y) {
    Piece* piece = getPieceByXY(x, y);
    if (piece != NULL) {
        ++piece->y;
        piece->isUpated = TRUE;
    }
}

void moveLeft(const unsigned char x, const unsigned char y) {
    Piece* piece = getPieceByXY(x, y);
    if (piece != NULL) {
        --piece->x;
        piece->isUpated = TRUE;
    }
}

void moveRight(const unsigned char x, const unsigned char y) {
    Piece* piece = getPieceByXY(x, y);
    if (piece != NULL) {
        ++piece->x;
        piece->isUpated = TRUE;
    }
}
