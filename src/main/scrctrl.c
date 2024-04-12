#include "common.h"

INCLUDE_ASM(const s32, "main/scrctrl", GetCurrentTblNumber);

INCLUDE_ASM(const s32, "main/scrctrl", RANK_LEVEL2DISP_LEVEL);

INCLUDE_ASM(const s32, "main/scrctrl", RANK_LEVEL2DISP_LEVEL_HK);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDbuffCtrlInit);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDbuffSet);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDbuffSetSp);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDbuffClear);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapCtrlInit);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDataTrans);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDataTransCheck);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapReq);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapReqStop);

INCLUDE_ASM(const s32, "main/scrctrl", exam_tbl_updownInit);

INCLUDE_ASM(const s32, "main/scrctrl", exam_tbl_updownSet);

INCLUDE_ASM(const s32, "main/scrctrl", exam_tbl_updownChange);

INCLUDE_RODATA(const s32, "main/scrctrl", D_00392A60);

INCLUDE_RODATA(const s32, "main/scrctrl", D_00392AA8);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSetMemorySave);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSetMemoryLoad);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSet);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSetMoto);

INCLUDE_ASM(const s32, "main/scrctrl", followTapInit);

INCLUDE_ASM(const s32, "main/scrctrl", followTapSave);

INCLUDE_ASM(const s32, "main/scrctrl", followTapLoad);

INCLUDE_ASM(const s32, "main/scrctrl", ScrLincChangTbl);

INCLUDE_ASM(const s32, "main/scrctrl", ScrLincChangTblRef);

INCLUDE_ASM(const s32, "main/scrctrl", ScrLineSafeRefMode);

INCLUDE_ASM(const s32, "main/scrctrl", ScrDrawTimeGet);

INCLUDE_ASM(const s32, "main/scrctrl", ScrDrawTimeGetFrame);

INCLUDE_ASM(const s32, "main/scrctrl", KeyCntClear);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlCurrentSearch);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvInit);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlExamClear);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlExamClearIndv);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvNextTime);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvNextReadLine);

INCLUDE_ASM(const s32, "main/scrctrl", getLvlTblRand);

INCLUDE_ASM(const s32, "main/scrctrl", tapLevelChangeSub);

INCLUDE_ASM(const s32, "main/scrctrl", tapLevelChange);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvNextRead);

INCLUDE_ASM(const s32, "main/scrctrl", intIndvStatusSet);

INCLUDE_ASM(const s32, "main/scrctrl", allIndvNextContinue);

INCLUDE_ASM(const s32, "main/scrctrl", allIndvGoContinue);

INCLUDE_ASM(const s32, "main/scrctrl", otherIndvPause);

INCLUDE_ASM(const s32, "main/scrctrl", otherIndvTapReset);

INCLUDE_ASM(const s32, "main/scrctrl", selectIndvTapResetPlay);

INCLUDE_ASM(const s32, "main/scrctrl", IndivMoveChange);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevAllMove);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevCodeGet);

INCLUDE_ASM(const s32, "main/scrctrl", targetTimeGet);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevSndKill);

INCLUDE_ASM(const s32, "main/scrctrl", useAllClearKeySnd);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevSndKillOther);

INCLUDE_ASM(const s32, "main/scrctrl", TapKeyCheckNum);

INCLUDE_ASM(const s32, "main/scrctrl", TapKeyGetDatPP);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupInit);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupTapClear);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroup);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupPoll);

INCLUDE_ASM(const s32, "main/scrctrl", tapEventCheck);

INCLUDE_ASM(const s32, "main/scrctrl", otehon_all_make);

INCLUDE_ASM(const s32, "main/scrctrl", treateTimeChange);

INCLUDE_ASM(const s32, "main/scrctrl", thnum_get);

INCLUDE_ASM(const s32, "main/scrctrl", MapNormalNumGet);

INCLUDE_ASM(const s32, "main/scrctrl", on_th_make);

INCLUDE_ASM(const s32, "main/scrctrl", exh_normal_add);

INCLUDE_ASM(const s32, "main/scrctrl", exh_normal_sub);

INCLUDE_ASM(const s32, "main/scrctrl", exh_nombar_sub);

INCLUDE_ASM(const s32, "main/scrctrl", exh_mbar_key_out);

INCLUDE_ASM(const s32, "main/scrctrl", exh_mbar_time_out);

INCLUDE_ASM(const s32, "main/scrctrl", exh_mbar_num_out);

INCLUDE_ASM(const s32, "main/scrctrl", exh_yaku);

INCLUDE_ASM(const s32, "main/scrctrl", exh_yaku_original);

INCLUDE_ASM(const s32, "main/scrctrl", exh_yaku_hane);

INCLUDE_ASM(const s32, "main/scrctrl", exh_allkey_out);

INCLUDE_ASM(const s32, "main/scrctrl", exh_allkey_out_nh);

INCLUDE_ASM(const s32, "main/scrctrl", exh_command);

INCLUDE_ASM(const s32, "main/scrctrl", exh_renda_out);

INCLUDE_ASM(const s32, "main/scrctrl", manemane_check_sub);

INCLUDE_ASM(const s32, "main/scrctrl", manemane_check);

INCLUDE_ASM(const s32, "main/scrctrl", exh_mane);

INCLUDE_ASM(const s32, "main/scrctrl", exh_all_add);

INCLUDE_ASM(const s32, "main/scrctrl", IndvGetTapSetAdrs);

INCLUDE_ASM(const s32, "main/scrctrl", nextExamTime);

INCLUDE_ASM(const s32, "main/scrctrl", GetSindvPcodeLine);

INCLUDE_ASM(const s32, "main/scrctrl", ExamScoreCheck);

INCLUDE_ASM(const s32, "main/scrctrl", ExamScoreCheckSame);

INCLUDE_ASM(const s32, "main/scrctrl", levelChangeCheck);

INCLUDE_ASM(const s32, "main/scrctrl", levelUpRank);

INCLUDE_ASM(const s32, "main/scrctrl", levelDownRank);

INCLUDE_RODATA(const s32, "main/scrctrl", D_00392D30);

INCLUDE_RODATA(const s32, "main/scrctrl", D_00392D40);

INCLUDE_RODATA(const s32, "main/scrctrl", D_00392D88);

INCLUDE_ASM(const s32, "main/scrctrl", ScrMoveSetSub);

INCLUDE_ASM(const s32, "main/scrctrl", ScrExamSetCheck);

INCLUDE_ASM(const s32, "main/scrctrl", subjobEvent);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvJob);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTimeRenew);

INCLUDE_ASM(const s32, "main/scrctrl", ScrMbarReq);

INCLUDE_ASM(const s32, "main/scrctrl", allTimeCallbackTimeSetChanTempo);

INCLUDE_ASM(const s32, "main/scrctrl", SetIndvDrawTblLine);

INCLUDE_ASM(const s32, "main/scrctrl", otehonSetCheck);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlMainLoop);

INCLUDE_ASM(const s32, "main/scrctrl", GetTimeType);

INCLUDE_ASM(const s32, "main/scrctrl", GetTimeOfset);

INCLUDE_ASM(const s32, "main/scrctrl", GetSubtLine);

INCLUDE_ASM(const s32, "main/scrctrl", GetDrawLine);

INCLUDE_ASM(const s32, "main/scrctrl", GetLineTempo);

INCLUDE_ASM(const s32, "main/scrctrl", SetLineChannel);

INCLUDE_ASM(const s32, "main/scrctrl", SetIndvCdChannel);

INCLUDE_ASM(const s32, "main/scrctrl", CheckIndvCdChannel);

INCLUDE_RODATA(const s32, "main/scrctrl", dbg_tbl_msg);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlInit);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlQuit);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlInitCheck);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlGoLoop);

INCLUDE_ASM(const s32, "main/scrctrl", ScrEndCheckScore);

INCLUDE_ASM(const s32, "main/scrctrl", ScrEndCheckTitle);

INCLUDE_ASM(const s32, "main/scrctrl", ScrEndCheckFadeOut);

INCLUDE_ASM(const s32, "main/scrctrl", ScrEndWaitLoop);

INCLUDE_ASM(const s32, "main/scrctrl", bonusGameInit);

INCLUDE_ASM(const s32, "main/scrctrl", bonusGameCntPls);

INCLUDE_ASM(const s32, "main/scrctrl", bonusPointSave);

INCLUDE_ASM(const s32, "main/scrctrl", bngTapEventCheck);

INCLUDE_ASM(const s32, "main/scrctrl", bonusGameParaReq);

INCLUDE_ASM(const s32, "main/scrctrl", bonusGameKoamaReq);

INCLUDE_ASM(const s32, "main/scrctrl", bonus_minus_point_sub);

INCLUDE_ASM(const s32, "main/scrctrl", bonus_pls_point_sub);

INCLUDE_ASM(const s32, "main/scrctrl", bonusGameCtrl);

INCLUDE_ASM(const s32, "main/scrctrl", hex2dec);

INCLUDE_ASM(const s32, "main/scrctrl", bnNumberDisp);

INCLUDE_ASM(const s32, "main/scrctrl", bonusScoreDraw);

INCLUDE_ASM(const s32, "main/scrctrl", set_lero_gifset);

INCLUDE_ASM(const s32, "main/scrctrl", LessonRoundDisp);
