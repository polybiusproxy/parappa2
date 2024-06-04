#ifndef DRAWCTRL_H
#define DRAWCTRL_H

#include "common.h"

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