#include "menu/menusub.h"
#include "menu/menu_mdl.h"

/* .sdata */
TSTEX_INF *tblTex;
HOSI_OBJ *HOSIObj;

/* .bss */
extern TSREPPAD menuPadState[2][4];

static int TsGetMenuPadIsRepeat(int no, int npad)
{
    return (menuPadState[no][npad].state < 2) ^ 1;
}

INCLUDE_ASM(const s32, "menu/menusub", TSSNDPLAY);

INCLUDE_ASM(const s32, "menu/menusub", TSSNDSTOP);

INCLUDE_ASM(const s32, "menu/menusub", TSSNDMASK_CHAN);

INCLUDE_ASM(const s32, "menu/menusub", TSSND_SKIPSTOP);

INCLUDE_ASM(const s32, "menu/menusub", TSSND_SKIPPLAY);

INCLUDE_ASM(const s32, "menu/menusub", TSSND_CHANISSTOP);

INCLUDE_ASM(const s32, "menu/menusub", tsBGMONEPlay);

INCLUDE_ASM(const s32, "menu/menusub", tsBGMONEStop);

INCLUDE_ASM(const s32, "menu/menusub", tsBGMONEVol);

INCLUDE_ASM(const s32, "menu/menusub", tsBGMONETop);

INCLUDE_ASM(const s32, "menu/menusub", tsBGMONEflow);

INCLUDE_ASM(const s32, "menu/menusub", tsBGMONEPause);

INCLUDE_ASM(const s32, "menu/menusub", TsBGMInit);

INCLUDE_ASM(const s32, "menu/menusub", TsBGMPlay);

INCLUDE_ASM(const s32, "menu/menusub", TsBGMStop);

INCLUDE_ASM(const s32, "menu/menusub", TsBGMMute);

INCLUDE_ASM(const s32, "menu/menusub", TsBGMLoadCheck);

INCLUDE_ASM(const s32, "menu/menusub", TsBGMPause);

INCLUDE_ASM(const s32, "menu/menusub", TsBGMChangePos);

INCLUDE_ASM(const s32, "menu/menusub", TsBGMPoll);

INCLUDE_ASM(const s32, "menu/menusub", TsCmnPkOpen);

INCLUDE_ASM(const s32, "menu/menusub", TsCmnPkClose);

INCLUDE_ASM(const s32, "menu/menusub", _P3DATA_SIZE);

INCLUDE_ASM(const s32, "menu/menusub", TsGetTm2Tex);

INCLUDE_ASM(const s32, "menu/menusub", TsGetTm2HedTex);

INCLUDE_ASM(const s32, "menu/menusub", TsClearMenuPad);

INCLUDE_ASM(const s32, "menu/menusub", TsGetMenuPad);

INCLUDE_ASM(const s32, "menu/menusub", TsSndFlow);

INCLUDE_ASM(const s32, "menu/menusub", TSNumMov);

INCLUDE_ASM(const s32, "menu/menusub", TSNumRBack);

static int TSLOOP(int no, int max)
{
    return (no + max) % max;
}

static int TSLIMIT(int no, int min, int max)
{
    if (no < min)
        return min;

    if (no >= max)
        return max - 1;
    
    return no;
}

INCLUDE_ASM(const s32, "menu/menusub", TsMENU_GetMapNo);

INCLUDE_ASM(const s32, "menu/menusub", TsMENU_GetMapTimeState);

INCLUDE_ASM(const s32, "menu/menusub", TsSetScene_Map);

INCLUDE_ASM(const s32, "menu/menusub", TsSet_ParappaCapColor);

INCLUDE_RODATA(const s32, "menu/menusub", D_00395F10);

INCLUDE_RODATA(const s32, "menu/menusub", D_00395F30);

INCLUDE_ASM(const s32, "menu/menusub", TsClearSet);

INCLUDE_ASM(const s32, "menu/menusub", TsCheckEnding);

INCLUDE_ASM(const s32, "menu/menusub", TsMENU_InitSystem);

INCLUDE_ASM(const s32, "menu/menusub", TsMENU_EndSystem);

INCLUDE_ASM(const s32, "menu/menusub", TsMenu_RankingClear);

INCLUDE_ASM(const s32, "menu/menusub", TsMenu_Init);

INCLUDE_ASM(const s32, "menu/menusub", TsMenu_End);

INCLUDE_ASM(const s32, "menu/menusub", TsMenu_InitFlow);

INCLUDE_ASM(const s32, "menu/menusub", TsMenuMemcChk_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsMenu_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsMenu_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsSetRankingName);

INCLUDE_ASM(const s32, "menu/menusub", TsSetRanking2UData);

INCLUDE_ASM(const s32, "menu/menusub", TsSetSaveData);

INCLUDE_ASM(const s32, "menu/menusub", TsRestoreSaveData);

int DateChgInt(u_int n)
{
    // Convert BCD to decimal
    return 
    (
        ((n & 0xf0) >> 4) * 10 +
         (n & 0xf)
    );
}

INCLUDE_ASM(const s32, "menu/menusub", GetRankScoreID);

INCLUDE_ASM(const s32, "menu/menusub", TsRanking_Set);

INCLUDE_ASM(const s32, "menu/menusub", TsMENU_SetMapScreen);

INCLUDE_ASM(const s32, "menu/menusub", TsCheckTimeMapChange);

int TsAnimeWait_withKeySkip(u_int tpad, MN_SCENE *scene, int ltim, u_int bnk)
{
    if (bnk == -1)
    {
        return MNScene_isAnime(scene, ltim);
    }
    else
    {
        return MNScene_isAnimeBank(scene, ltim, bnk);
    }
}

INCLUDE_ASM(const s32, "menu/menusub", TsMemCardCheck_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsMap_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsMakeUserWork);

INCLUDE_ASM(const s32, "menu/menusub", TsSaveSuccessProc);

INCLUDE_ASM(const s32, "menu/menusub", MpSave_Flow);

INCLUDE_ASM(const s32, "menu/menusub", MpCityHall_Flow);

INCLUDE_ASM(const s32, "menu/menusub", MpCityHallParaStart);

INCLUDE_ASM(const s32, "menu/menusub", MpCityHallFPHSSoundMask);

INCLUDE_ASM(const s32, "menu/menusub", MpCityHallFPHSMove);

INCLUDE_ASM(const s32, "menu/menusub", MpCityHallFPHOK);

INCLUDE_ASM(const s32, "menu/menusub", MpCityHallCharPosSet);

INCLUDE_ASM(const s32, "menu/menusub", MpPopMenu_Flow);

INCLUDE_ASM(const s32, "menu/menusub", MpMapMenu_Flow);

INCLUDE_ASM(const s32, "menu/menusub", _MapGetMovableDir);

INCLUDE_ASM(const s32, "menu/menusub", McErrorMess);

INCLUDE_ASM(const s32, "menu/menusub", McInitFlow);

INCLUDE_ASM(const s32, "menu/menusub", McStartCheckFlow);

INCLUDE_ASM(const s32, "menu/menusub", McUserCheckFlow);

INCLUDE_ASM(const s32, "menu/menusub", McUserSaveFlow);

INCLUDE_ASM(const s32, "menu/menusub", McUserLoadFlow);

INCLUDE_ASM(const s32, "menu/menusub", TsMCAMes_Init);

INCLUDE_ASM(const s32, "menu/menusub", TsMCAMes_GetSelect);

INCLUDE_ASM(const s32, "menu/menusub", TsMCAMes_IsON);

INCLUDE_ASM(const s32, "menu/menusub", TsMCAMes_SetPos);

INCLUDE_ASM(const s32, "menu/menusub", TsMCAMes_SetMes);

INCLUDE_ASM(const s32, "menu/menusub", TsMCAMes_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsMCAMes_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsCMPMes_SetPos);

INCLUDE_ASM(const s32, "menu/menusub", TsCMPMes_SetMes);

INCLUDE_ASM(const s32, "menu/menusub", TsCMPMes_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsANIME_Init);

INCLUDE_ASM(const s32, "menu/menusub", TsANIME_Poll);

INCLUDE_ASM(const s32, "menu/menusub", TsANIME_Start);

INCLUDE_ASM(const s32, "menu/menusub", TsANIME_GetRate);

INCLUDE_ASM(const s32, "menu/menusub", _TsSortSetRanking);

INCLUDE_ASM(const s32, "menu/menusub", TsGetRankingList);

INCLUDE_ASM(const s32, "menu/menusub", TsPopCusAOff);

INCLUDE_ASM(const s32, "menu/menusub", TsPopCusDim);

INCLUDE_ASM(const s32, "menu/menusub", TsPopCusInit);

INCLUDE_ASM(const s32, "menu/menusub", TsPopCusFlow);

INCLUDE_ASM(const s32, "menu/menusub", TsPopCusPut);

INCLUDE_ASM(const s32, "menu/menusub", TsPUPCheckMove);

INCLUDE_RODATA(const s32, "menu/menusub", D_00396068);

INCLUDE_RODATA(const s32, "menu/menusub", D_00396070);

INCLUDE_RODATA(const s32, "menu/menusub", D_00396078);

INCLUDE_ASM(const s32, "menu/menusub", TsPopMenu_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsPopMenu_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsPopMenCus_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsSaveMenu_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsSaveMenu_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TSJukeCDObj_Init);

INCLUDE_ASM(const s32, "menu/menusub", _TsJkJacketPut);

INCLUDE_ASM(const s32, "menu/menusub", _TsJkRecordPut);

INCLUDE_ASM(const s32, "menu/menusub", TSJukeCDObj_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsJukeIsObjAnime);

INCLUDE_ASM(const s32, "menu/menusub", TsJukeObjAnime);

INCLUDE_ASM(const s32, "menu/menusub", TsJukeObjAnime2);

INCLUDE_ASM(const s32, "menu/menusub", _TsJKMoveCus);

INCLUDE_ASM(const s32, "menu/menusub", _TsJKSetPadArrow);

INCLUDE_ASM(const s32, "menu/menusub", TsJukeMenu_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsJukeMenu_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsCmnCell_CusorSET);

INCLUDE_ASM(const s32, "menu/menusub", TsCmnCell_CusorON);

INCLUDE_ASM(const s32, "menu/menusub", TsCmnCell_CusorOFF);

INCLUDE_ASM(const s32, "menu/menusub", TsCmnCell_CusorSEL);

INCLUDE_ASM(const s32, "menu/menusub", TsCmnCell_CusorMASK);

INCLUDE_ASM(const s32, "menu/menusub", TsCmnCell_CusorDraw);

INCLUDE_ASM(const s32, "menu/menusub", TsOption_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsOption_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_GetCurFileNo);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_IsGetFileSave);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_SortUser);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_SetCurUserData);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_SetCurDispUserData);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_SetCurFileNoCusor);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_SetType);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_TagChangeAble);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_SetCurTag);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsUserList_Draw);

INCLUDE_ASM(const s32, "menu/menusub", NameSpaceCut);

INCLUDE_RODATA(const s32, "menu/menusub", D_00396120);

INCLUDE_ASM(const s32, "menu/menusub", TsUser_PanelDraw);

INCLUDE_ASM(const s32, "menu/menusub", TsNAMEINBox_SetName);

INCLUDE_ASM(const s32, "menu/menusub", TsNAMEINBox_GetName);

INCLUDE_ASM(const s32, "menu/menusub", TsNAMEINBox_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsNAMEINBox_Draw);

INCLUDE_ASM(const s32, "menu/menusub", TsSCFADE_Set);

INCLUDE_ASM(const s32, "menu/menusub", TsSCFADE_Flow);

INCLUDE_ASM(const s32, "menu/menusub", TsSCFADE_Draw);

INCLUDE_ASM(const s32, "menu/menusub", _PkMCMsgPut);

INCLUDE_ASM(const s32, "menu/menusub", _PkMCMsgGetLine);

INCLUDE_ASM(const s32, "menu/menusub", _PkSubMsgPut);

INCLUDE_ASM(const s32, "menu/menusub", TsMenu_CleanVram);

void TsMenu_CaptureVram(SPR_PKT pk, SPR_PRM *spr)
{
    PkSprPkt_SetDrawEnv(pk, spr, DrawGetDrawEnvP(DNUM_VRAM2));
    PkSprPkt_SetTexVram(pk, spr, DrawGetDrawEnvP(DNUM_DRAW));

    PkTEX1_Add(pk, SCE_GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
    PkALPHA_Add(pk, SCE_GS_SET_ALPHA(2, 2, 0, 0, 0));

    spr->zy = 1.0f;
    spr->zx = 1.0f;
    spr->zdepth = 0;
    spr->rgba0 = SCE_GS_SET_RGBAQ(128, 128, 128, 128, 0);

    SetSprScreenXYWH(spr);
    spr->ux = spr->px;
    spr->uy = spr->py;
    spr->uw = spr->sw;
    spr->uh = spr->sh;

    PkNSprite_AddAdj(pk, spr, 1);
    PkTEX1_Add(pk, 0x2020);

    PkSprPkt_SetDrawEnv(pk, spr, DrawGetDrawEnvP(DNUM_DRAW));
    PkSprPkt_SetTexVram(pk, spr, DrawGetDrawEnvP(DNUM_VRAM2));

    PkALPHA_Add(pk, SCE_GS_SET_ALPHA(0, 1, 0, 1, 0));
}

INCLUDE_ASM(const s32, "menu/menusub", TsSetCTransSpr);

INCLUDE_ASM(const s32, "menu/menusub", TsSetSLTransSpr);

INCLUDE_ASM(const s32, "menu/menusub", TsSetPNTransSpr);
void TsSetPNTransSpr(/* s3 19 */ SPR_PKT pk, /* s2 18 */ SPR_PRM *spr, /* a2 6 */ int mx, /* a3 7 */ int my, /* f12 50 */ float wr, /* f22 60 */ float dr);

INCLUDE_ASM(const s32, "menu/menusub", TsPatTexFnc);

INCLUDE_ASM(const s32, "menu/menusub", _TsPatSetPrm);
/* static */ void _TsPatSetPrm(/* t1 9 */ SPR_PKT pk, /* s0 16 */ SPR_PRM *spr, /* s2 18 */ PATPOS *ppos, /* s3 19 */ int ox, /* s4 20 */ int oy);

INCLUDE_ASM(const s32, "menu/menusub", TsPatPut);

static void TsPatGetSize(PATPOS *ppos, int *x, int *y, int *w, int *h)
{
	TSTEX_INF *ptex = &tblTex[ppos->texNo];

    if (w != NULL)
        *w = ptex->w;

    if (h != NULL)
        *h = ptex->h;
    
    if (x != NULL)
        *x = ppos->x + (ptex->w / 2);

    if (y != NULL)
        *y = ppos->y + (ptex->h / 2);      
}

INCLUDE_ASM(const s32, "menu/menusub", TsPatPutRZoom);

INCLUDE_ASM(const s32, "menu/menusub", TsPatPutMZoom);

INCLUDE_ASM(const s32, "menu/menusub", TsPatPutSwing);

static void TsPatPutUneri(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, int mx, int my, float Crx, float Drt)
{
    _TsPatSetPrm(pk, spr, ppos, ox, oy);
    TsSetPNTransSpr(pk, spr, mx, my, Crx, Drt);
}

INCLUDE_ASM(const s32, "menu/menusub", TsCELBackInit);

void TsCELBackEnd(void)
{
    if (HOSIObj != NULL)
    {
        free(HOSIObj);
        HOSIObj = NULL;
    }
}

INCLUDE_ASM(const s32, "menu/menusub", TsCELBackDraw);

INCLUDE_ASM(const s32, "menu/menusub", _TsCELBackObjDraw);

INCLUDE_ASM(const s32, "menu/menusub", TsHosiPut);

INCLUDE_RODATA(const s32, "menu/menusub", D_00396168);

INCLUDE_RODATA(const s32, "menu/menusub", D_00396170);
