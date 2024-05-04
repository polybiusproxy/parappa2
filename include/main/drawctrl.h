#ifndef DRAWCTRL_H
#define DRAWCTRL_H

#include "common.h"

int DrawVramClear(void *para_pp, int frame, int first_f, int useDisp, int drDisp);
void DrawCtrlTblChange(int ctrlTbl);
int DrawTapReqTbl(int atap, int pindx, u_char *prs_pp);

#endif