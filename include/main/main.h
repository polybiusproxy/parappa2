#ifndef MAIN_H
#define MAIN_H

#include "common.h"

#include "main/etc.h"
#include "main/mcctrl.h"

#include <eetypes.h>

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

// TODO: move somewhere else
typedef enum {
    SEL_MENU_STAGESEL = 0,
    SEL_MENU_SAVE = 1,
    SEL_MENU_REPLAY = 2
} SEL_MENU_ENUM;

typedef struct { // 0xc
    /* 0x0 */ GAME_STATUS *game_status_p;
    /* 0x4 */ MC_REP_STR *mc_rep_str_p;
    /* 0x8 */ SEL_MENU_ENUM sel_menu_enum;
} MENU_STR;

void mainStart(void* xx);

#endif
