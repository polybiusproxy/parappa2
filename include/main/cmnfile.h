#ifndef CMNFILE_H
#define CMNFILE_H

#include "common.h"

#include "os/tim2.h"

#include <stdio.h>

typedef enum
{
    CMN_NONE = 0,   // None
    CMN_VRAM = 1,   // TIM2 textures
    CMN_SND = 2,    // Sounds
    CMN_ONMEM = 3,  // Models, animations, etc. to store in the memory pool
    CMN_MAX = 4     // N/A
} CMN_FILE_TYPE_ENUM;

typedef struct { // 0x10
    /* 0x0 */ int fnum;
    /* 0x4 */ int ftype;
    /* 0x8 */ int f_size;
    /* 0xc */ int pad;
    /* 0x10 */ int adr[0];
} CMN_FILE_STR;

int     cmnfTim2Trans(void);
void*   cmnfGetFileAdrs(int num);
int     cmnfGetFileSize(int num);

#endif