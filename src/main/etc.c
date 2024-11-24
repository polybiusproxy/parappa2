#include "common.h"

INCLUDE_ASM("main/etc", GlobalInit);

INCLUDE_ASM("main/etc", clearStageCheck);

INCLUDE_ASM("main/etc", GlobalTimeInit);

INCLUDE_ASM("main/etc", GlobalSetTempo);

INCLUDE_ASM("main/etc", GlobalTimeJobChange);

INCLUDE_ASM("main/etc", GlobalTimeJob);

INCLUDE_ASM("main/etc", GlobalTimeGet);

INCLUDE_ASM("main/etc", GlobalSndTimeGet);

INCLUDE_ASM("main/etc", GlobalSndSampleGet);

INCLUDE_ASM("main/etc", GetHatRound);

INCLUDE_ASM("main/etc", GlobalMendererUseCheck);

INCLUDE_ASM("main/etc", GlobalLobcalCopy);

INCLUDE_ASM("main/etc", GlobalPlySet);

INCLUDE_ASM("main/etc", GetPcode2PadType);

INCLUDE_ASM("main/etc", TimeCallback);

INCLUDE_ASM("main/etc", TimeCallbackSet);

INCLUDE_ASM("main/etc", TimeCallbackTimeGetChan);

INCLUDE_ASM("main/etc", TimeCallbackTimeSetChan);

INCLUDE_ASM("main/etc", TimeCallbackTimeSetChanTempo);

INCLUDE_ASM("main/etc", TimeCallbackTimeGet);

INCLUDE_ASM("main/etc", TimeCallbackTimeSet);

INCLUDE_ASM("main/etc", Pcode2Pindex);

INCLUDE_ASM("main/etc", GetKeyCode2Index);

INCLUDE_ASM("main/etc", GetIndex2KeyCode);

INCLUDE_ASM("main/etc", GetIndex2PressId);

INCLUDE_ASM("main/etc", GetKeyCode2PressId);

INCLUDE_ASM("main/etc", ChangeTapLevel);

void UsrPrInitScene(void)
{
    // Empty
}

void UsrPrQuitScene(void)
{
    // Empty
}

INCLUDE_RODATA("main/etc", D_003935B8);

INCLUDE_ASM("main/etc", UsrPrSetScene);

INCLUDE_ASM("main/etc", SpuBankSet);

INCLUDE_ASM("main/etc", SpuBankSetAll);

INCLUDE_ASM("main/etc", inCmnInit);

INCLUDE_ASM("main/etc", inCmnHookMaxLineCnt);

INCLUDE_ASM("main/etc", inCmnHookMaxLinePknum);

INCLUDE_ASM("main/etc", inCmnHookSet);

INCLUDE_ASM("main/etc", inCmnHook2GameCheck);

INCLUDE_ASM("main/etc", inCmnHook2GameSave);
