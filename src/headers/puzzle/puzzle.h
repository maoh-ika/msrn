#ifndef INCLUDED_PUZZLE_H
#define INCLUDED_PUZZLE_H

#define PUZZLE_WIDTH 8
#define PUZZLE_HEIGHT 7
#define PUZZLE_PIECE_COUNT 56
#define PIECE_WIDTH 16 // in pixel
#define PIECE_HEIGHT 16 // in pixel

typedef struct piece_t {
    unsigned char x; // in puzzle coordinate
    unsigned char y; // in puzzle coordinate
    unsigned char tileIds[4]; // sprite tiles in 8x8 format
    BOOLEAN isUpated;
} Piece;

typedef struct puzzle_state_t {
    Piece* selectedPiece;
    unsigned char borderTileIds[4];
    unsigned char borderObjectIds[4];
} PuzzleState;

extern Piece pieces[];
extern PuzzleState puzzleState;

Piece* initPuzzle(
    const int bgTileIdxOffset,
    const int spriteTileIdxOffset,
    const int spriteIdOffset,
    const unsigned char startX, // top left in stage xy coordinate
    const unsigned char startY);

void drawPuzzle(void);

Piece* getSelectedPiece(void);

void selectPiece(const unsigned char x, const unsigned char y);
void moveUp(const unsigned char x, const unsigned char y);
void moveDown(const unsigned char x, const unsigned char y);
void moveLeft(const unsigned char x, const unsigned char y);
void moveRight(const unsigned char x, const unsigned char y);

#endif