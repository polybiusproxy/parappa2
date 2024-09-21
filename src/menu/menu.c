#include "menu/menu.h"

#include "os/mtc.h"
#include "os/system.h"

#include "main/wipe.h"

#include "menu/menusub.h"
#include "menu/menudata.h"

#include <prlib/prlib.h>

#include <string.h>

/* bss - static */
extern P3LOG_VAL P3Log_Val;

static int bFirst;
static int VsLev;

static void P3LogInit(P3LOG_VAL *plog);

static void menuDraw(void *x)
{
    while (1)
    {
        TsMenu_Draw();
        MtcWait(1);
    }
}

static void menuDrawReq(void)
{
    PrSetFrameRate(60.0f);
    MtcExec(menuDraw, MTC_TASK_07);
}

static void menuDrawQuit(void)
{
    MtcKill(MTC_TASK_07);
}

int MenuMemCardCheck(void)
{
    bFirst = 1;
    VsLev = 0;

    P3LogInit(&P3Log_Val);

    TsMENU_InitSystem();
    TsMenu_RankingClear();
    TsMenu_Init(0, NULL);

    // Start menu rendering
    menuDrawReq();
    MenuMsgInit();

    while (!TsMenuMemcChk_Flow())
        MtcWait(1);

    TsMenu_End();
    menuDrawQuit();
    return 0;
}

#if 1
INCLUDE_ASM("menu/menu", MenuCtrl);
#else
int MenuCtrl(/* s0 16 */ MENU_STR *menu_str_ptr)
{
    int         ret;

    P3GAMESTATE P3GameState;
    GAME_STATUS *pGStatus = P3GameState.pGameStatus;

    MenuDataSndInit();

    P3GameState.vsLev = VsLev;
    P3GameState.pLog = &P3Log_Val;
    P3GameState.pGameStatus = menu_str_ptr->game_status_p;
    P3GameState.pReplayArea = menu_str_ptr->mc_rep_str_p;
    P3GameState.endFlg = menu_str_ptr->sel_menu_enum;

    if (bFirst)
    {
        P3Log_Val.nRound = 0;
        P3Log_Val.clrFlg[0] = 0;

        P3GameState.nStage = 0;
        P3GameState.endFlg = 0;
    }
    else
    {
        P3GameState.nStage = pGStatus->play_stageG;
    }

    bFirst = 0;

    P3GameState.nMode = pGStatus->play_modeG;
    P3GameState.bCoolClr = (pGStatus->disp_level == DLVL_COOL);
    P3GameState.bonusG = pGStatus->bonusG;
    P3GameState.score = pGStatus->scoreG[0];
    P3GameState.score2P = /**(u_int*)*/pGStatus->scoreG[1];

    if (global_data.global_ply[0].vsWin == global_data.global_ply[0].vsLost)
    {
        P3GameState.winPlayer = 2;
    }
    else if (global_data.global_ply[0].vsWin < global_data.global_ply[0].vsLost)
    {
        P3GameState.winPlayer = 1;
    }
    else if (global_data.global_ply[0].vsWin >= global_data.global_ply[0].vsLost)
    {
        P3GameState.winPlayer = 0;
    }

    P3GameState.isState = 0;
    TsMenu_Init(1, &P3GameState);

    menuDrawReq();
    MenuMsgInit();

    TsMenu_InitFlow(&P3GameState);
    while (1)
    {
        P3GameState.isWipeEnd = WipeEndCheck();

        ret = TsMenu_Flow();
        if (ret)
            break;

        MtcWait(1);
    }

    MenuDataSndQuit();
    menuDrawQuit();
    TsMenu_End();

    pGStatus->play_stageG = P3GameState.nStage;
    pGStatus->play_modeG  = P3GameState.nMode;

    if (P3GameState.nMode != 0)
    {
        VsLev = P3GameState.vsLev;
    }
    else
    {
        pGStatus->level_vs_enumG = LVS_1;
        VsLev = 0;
    }

    if (ret == 2)
    {
        pGStatus->endingFlag = P3GameState.endingGame;
    }
    else
    {
        pGStatus->endingFlag = 0;
    }

    pGStatus->stClearBit = P3GameState.pLog->clrFlg[0];

    if (ret == 3)
    {
        pGStatus->demo_flagG = DEMOF_REPLAY;
    }
    else
    {
        pGStatus->demo_flagG = DEMOF_OFF;
    }

    return (ret == 4);
}
#endif

static void P3LogInit(P3LOG_VAL *plog)
{
    memset(plog, 0, sizeof(P3LOG_VAL));
}
