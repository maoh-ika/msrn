#ifndef INCLUDED_PUZZLE_STAGE_H
#define INCLUDED_PUZZLE_STAGE_H

#define STAGE_OBJECT_PIECE 0
#define STAGE_OBJECT_WALL 1
#define STAGE_OBJECT_PIT 2
#define STAGE_OBJECT_RESERVED 3

#define STAGE_WIDTH 10
#define STAGE_HEIGHT 9
#define STAGE_OBJECT_TILES 2

typedef struct stage_object_t {
    unsigned char type;
    unsigned char x;
    unsigned char y;
    BOOLEAN isUpdated;
} StageObject;

extern StageObject gStage[];

void initStage(const int bgTileIdxOffset);
void prepareStage(void);
BOOLEAN isStageReady(void);
void drawStage(void);
void finalizeStage(void);

void getDirectionCandidates(BOOLEAN* up, BOOLEAN* down, BOOLEAN* left, BOOLEAN* right);
BOOLEAN moveSelectedPiece(void);
BOOLEAN selectUpPiece(void);
BOOLEAN selectDownPiece(void);
BOOLEAN selectLeftPiece(void);
BOOLEAN selectRightPiece(void);

#endif
