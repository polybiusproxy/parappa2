#ifndef MENUSUB_H
#define MENUSUB_H

#include "menu/menu.h"

void TsMENU_InitSystem(void);
void TsMENU_EndSystem(void);
void TsMenu_RankingClear(void);

void TsMenu_Init(int iniflg, P3GAMESTATE *pstate);
void TsMenu_End(void);

void TsMenu_InitFlow(P3GAMESTATE *pstate);
int  TsMenuMemcChk_Flow(void);
int  TsMenu_Flow(void);

void TsMenu_Draw(void);

#endif