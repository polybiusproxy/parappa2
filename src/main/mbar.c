#include "main/mbar.h"

#include "main/scrctrl.h"

/* .bss */
extern MBAR_REQ_STR mbar_req_str[5];

/* .sbss */
extern int mbar_ctrl_time;
extern int mbar_ctrl_stage;
extern int mbar_ctrl_stage_selT;

INCLUDE_ASM("main/mbar", examCharSet);

INCLUDE_ASM("main/mbar", clrColorBuffer);

INCLUDE_ASM("main/mbar", MbarMemberClear);

INCLUDE_ASM("main/mbar", examCharBasic);

INCLUDE_ASM("main/mbar", examCharScaleSet);

INCLUDE_ASM("main/mbar", examCharCltSet);

INCLUDE_ASM("main/mbar", examCharPosSet);

INCLUDE_ASM("main/mbar", examCharUVWHSet);

INCLUDE_ASM("main/mbar", examCharAlphaSet);

INCLUDE_ASM("main/mbar", examCharKidoSet);

INCLUDE_ASM("main/mbar", NikoReset);

INCLUDE_ASM("main/mbar", MbarNikoHookUse);

INCLUDE_ASM("main/mbar", MbarNikoVsUse);

INCLUDE_ASM("main/mbar", MbarNikoUnUse);

INCLUDE_ASM("main/mbar", MbarNikoSet);

INCLUDE_ASM("main/mbar", MbarNikoDisp);

INCLUDE_ASM("main/mbar", MbarHookUseInit);

INCLUDE_ASM("main/mbar", MbarHookUnUse);

INCLUDE_ASM("main/mbar", MbarHookUseOK);

INCLUDE_ASM("main/mbar", MbarHookUseNG);

INCLUDE_ASM("main/mbar", MbarHookPoll);

INCLUDE_ASM("main/mbar", vsAnimationInit);

INCLUDE_ASM("main/mbar", vsAnimationReq);

INCLUDE_ASM("main/mbar", vsAnimationReset);

INCLUDE_ASM("main/mbar", vsScr2Move);

INCLUDE_ASM("main/mbar", vsAnimationPoll);

INCLUDE_ASM("main/mbar", metColorInit);

INCLUDE_ASM("main/mbar", metColorSet);

INCLUDE_ASM("main/mbar", metFrameInit);

INCLUDE_ASM("main/mbar", conditionFrameInit);

INCLUDE_ASM("main/mbar", ExamDispInit);

INCLUDE_ASM("main/mbar", ExamDispPlySet);

INCLUDE_ASM("main/mbar", ExamDispReq);

INCLUDE_ASM("main/mbar", ExamDispReset);

INCLUDE_ASM("main/mbar", examScore2Level);

INCLUDE_ASM("main/mbar", ExamDispOn);

INCLUDE_ASM("main/mbar", hex2dec_mbar_tmp);

INCLUDE_ASM("main/mbar", hex2decPlMi);

INCLUDE_ASM("main/mbar", examNumDisp);

INCLUDE_ASM("main/mbar", examScoreSet);

INCLUDE_ASM("main/mbar", examLevelDisp);

INCLUDE_ASM("main/mbar", ExamDispSet);

void ExamDispSubt(void)
{
    // Empty
}

void MbarInit(int stg)
{
    mbar_ctrl_stage = stg;

    if (stg == 6)
        mbar_ctrl_stage_selT = 1;
    else
        mbar_ctrl_stage_selT = stg;
    
    MbarCharSetSub();
}

INCLUDE_ASM("main/mbar", MbarReset);

INCLUDE_RODATA("main/mbar", D_00393440);

INCLUDE_RODATA("main/mbar", D_00393448);

INCLUDE_RODATA("main/mbar", D_00393450);

INCLUDE_RODATA("main/mbar", D_00393458);

void MbarReq(MBAR_REQ_ENUM mm_req, TAPSET *ts_pp, int curr_time, SCR_TAP_MEMORY *tm_pp, int tm_cnt, 
            int lang, int tapdat_size, TAPDAT *tapdat_pp, GUI_CURSOR_ENUM guic)
{
    PLAYER_INDEX pidx;

    if (ts_pp == NULL)
    {
        printf("MbarReq   TAPSET adrs is NULL\n");
        return;
    }

    pidx = Pcode2Pindex(ts_pp->player_code);
    mbar_req_str[pidx].mbar_req_enum = mm_req;
    mbar_req_str[pidx].tapset_pp = ts_pp;
    mbar_req_str[pidx].current_time = curr_time;
    mbar_req_str[pidx].scr_tap_memory_pp = tm_pp;
    mbar_req_str[pidx].scr_tap_memory_cnt = tm_cnt;
    mbar_req_str[pidx].lang = lang;

    mbar_req_str[pidx].tapdat_size = tapdat_size;
    mbar_req_str[pidx].tapdat_pp = tapdat_pp;
    mbar_req_str[pidx].gui_cursor_enum = guic;
}

void MbarSetCtrlTime(int mctime)
{
    mbar_ctrl_time = mctime;
}

INCLUDE_ASM("main/mbar", MbarCl1CharSet);

INCLUDE_ASM("main/mbar", MbarCharSetSub);

INCLUDE_ASM("main/mbar", MbarGifInit);

INCLUDE_ASM("main/mbar", MbarGifTrans);

INCLUDE_ASM("main/mbar", MbarCharSet);

INCLUDE_ASM("main/mbar", MbarCharSet2);

INCLUDE_ASM("main/mbar", MbarWindowSet);

INCLUDE_ASM("main/mbar", MbarGetDispPosX);

INCLUDE_ASM("main/mbar", MbarGetDispPosY);

INCLUDE_ASM("main/mbar", MbarGetTimeArea);

INCLUDE_ASM("main/mbar", MbarGetTimeArea2);

INCLUDE_ASM("main/mbar", MbarGetStartTime);

INCLUDE_ASM("main/mbar", MbarGetEndTime);

INCLUDE_ASM("main/mbar", MbarGetStartTap);

INCLUDE_ASM("main/mbar", MbarSclRotMake);

void MbarGuideLightMake(MBARR_CHR *mbarr_pp, int mbtime)
{
    u_char col = 128;

    if ((u_int)mbtime < 144)
    {
        col = (u_char)(((144 - mbtime) * 128) / 144) + 128;
    }

    mbarr_pp->r = mbarr_pp->g = mbarr_pp->b = col;
}

INCLUDE_ASM("main/mbar", MbarFlashMake);

INCLUDE_ASM("main/mbar", MbarBackSet);

INCLUDE_RODATA("main/mbar", D_00393488);

INCLUDE_RODATA("main/mbar", D_003934A0);

INCLUDE_ASM("main/mbar", MbarOthSet);

INCLUDE_ASM("main/mbar", MbarCurSet);

INCLUDE_ASM("main/mbar", MbarTapSubt);

INCLUDE_ASM("main/mbar", MbarPosOffsetSet);

INCLUDE_ASM("main/mbar", mbar_othon_frame_set);

INCLUDE_ASM("main/mbar", MbarDisp);

INCLUDE_ASM("main/mbar", MbarDispScene);

INCLUDE_ASM("main/mbar", MbarDispSceneDraw);

INCLUDE_ASM("main/mbar", MbarDispSceneVsDraw);

INCLUDE_ASM("main/mbar", MbarDispSceneVsDrawInit);

INCLUDE_ASM("main/mbar", guidisp_init_pr);

INCLUDE_ASM("main/mbar", guidisp_draw_quit);

INCLUDE_ASM("main/mbar", MbarDispGuiScene);

INCLUDE_ASM("main/mbar", MbarDispGuiSceneMbarArea);

INCLUDE_ASM("main/mbar", lessonTim2InfoGet);

INCLUDE_ASM("main/mbar", lessonCl2InfoGet);

INCLUDE_ASM("main/mbar", MbarDemoCharDisp);

INCLUDE_RODATA("main/mbar", D_003934E8);

INCLUDE_RODATA("main/mbar", D_00393500);
