#include "common.h"

#include <eetypes.h>

#include "main/sprite.h"

/* sdata - static */
VCLR_PARA vclr_black_tmp;

/* sbss - static */
int scenectrl_outside_cnt;
int scenectrl_outside_read_cnt;

u_int useDispFlag;
u_int drDispFlag;

INCLUDE_ASM(const s32, "main/drawctrl", UseGsRegSet);

INCLUDE_ASM(const s32, "main/drawctrl", UseGsSetXyOffset);

INCLUDE_ASM(const s32, "main/drawctrl", outsideDrawSceneClear);

INCLUDE_ASM(const s32, "main/drawctrl", outsideDrawSceneReq);

INCLUDE_ASM(const s32, "main/drawctrl", getOutsideCtrlScene);

INCLUDE_ASM(const s32, "main/drawctrl", bra_tap_GetNext);

INCLUDE_ASM(const s32, "main/drawctrl", bra_title_GetNext);

INCLUDE_ASM(const s32, "main/drawctrl", bra_ret_GetNext);

INCLUDE_ASM(const s32, "main/drawctrl", XAnimationLinkOption);

INCLUDE_ASM(const s32, "main/drawctrl", XAnimationPositionLink);

INCLUDE_ASM(const s32, "main/drawctrl", XAnimationPositionUnLink);

INCLUDE_ASM(const s32, "main/drawctrl", BallThrowSetFrame);

INCLUDE_ASM(const s32, "main/drawctrl", BallThrowInit);

INCLUDE_ASM(const s32, "main/drawctrl", BallThrowInitDare);

INCLUDE_ASM(const s32, "main/drawctrl", vs06BomAdr);

INCLUDE_ASM(const s32, "main/drawctrl", BallThrowReq);

INCLUDE_ASM(const s32, "main/drawctrl", BallThrowTarget);

INCLUDE_ASM(const s32, "main/drawctrl", BallThrowPoll);

INCLUDE_ASM(const s32, "main/drawctrl", BallThrowPollScene);

INCLUDE_ASM(const s32, "main/drawctrl", MozaikuPollSceneInit);

INCLUDE_ASM(const s32, "main/drawctrl", MozaikuPollSceneReq);

INCLUDE_ASM(const s32, "main/drawctrl", MozaikuPollScene);

INCLUDE_ASM(const s32, "main/drawctrl", DrawTmpBuffInit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawTmpBuffQuit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawTmpBuffGetArea);

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393128);

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393140);

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393158);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjdatInit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjdatReset);

INCLUDE_ASM(const s32, "main/drawctrl", DrawSceneInit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjCtrlInit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrInit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjTapStrInit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrReq);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrTapReq);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjTapStrTapReq);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrTapQuit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjTapStrTapQuit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrKill);

INCLUDE_ASM(const s32, "main/drawctrl", GetSpfTimeCtrl);

INCLUDE_ASM(const s32, "main/drawctrl", camOtherKill);

INCLUDE_ASM(const s32, "main/drawctrl", posAniOtherKill);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrDispTap);

INCLUDE_ASM(const s32, "main/drawctrl", DrawSceneReset);

INCLUDE_ASM(const s32, "main/drawctrl", DrawSceneFirstSet);

INCLUDE_ASM(const s32, "main/drawctrl", Cl2MixTrans);

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393290);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjPrReq);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrTapTimeNext);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjTapCtrl);

INCLUDE_ASM(const s32, "main/drawctrl", DrawSceneObjData);

INCLUDE_ASM(const s32, "main/drawctrl", DrawDoubleDispIn);

INCLUDE_ASM(const s32, "main/drawctrl", DrawVramClear);

INCLUDE_ASM(const s32, "main/drawctrl", DrawMoveDispIn);

INCLUDE_ASM(const s32, "main/drawctrl", DrawAlphaBlendDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawMozaikuDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawFadeDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawPlphaIndex8Disp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawTim2DIsp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawNoodlesDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawVramLocalCopy);

INCLUDE_ASM(const s32, "main/drawctrl", DrawVramLocalCopy2);

INCLUDE_ASM(const s32, "main/drawctrl", drawUseDrDispCheckInit);

INCLUDE_ASM(const s32, "main/drawctrl", drawDispCheckSub);

/* static */ int drawUseDispCheck(/* a0 4 */ int useD)
{
    return drawDispCheckSub(useD, &useDispFlag);
}

// INCLUDE_ASM(const s32, "main/drawctrl", drawDrDispCheck);
/* static */ int drawDrDispCheck(/* a0 4 */ int drD)
{
    return drawDispCheckSub(drD, &drDispFlag);
}

INCLUDE_ASM(const s32, "main/drawctrl", DrawScenectrlReq);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlInit);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlTitle);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlTitleDera);

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393300);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrl);

INCLUDE_ASM(const s32, "main/drawctrl", mendRatioTitleGet);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlTitleDisp);

INCLUDE_ASM(const s32, "main/drawctrl", MendererReq);

INCLUDE_ASM(const s32, "main/drawctrl", GetMendererEnum);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlScene);

INCLUDE_ASM(const s32, "main/drawctrl", sceneConditionCheck);

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlMain);

INCLUDE_ASM(const s32, "main/drawctrl", DrawSceneStrInit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawSceneStrReset);

INCLUDE_ASM(const s32, "main/drawctrl", resetDrawSceneObjData);

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlInit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlQuit);

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlTimeSet);

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlTblChange);

INCLUDE_ASM(const s32, "main/drawctrl", DrawTapReqTbl);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_event_sel);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_event_msg);

INCLUDE_ASM(const s32, "main/drawctrl", func_00115FB8);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_scene_msg);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_event_check);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_scene_check);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_bmp_check);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_event_sub);

INCLUDE_RODATA(const s32, "main/drawctrl", D_003933B0);

INCLUDE_RODATA(const s32, "main/drawctrl", D_003933C0);

INCLUDE_RODATA(const s32, "main/drawctrl", D_003933D0);

INCLUDE_RODATA(const s32, "main/drawctrl", D_003933E0);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_event_sub_bmp);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_tap_check);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_scene_sub);

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_tctrl_sub);

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlMainDebug);

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlInitDebug);
