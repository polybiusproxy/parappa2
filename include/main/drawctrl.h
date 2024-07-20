#ifndef DRAWCTRL_H
#define DRAWCTRL_H

#include "common.h"

#include <eetypes.h>

typedef struct { // 0x20
    /* 0x00 */ u_int start_flame;
    /* 0x04 */ u_int end_flame;
    /* 0x08 */ u_int useDisp;
    /* 0x0c */ u_int drDisp;
    /* 0x10 */ u_char pri;
    /* 0x11 */ u_char use_flag;
    /* 0x14 */ int (*prg_pp)(void *para_pp, int frame, int first_f, int useDisp, int drDisp);
    /* 0x18 */ void *param_pp;
    /* 0x1c */ u_int condition;
} SCENECTRL;

typedef struct { // 0x8
    /* 0x0 */ int scenectrl_num;
    /* 0x4 */ SCENECTRL *scenectrl_pp;
} SCENESTR;

typedef struct { // 0x8
    /* 0x0 */ int scenestr_size;
    /* 0x4 */ SCENESTR *scenestr_pp;
} EVENTREC;

int DrawFadeDisp(void *para_pp, int frame, int first_f, int useDisp, int drDisp);
int DrawMoveDispIn(void *para_pp, int frame, int first_f, int useDisp, int drDisp);

int DrawVramClear(void *para_pp, int frame, int first_f, int useDisp, int drDisp);
void DrawCtrlTblChange(int ctrlTbl);
int DrawTapReqTbl(int atap, int pindx, u_char *prs_pp);

void Cl2MixTrans(int now_T, int max_T, u_char *cl2_0_pp, u_char *cl2_1_pp);

void DrawCtrlInit(EVENTREC *ev_pp, int ctrlTbl, void *dat_top);
void DrawCtrlQuit(void);
void DrawCtrlTimeSet(int time);

#endif