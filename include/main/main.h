#ifndef MAIN_H
#define MAIN_H

#include "common.h"

enum
{
    dmyPmTitle = 0,
    dmyPmREPLAY = 1,
    dmyPmSINGLE = 2,
    dmyPmVS_MAN = 3,
    dmyPmVS_COM = 4
};

typedef struct { // 0x8
    /* 0x0 */ int num;
    /* 0x4 */ int *data_pp;
} RT2TRANS_STR;

typedef enum
{
    CBE_NORMAL = 0,
    CBE_SINGLE = 1,
    CBE_VS_MAN = 2,
    CBE_VS_COM = 3,
    CBE_HOOK = 4,
    CBE_MAX = 5
} CANCEL_TYPE_ENUM;

void mainStart(void* xx);

#endif
