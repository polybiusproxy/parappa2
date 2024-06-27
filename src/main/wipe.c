#include "main/wipe.h"

#include "main/sprite.h"
#include "main/cmnfile.h"
#include "main/scrctrl.h"

#include "os/mtc.h"

#include "iop_mdl/tapctrl_rpc.h"
#include "os/syssub.h"
#include "prlib/prlib.h"

/* data */
extern SNDTAP sndtap_wipe[];
extern LDMAP ldmap[];

/* sdata - static */
WIPE_TYPE wipe_type;
int wipe_end_flag;
int loading_wipe_switch;

int ldmove_rate;
int ldrecode_rate;
int ldlogo_rate;

int wipe_para_spa_type;
VCLR_PARA vclr_para_disp;

/* sbss - static */
PR_SCENEHANDLE ldmap_hdl;

void wipeSndReq(SNDTAP_WIPE_ENUM req)
{
    SNDTAP *sndtap_pp;

    if (req >= STW_MAX)
    {
        printf("WIPE SND REQ OVER!![%d]\n", req);
        return;
    }

    sndtap_pp = &sndtap_wipe[req];

    TapCt(0xb0, 0, 0);
    TapCt(0xf3, 0xf, sndtap_pp->volume);
    TapCt(0xd3, 0xf, sndtap_pp->prg + sndtap_pp->key * 256);
}

void wipeSndStop(void)
{
    TapCt(0xe0, 0xf, 0);
}

void wipeSndFileTrans(void)
{
    TapCt(0x8033, (int)cmnfGetFileAdrs(74), cmnfGetFileSize(74));
    TapCt(0x8043, (int)cmnfGetFileAdrs(73), cmnfGetFileSize(73));

    TapCt(0x90, PR_CONCAT(0x3fff, 0x3fff), 0);
    TapCt(0xa0, PR_CONCAT(0x3fff, 0x3fff), 0);
    TapCt(0xb0, 0, 0x1fff);
    TapCt(0x80, PR_CONCAT(0x3fff, 0x3fff), 0);
}

static void LocalBufCopy(int disp)
{
    sceGsMoveImage mi;
    short          dbp = 0;
    short          sbp = 0;

    switch (disp)
    {
    case 0:
        sbp = DrawGetTbpPos(DNUM_SHOW);
        dbp = DrawGetTbpPos(DNUM_VRAM2);
        break;
    case 1:
        sbp = DrawGetTbpPos(DNUM_DRAW);
        dbp = DrawGetTbpPos(DNUM_VRAM2);
        break;
    case -1:
        sbp = DrawGetTbpPos(DNUM_VRAM2);
        dbp = DrawGetTbpPos(DNUM_SHOW);
        break;
    case -2:
        sbp = DrawGetTbpPos(DNUM_VRAM2);
        dbp = DrawGetTbpPos(DNUM_DRAW);
        break;
    default:
        printf(" local copy error!!\n");
        break;
    }

    GGsSetLocalMoveImage(&mi, dbp, 10, 0, 0, 0, sbp, 10, 0, 0, 0, 640, 224, 0);
    FlushCache(0);

    GGsExecLocalMoveImage(&mi);
    sceGsSyncPath(0, 0);
}

INCLUDE_ASM(const s32, "main/wipe", wipeTimeGetInWait);
int wipeTimeGetInWait(/* a0 4 */ int time, /* a1 5 */ WSHC_ENUM wshc_enum);

#if 1
INCLUDE_ASM(const s32, "main/wipe", lddisp_init_pr);
static void lddisp_init_pr(void);
#else /* Matching */
static void lddisp_init_pr(void)
{
    LDMAP *ldmap_pp;
    u_int  i;

    PrSetFrameRate(60.0f);
    ldmap_hdl = PrInitializeScene(&DBufDc.draw01, "wipe recode", -1);

    ldmap_pp = ldmap;
    for (i = 0; i < 5; i++, ldmap_pp++)
    {
        ldmap_pp->spmHdl = PrInitializeModel(cmnfGetFileAdrs(ldmap_pp->spmmap), ldmap_hdl);

        /* Initialize animation */
        if (ldmap_pp->spamap >= 0)
        {
            ldmap_pp->spaHdl = PrInitializeAnimation(cmnfGetFileAdrs(ldmap_pp->spamap));

            PrLinkAnimation(ldmap_pp->spmHdl, ldmap_pp->spaHdl);
            PrAnimateModel(ldmap_pp->spmHdl, *ldmap_pp->frame_pp);
        }

        /* Initialize position animation */
        if (ldmap_pp->spamapP >= 0)
        {
            ldmap_pp->spaHdlP = PrInitializeAnimation(cmnfGetFileAdrs(ldmap_pp->spamapP));

            PrLinkPositionAnimation(ldmap_pp->spmHdl, ldmap_pp->spaHdlP);
            PrAnimateModelPosition(ldmap_pp->spmHdl, *ldmap_pp->frame_ppP);
        }

        PrHideModel(ldmap_pp->spmHdl);
    }

    PrPreprocessSceneModel(ldmap_hdl);
}
#endif

INCLUDE_ASM(const s32, "main/wipe", lddisp_draw_quit);
static void lddisp_draw_quit(/* s0 16 */ int drapP);

static void lddisp_draw_on(LDMAP_ENUM ldmap_enum)
{
    PrShowModel(ldmap[ldmap_enum].spmHdl, NULL);
}

void WipeLoadInDisp(void *x)
{
    int timer;
    int firstf;
    int ttmp;

    firstf = 1;

    wipeSndReq(STW_TURN_IN);
    TimeCallbackTimeSetChan(TCBK_CHANNEL_WIPE, 0);

    while (1)
    {
        timer = TimeCallbackTimeGetChan(TCBK_CHANNEL_WIPE);

        if (timer >= 67)
        {
            ldmove_rate   = wipeTimeGetInWait(timer - 67, WSHC_LOOP_MOVE);
            ldrecode_rate = wipeTimeGetInWait(timer - 67, timer >= 67);
        }
        else
        {
            ldmove_rate   = wipeTimeGetInWait(timer, WSHC_IN_MOVE);
            ldrecode_rate = wipeTimeGetInWait(timer, timer >= 67);
        }

        if (!loading_wipe_switch)
            ldmove_rate = 480 - ldmove_rate;

        ldlogo_rate = timer % 360;

        lddisp_init_pr();
        lddisp_draw_on(LDMAP_TURN);
        lddisp_draw_on(LDMAP_LOGO);
        lddisp_draw_on(LDMAP_RECODE);
        lddisp_draw_on(LDMAP_RECODE_LT);
        lddisp_draw_on(LDMAP_LABEL);
        lddisp_draw_quit(DNUM_DRAW);

        wipe_end_flag = 0;
        timer++;

        if (timer > 67)
        {
            if (firstf)
            {
                firstf = 0;

                wipeSndReq(STW_TURN_WAIT);
                wipe_end_flag = 1;

                TimeCallbackTimeSetChan(TCBK_CHANNEL_WIPE, 67);
            }
            else
            {
                ttmp = (timer - 67) % 135;

                if (ttmp == 134 || ttmp == 0)
                {
                    wipe_end_flag = 1;
                }
            }
        }

        MtcWait(1);
    }
}

INCLUDE_ASM(const s32, "main/wipe", WipeLoadInDispNR);

INCLUDE_ASM(const s32, "main/wipe", WipeLoadOutDispNR);

INCLUDE_ASM(const s32, "main/wipe", WipeLoadOutDisp);

void WipeInReq(void)
{
    loading_wipe_switch ^= 1;
    wipe_end_flag = 0;
    wipe_type = WIPE_TYPE_LOADING;

    LocalBufCopy(0);
    MtcKill(MTC_TASK_WIPECTRL);

    WipeLoadInDispNR();
    MtcExec(WipeLoadInDisp, MTC_TASK_WIPECTRL);
}

INCLUDE_RODATA(const s32, "main/wipe", D_00393680);

INCLUDE_ASM(const s32, "main/wipe", WipeLoadInDispSame);
void WipeLoadInDispSame(void *x);

void WipeInReqSame(void)
{
    wipe_type = WIPE_TYPE_SAME;
    wipe_end_flag = 0;

    MtcExec(WipeLoadInDispSame, MTC_TASK_WIPECTRL);
    LocalBufCopy(0);
}

INCLUDE_ASM(const s32, "main/wipe", WipeOutReq);

int WipeEndCheck(void)
{
    return wipe_end_flag;
}

INCLUDE_ASM(const s32, "main/wipe", WipeYesNoDispTask);
void WipeYesNoDispTask(void *x);

void wipeYesNoDispReq(void)
{
    printf("wipe yes/no req\n");

    wipe_type = WIPE_TYPE_YES_NO;
    wipe_end_flag = 1;
    wipeSndReq(STW_YESNO);

    MtcExec(WipeYesNoDispTask, MTC_TASK_WIPECTRL);
    LocalBufCopy(0);
}

void wipeYesNoDispEnd(void)
{
    MtcKill(MTC_TASK_WIPECTRL);
}

INCLUDE_ASM(const s32, "main/wipe", WipeInitPrDataPara);

INCLUDE_ASM(const s32, "main/wipe", WipeQuitPrDataPara);

INCLUDE_ASM(const s32, "main/wipe", WipeDispPrDataPara);

void WipeParaColorSet(u_char r, u_char g, u_char b)
{
    vclr_para_disp.r = r;
    vclr_para_disp.g = g;
    vclr_para_disp.b = b;
}

void WipeEnd(void)
{
    wipe_end_flag = 1;
  
    MtcKill(MTC_TASK_WIPECTRL);
    wipeSndStop();
}

INCLUDE_ASM(const s32, "main/wipe", WipeParaInDisp);
void WipeParaInDisp(void *x);

INCLUDE_ASM(const s32, "main/wipe", WipeParaInDispMove);
void WipeParaInDispMove(void *x);

INCLUDE_ASM(const s32, "main/wipe", WipeParaOutDisp);
void WipeParaOutDisp(void *x);

void wipeParaInReq(void)
{
    wipe_end_flag = 0;
    wipe_type = WIPE_TYPE_PARA;
    wipeSndReq(STW_PARAtoMINI);

    wipe_para_spa_type = 0;
    MtcExec(WipeParaInDisp, MTC_TASK_WIPECTRL);

    LocalBufCopy(0);
}

/* Used in the demos - unused on the final version */
void wipeParaInReqByeBye(void)
{
    wipe_end_flag = 0;
    wipe_type = WIPE_TYPE_PARA;
    wipeSndReq(STW_PARAtoMINI);

    wipe_para_spa_type = 1;
    MtcExec(WipeParaInDisp, MTC_TASK_WIPECTRL);

    LocalBufCopy(0);
}

void wipeParaInReqMove(void)
{
    wipe_end_flag = 0;
    wipe_type = WIPE_TYPE_PARA;
    wipeSndReq(STW_PARAtoMINI);

    wipe_para_spa_type = 0;
    MtcExec(WipeParaInDispMove, MTC_TASK_WIPECTRL);
}

void wipeParaOutReq(void)
{
    wipe_end_flag = 0;
    wipeSndReq(STW_PARAtoBIG);

    MtcExec(WipeParaOutDisp, MTC_TASK_WIPECTRL);
}

INCLUDE_RODATA(const s32, "main/wipe", D_003936B8);

INCLUDE_RODATA(const s32, "main/wipe", D_003936C8);

INCLUDE_RODATA(const s32, "main/wipe", D_003936E0);

INCLUDE_RODATA(const s32, "main/wipe", D_00393700);

INCLUDE_ASM(const s32, "main/wipe", WipeBoxyInDisp);
void WipeBoxyInDisp(void *x);

void wipeBoxyInReq(void)
{
    wipe_end_flag = 0;
    wipe_type = WIPE_TYPE_BOXY;
    wipeSndReq(STW_BOXY_IN);

    LocalBufCopy(0);
    MtcExec(WipeBoxyInDisp, MTC_TASK_WIPECTRL);
}

INCLUDE_ASM(const s32, "main/wipe", WipeBoxyWaitDisp);
void WipeBoxyWaitDisp(void *x);

void wipeBoxyWaitReq(void)
{
    wipe_end_flag = 0;
    wipe_type = WIPE_TYPE_BOXY_WAIT;
    wipeSndReq(STW_BOXY_WAIT);

    MtcExec(WipeBoxyWaitDisp, MTC_TASK_WIPECTRL);
}
