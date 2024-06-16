#include "main/mcctrl.h"
#include "os/syssub.h"

/* bss - static */
extern MC_REP_STR mc_rep_str_local;

/* sbss - static */
u_char mc_holdTmp[4];
u_char mc_resetTmp[4];

INCLUDE_ASM(const s32, "main/mcctrl", setAscii2SjisCode);

void mccReqInit(void)
{
    WorkClear(&mc_rep_str_local, 0x4528);
    WorkClear(mc_holdTmp, sizeof(mc_holdTmp));
    WorkClear(mc_resetTmp, sizeof(mc_resetTmp));
}

INCLUDE_ASM(const s32, "main/mcctrl", mccReqSaveInit);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqCtrlClr);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqScrSet);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqScrGet);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqLvlSet);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqLvlGet);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqTapSet);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqTapResetSet);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqTapHoldSet);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqVSOTHSAVEset);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqVSOTHSAVEget);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqTapGet);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqTapForward);

INCLUDE_ASM(const s32, "main/mcctrl", mccReqTapForwardOwn);

INCLUDE_ASM(const s32, "main/mcctrl", mccLocalGlobalCopy);

INCLUDE_ASM(const s32, "main/mcctrl", mccGlobalLocalCopy);

INCLUDE_RODATA(const s32, "main/mcctrl", D_00393800);
