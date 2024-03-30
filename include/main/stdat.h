#ifndef STDAT_H
#define STDAT_H

#include "common.h"

#include "os/mtc.h"
#include "main/cdctrl.h"
#include "main/subt.h"

typedef enum {
    PSTEP_SERIAL = 0,
    PSTEP_HOOK = 1,
    PSTEP_GAME = 2,
    PSTEP_BONUS = 3,
    PSTEP_VS = 4,
    PSTEP_XTR = 5,
    PSTEP_MAX = 6
} PLAY_STEP;

// Temp typedefs!
typedef int EVENTREC;
typedef int SCR_MAIN;
typedef int TAPLVL_STR;

typedef struct { // 0xd0
    /* 0x00 */ PLAY_STEP play_step;
    /* 0x04 */ char *ply_name;
    /* 0x08 */ float tempo;
    /* 0x0c */ int stage;
    /* 0x10 */ EVENTREC *ev_pp;
    /* 0x14 */ SCR_MAIN *scr_pp;
    /* 0x18 */ JIMAKU_STR *jimaku_str_pp;
    /* 0x1c */ FILE_STR intfile;
    /* 0x48 */ FILE_STR sndfile[3];
    /* 0xcc */ TAPLVL_STR *taplvl_str_pp;
} STDAT_DAT;

typedef struct { // 0x38
    /* 0x00 */ FILE_STR ovlfile;
    /* 0x2c */ int stdat_dat_num;
    /* 0x30 */ STDAT_DAT *stdat_dat_pp;
    /* 0x34 */ char *strec_name;
} STDAT_REC;

void stDatFirstFileSearch(void);

#endif