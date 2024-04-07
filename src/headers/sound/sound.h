#ifndef INCLUDED_SOUND_H
#define INCLUDED_SOUND_H

#include <types.h>

#define SOUND_TYPE_MENU_SELECT 0
#define SOUND_TYPE_MENU_MOVE 1
#define SOUND_TYPE_MENU_CANCEL 2

#define DEFAULT_SOUND_DURATION 10

typedef struct sound_item {
    unsigned long startFrame;
    unsigned long endFrame;
    unsigned char type;
    BOOLEAN isActive;
    void (*start)(void);
    void (*stop)(void);
} SoundItem;

void initSound(void);

void setSound(const unsigned char itemId, const unsigned char type, unsigned short duration);
void offSound(void);

#endif