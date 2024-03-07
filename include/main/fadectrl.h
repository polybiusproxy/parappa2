#ifndef FADECTRL_H
#define FADECTRL_H

#include "common.h"

#include <eeregs.h>
#include <eestruct.h>

#include "os/mtc.h"

typedef enum {
    FMODE_BLACK_IN = 0,
    FMODE_BLACK_OUT = 256,
    FMODE_WHITE_IN = 1,
    FMODE_WHITE_OUT = 257
} FADE_MODE;

typedef struct { // 0xc
    /* 0x0 */ FADE_MODE fmode;
    /* 0x4 */ int max_time;
    /* 0x8 */ int current_time;
} FMODE_CTRL_STR;

typedef struct { // 0x4
    /* 0x0 */ u_char r;
    /* 0x1 */ u_char g;
    /* 0x2 */ u_char b;
    /* 0x3 */ u_char alp;
} FADE_MAKE_STR;

// TODO: move to main/effect.c once splitted.
void CG_FadeDisp(FADE_MAKE_STR *fade_pp, int pri, sceGsFrame *texFr_pp);

void FadeCtrlMain(void *x);
void FadeCtrlReq(FADE_MODE fmode, int time);

#endif