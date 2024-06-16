#ifndef MCCTRL_H
#define MCCTRL_H

#include "common.h"

#include <eetypes.h>

#include "main/etc.h"

typedef struct { // 0x4
    /* 0x0 */ u_int timeP;
    /* 0x2 */ u_char padId;
    /* 0x2 */ u_char resT;
    /* 0x2 */ u_char holdT;
    /* 0x2 */ u_char ply;
    /* 0x3 */ u_char useL;
} MC_REP_DAT;

typedef u_char VSOTHSAVE[32];

typedef struct { // 0x10
    /* 0x0 */ int now_score;
    /* 0x4 */ int exam_score[3];
} MC_REP_SCR;

// FIXME: Struct size is wrong (0x9528)
typedef struct { // 0x4528
    /* 0x0000 */ PLAY_MODE play_modeS;
    /* 0x0004 */ PLAY_TYPE play_typeS;
    /* 0x0008 */ TAP_ROUND_ENUM roundS;
    /* 0x000c */ PLAY_TABLE_MODE play_table_modeS;
    /* 0x0010 */ int play_stageS;
    /* 0x0014 */ LEVEL_VS_ENUM level_vs_enumS;
    /* 0x0018 */ u_int scoreN_cnt;
    /* 0x001c */ MC_REP_SCR mc_rep_scr[256];
    /* 0x101c */ u_int levelN_cnt;
    /* 0x1020 */ u_char levelN[128];
    /* 0x10a0 */ u_int mc_rep_dat_cnt;
    /* 0x10a4 */ MC_REP_DAT mc_rep_dat[2560];
    /* 0x38a4 */ u_int mc_vsoth_cnt;
    /* 0x38a8 */ VSOTHSAVE vsothsave[100];
} MC_REP_STR;

typedef struct { // 0x1c
    /* 0x00 */ u_int cl_scoreN_cnt;
    /* 0x04 */ u_int cl_levelN_cnt;
    /* 0x08 */ u_int cl_mc_rep_dat_cnt[4];
    /* 0x18 */ u_int cl_vsoth_cnt;
} MC_REP_CTRL;

void mccReqLvlSet(u_int lvl);
u_int mccReqLvlGet(void);

void mccReqVSOTHSAVEset(VSOTHSAVE *sv);
int mccReqVSOTHSAVEget(VSOTHSAVE *sv);

#endif