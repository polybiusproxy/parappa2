#ifndef MENU_H
#define MENU_H

#include "common.h"

#include "main/etc.h"
#include "main/mcctrl.h"

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

typedef struct { // 0x188
    /* 0x000 */ int nRound;
    /* 0x004 */ char name[12];
    /* 0x010 */ char name1[12];
    /* 0x01c */ char name2[12];
    /* 0x028 */ u_int clrFlg[4];
    /* 0x038 */ int clrCount[8];
    /* 0x058 */ int clrVSCOM1[8];
    /* 0x078 */ int clrCOOL[8];
    /* 0x098 */ u_int logCOOL[8];
    /* 0x0b8 */ GAME_STATUS game_status;
} P3LOG_VAL;

typedef struct { // 0x58
    /* 0x00 */ int nStage;
    /* 0x04 */ int nMode;
    /* 0x08 */ int vsLev;
    /* 0x0c */ P3LOG_VAL *pLog;
    /* 0x10 */ GAME_STATUS *pGameStatus;
    /* 0x14 */ MC_REP_STR *pReplayArea;
    /* 0x18 */ int endFlg;
    /* 0x1c */ u_int score;
    /* 0x20 */ u_int score2P;
    /* 0x24 */ u_int bonusG;
    /* 0x28 */ int bCoolClr;
    /* 0x2c */ int winPlayer;
    /* 0x30 */ int endingGame;
    /* 0x34 */ int isWipeEnd;
    /* 0x38 */ int isState;
    /* 0x3c */ short *pAutoMove;
    /* 0x40 */ short autoMovePos[10];
    /* 0x54 */ int curRecJacket;
} P3GAMESTATE;

int MenuMemCardCheck(void);

#endif