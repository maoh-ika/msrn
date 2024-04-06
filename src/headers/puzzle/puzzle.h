#ifndef INCLUDED_PUZZLE_H
#define INCLUDED_PUZZLE_H

#define PUZZLE_WIDTH 4
#define PUZZLE_HEIGHT 7
#define PUZZLE_PIECE_COUNT 28
#define PIECE_WIDTH_PIXELS 32
#define PIECE_HEIGHT_PIXELS 16
#define PIECE_WIDTH_TILES 4
#define PIECE_HEIGHT_TILES 2
#define PIECE_TILE_COUNT 8
#define PIECE_WIDTH_OBJECT 2
#define PIECE_HEIGHT_OBJECT 1

#define PUZZLE_ID_HI 0
#define PUZZLE_ID_MASHI 1
#define PUZZLE_ID_IIKOCHAN 2
#define PUZZLE_ID_NICE 3
#define PUZZLE_ID_MOEGI 4

typedef struct piece_t {
    unsigned char x; // in puzzle coordinate
    unsigned char y; // in puzzle coordinate
    unsigned char tileIndices[PIECE_TILE_COUNT]; // sprite tiles in 8x8 format
    BOOLEAN isUpated;
} Piece;

typedef struct puzzle_context_t {
    Piece* selectedPieceIdx;
    unsigned char borderTileIndices[PIECE_TILE_COUNT];
    unsigned char borderObjectIds[PIECE_TILE_COUNT];
    int bgTileIdxOffset;
    BOOLEAN showBorder;
} PuzzleContext;

Piece* initPuzzle(
    const int bgTileIdxOffset,
    const int spriteTileIdxOffset,
    const int spriteIdOffset,
    const unsigned char startX, // top left in stage xy coordinate
    const unsigned char startY,
    BOOLEAN showBorder);

void drawPuzzle(void);
void finalizePuzzle(void);

Piece* getSelectedPiece(void);
BOOLEAN isCompleted(void);
void setPuzzleId(const unsigned char id);

BOOLEAN selectPiece(const unsigned char x, const unsigned char y);
void moveUp(const unsigned char x, const unsigned char y);
void moveDown(const unsigned char x, const unsigned char y);
void moveLeft(const unsigned char x, const unsigned char y);
void moveRight(const unsigned char x, const unsigned char y);

#endif