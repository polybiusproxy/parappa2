#include "main/wipe.h"

#include "main/sprite.h"
#include "main/scrctrl.h"

#include "iop_mdl/tapctrl_rpc.h"
#include <prlib/prlib.h>

/* data */
extern SNDTAP sndtap_wipe[];

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

INCLUDE_ASM(const s32, "main/wipe", wipeSndStop);

INCLUDE_ASM(const s32, "main/wipe", wipeSndFileTrans);

INCLUDE_ASM(const s32, "main/wipe", LocalBufCopy);

INCLUDE_ASM(const s32, "main/wipe", wipeTimeGetInWait);

INCLUDE_ASM(const s32, "main/wipe", lddisp_init_pr);

INCLUDE_ASM(const s32, "main/wipe", lddisp_draw_quit);

INCLUDE_ASM(const s32, "main/wipe", lddisp_draw_on);

INCLUDE_ASM(const s32, "main/wipe", WipeLoadInDisp);

INCLUDE_ASM(const s32, "main/wipe", WipeLoadInDispNR);

INCLUDE_ASM(const s32, "main/wipe", WipeLoadOutDispNR);

INCLUDE_ASM(const s32, "main/wipe", WipeLoadOutDisp);

INCLUDE_ASM(const s32, "main/wipe", WipeInReq);

INCLUDE_ASM(const s32, "main/wipe", WipeLoadInDispSame);

INCLUDE_ASM(const s32, "main/wipe", WipeInReqSame);

INCLUDE_ASM(const s32, "main/wipe", WipeOutReq);

INCLUDE_ASM(const s32, "main/wipe", WipeEndCheck);

INCLUDE_ASM(const s32, "main/wipe", WipeYesNoDispTask);

INCLUDE_RODATA(const s32, "main/wipe", D_00393680);

INCLUDE_ASM(const s32, "main/wipe", wipeYesNoDispReq);

INCLUDE_ASM(const s32, "main/wipe", wipeYesNoDispEnd);

INCLUDE_ASM(const s32, "main/wipe", WipeInitPrDataPara);

INCLUDE_ASM(const s32, "main/wipe", WipeQuitPrDataPara);

INCLUDE_ASM(const s32, "main/wipe", WipeDispPrDataPara);

INCLUDE_ASM(const s32, "main/wipe", WipeParaColorSet);

INCLUDE_ASM(const s32, "main/wipe", WipeEnd);

INCLUDE_ASM(const s32, "main/wipe", WipeParaInDisp);

INCLUDE_ASM(const s32, "main/wipe", WipeParaInDispMove);

INCLUDE_ASM(const s32, "main/wipe", WipeParaOutDisp);

INCLUDE_ASM(const s32, "main/wipe", wipeParaInReq);

INCLUDE_ASM(const s32, "main/wipe", wipeParaInReqByeBye);

INCLUDE_ASM(const s32, "main/wipe", wipeParaInReqMove);

INCLUDE_ASM(const s32, "main/wipe", wipeParaOutReq);

INCLUDE_RODATA(const s32, "main/wipe", D_003936B8);

INCLUDE_RODATA(const s32, "main/wipe", D_003936C8);

INCLUDE_RODATA(const s32, "main/wipe", D_003936E0);

INCLUDE_RODATA(const s32, "main/wipe", D_00393700);

INCLUDE_ASM(const s32, "main/wipe", WipeBoxyInDisp);

INCLUDE_ASM(const s32, "main/wipe", wipeBoxyInReq);

INCLUDE_ASM(const s32, "main/wipe", WipeBoxyWaitDisp);

INCLUDE_ASM(const s32, "main/wipe", wipeBoxyWaitReq);
