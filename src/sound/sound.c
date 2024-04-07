#include <gb/gb.h>
#include <time.h>
#include "sound/sound.h"

#define SOUND_ITEM_COUNT 2
static SoundItem gItems[SOUND_ITEM_COUNT];
static unsigned long gCurrentFrame;

void menuSelect(void) {
    // チャンネル1(矩形波スイープあり)
    NR10_REG = 0x00;// b6-4:スイープ時間=0 / b3:スイープ方向=0(上) / b2-0:スイープ変化量=0
    NR11_REG = 0xC0;// b7-6:デューティ比=3 / b5-0:音長カウンタ=0
    NR12_REG = 0xF0;// b7-4:初期音量=15 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間=0
    NR13_REG = 0x00;// b7-0:周波数(11ビットの下位8ビット)
    NR14_REG = 0x85;// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
}

void menuMove(void) {
    // チャンネル1(矩形波スイープあり)
    NR10_REG = 0x00;// b6-4:スイープ時間=0 / b3:スイープ方向=0(上) / b2-0:スイープ変化量=0
    NR11_REG = 0xC0;// b7-6:デューティ比=3 / b5-0:音長カウンタ=0
    NR12_REG = 0xF0;// b7-4:初期音量=15 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間=0
    NR13_REG = 0x00;// b7-0:周波数(11ビットの下位8ビット)
    NR14_REG = 0x82;// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
}

void menuCancel(void) {
    // チャンネル1(矩形波スイープあり)
    NR10_REG = 0x00;// b6-4:スイープ時間=0 / b3:スイープ方向=0(上) / b2-0:スイープ変化量=0
    NR11_REG = 0xC0;// b7-6:デューティ比=3 / b5-0:音長カウンタ=0
    NR12_REG = 0xF0;// b7-4:初期音量=15 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間=0
    NR13_REG = 0x0F;// b7-0:周波数(11ビットの下位8ビット)
    NR14_REG = 0x84;// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
}

void clearCh1(void) {
    NR10_REG = 0;
    NR11_REG = 0;
    NR12_REG = 0;
    NR13_REG = 0;
    NR14_REG = 0;
}

void updateSound(void) {
    gCurrentFrame = clock();
    for (int i = 0; i < SOUND_ITEM_COUNT; ++i) {
        if (!gItems[i].isActive) {
            continue;
        }
        if (gItems[i].endFrame <= gCurrentFrame) {
            gItems[i].stop();
            gItems[i].isActive = FALSE;
        }
    }
}

void initSound(void) {
    clearCh1();
    NR21_REG = 0;
    NR22_REG = 0;
    NR23_REG = 0;
    NR24_REG = 0;
    NR30_REG = 0;
    NR31_REG = 0;
    NR32_REG = 0;
    NR33_REG = 0;
    NR34_REG = 0;
    NR41_REG = 0;
    NR42_REG = 0;
    NR43_REG = 0;
    NR44_REG = 0;
    NR50_REG = 0;
    NR51_REG = 0;
    NR52_REG = 0;
    
    NR52_REG = 0x80;// b7:全チャンネルON=1 / b3-0:チャンネル4～1再生中フラグは読込のみ
    NR50_REG = 0x77;// b7:左VinOF=0F / b6-4:左音量=7 / b3:右VinOFF=0 / b2-0:右音量=7
    NR51_REG = 0xFF;// b7-4:チャンネル4～1左出力ON=F(1111) / b3-0:チャンネル4～1右出力ON=F(1111)

    add_VBL(updateSound);// VBL割込に追加
    enable_interrupts();// 割込有効
    set_interrupts(VBL_IFLAG);// VBL割
}

void setSound(const unsigned char itemId, const unsigned char type, unsigned short duration) {
    if (SOUND_ITEM_COUNT <= itemId) {
        return;
    }
    if (gItems[itemId].isActive) {
        gItems[itemId].stop();
        gItems[itemId].isActive = FALSE;
    }

    gItems[itemId].startFrame = gCurrentFrame;
    if ((UINT32_MAX - gCurrentFrame) < duration) {
        gItems[itemId].endFrame = UINT32_MAX;
    } else {
        gItems[itemId].endFrame = gCurrentFrame + duration;
    }

    void (*startFunc)(void) = NULL;
    void (*stopFunc)(void) = NULL;
    if (type == SOUND_TYPE_MENU_SELECT) {
        startFunc = menuSelect;
        stopFunc = clearCh1;
    } else if (type == SOUND_TYPE_MENU_MOVE) {
        startFunc = menuMove;
        stopFunc = clearCh1;
    } else if (type == SOUND_TYPE_MENU_CANCEL) {
        startFunc = menuCancel;
        stopFunc = clearCh1;
    }

    if (startFunc) {
        gItems[itemId].start = startFunc;
        gItems[itemId].stop = stopFunc;
        gItems[itemId].start();
        gItems[itemId].isActive = TRUE;
    } 
}

void offSound(void) {
    initSound();
}