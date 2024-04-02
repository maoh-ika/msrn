#ifndef INCLUDED_PUZZLE_STAGE_H
#define INCLUDED_PUZZLE_STAGE_H

#define STAGE_OBJECT_PIECE 0
#define STAGE_OBJECT_WALL 1
#define STAGE_OBJECT_PIT 2
#define STAGE_OBJECT_RESERVED 3

#define STAGE_WIDTH 10
#define STAGE_HEIGHT 9

typedef struct stage_object_t {
    unsigned char type;
    unsigned char x;
    unsigned char y;
    BOOLEAN isUpdated;
} StageObject;

extern StageObject stage[];

void initStage(void);
void drawStage(void);

void getDirectionCandidates(BOOLEAN* up, BOOLEAN* down, BOOLEAN* left, BOOLEAN* right);
BOOLEAN moveSelectedPiece(void);

#endif
