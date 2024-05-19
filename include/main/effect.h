#ifndef EFFECT_H
#define EFFECT_H

#include "common.h"

typedef enum {
    WM_WSLICE = 0,
    WM_HSLICE = 1,
    WM_MAX = 2
} WMODE_ENUM;

typedef struct { // 0x2c
    /* 0x00 */ int wmode;
    /* 0x04 */ int linecnt;
    /* 0x08 */ short int x;
    /* 0x0a */ short int y;
    /* 0x0c */ short int sizeW;
    /* 0x0e */ short int sizeH;
    /* 0x10 */ short int addW;
    /* 0x12 */ short int addH;
    /* 0x14 */ short int u;
    /* 0x16 */ short int v;
    /* 0x18 */ short int addU;
    /* 0x1a */ short int addV;
    /* 0x1c */ float mvSize;
    /* 0x20 */ float plsAng1line;
    /* 0x24 */ float plsAng1time;
    /* 0x28 */ float currentAng;
} WAVE_STR;

#endif