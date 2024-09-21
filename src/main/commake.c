#include "main/commake.h"
#include "main/scrctrl.h"

// data
extern void (*comMakeingTblLevel0[8])(CM_STR_CTRL *cmstr_pp);
extern void (*comMakeingTblLevel1[7])(CM_STR_CTRL *cmstr_pp);
extern void (*comMakeingTblLevel2[6])(CM_STR_CTRL *cmstr_pp);
extern void (*comMakeingTblLevel3[4])(CM_STR_CTRL *cmstr_pp);

// bss
extern CM_STR_CTRL cm_str_ctrl;

INCLUDE_ASM("main/commake", setMakinDataMoto);

INCLUDE_ASM("main/commake", getMakingDataKeyKind);

INCLUDE_ASM("main/commake", getMakingDataKeyCnt);

INCLUDE_ASM("main/commake", setMakingDataCopy);

INCLUDE_ASM("main/commake", setMakingDataCopyCnt);

INCLUDE_ASM("main/commake", setMakingDataCOMMAKE_STR);

INCLUDE_ASM("main/commake", getMakinKeyKind2KeyKindNum);

INCLUDE_ASM("main/commake", comMakeSSmaxCntGet);

INCLUDE_ASM("main/commake", comMakeSSminCntGet);

void comMakeSubYure(CM_STR *cms_pp, int cnt, int min, int max)
{
    int i;
    int rnd_cnt;

    for (i = 0; i < cnt; i++, cms_pp++)
    {
        if (cms_pp->keyId != 0)
        {
            rnd_cnt = randMakeMax(max - min);
            cms_pp->timeOfs += rnd_cnt + min;
        }
    }
}

void comMakeSubYureReset(CM_STR *cms_pp, int cnt)
{
    int i;

    for (i = 0; i < cnt; i++, cms_pp++)
    {
        if (cms_pp->keyId != 0)
        {
            cms_pp->timeOfs = 0;
        }
    }
}

INCLUDE_ASM("main/commake", comMakeSubChangeKey);

void comMakeSubSwapKey(CM_STR *cms_pp, int cnt, int swKey1, int swKey2)
{
    int i;

    for (i = 0; i < cnt; i++, cms_pp++)
    {
        if (cms_pp->keyId == swKey1)
        {
            cms_pp->keyId = swKey2;
        }
        else if (cms_pp->keyId == swKey2)
        {
            cms_pp->keyId = swKey1;
        }
    }
}

INCLUDE_ASM("main/commake", comMakeSubSwapCntKey);

INCLUDE_ASM("main/commake", comMakeSubDoubleKey);

INCLUDE_ASM("main/commake", comMakeSubSpaceSearch);

INCLUDE_ASM("main/commake", comMakeSubUseKeyCode);

#ifndef NON_MATCHING
INCLUDE_ASM("main/commake", comMakingNo0);
#else
void comMakingNo0(CM_STR_CTRL *cmstr_pp)
{
    int moto_code;
    int saki_code;

    setMakingDataCopy(cmstr_pp->cm_str_make, cmstr_pp->cm_str_now);

    if (cmstr_pp->keyKindNum > 2)
    {
        moto_code = comMakeSSmaxCntGet(cmstr_pp->keyCnt_mt);
        saki_code = comMakeSSminCntGet(cmstr_pp->keyCnt_mt);

        if (moto_code != saki_code)
        {
            if ((saki_code == 0) && (moto_code == 0))
            {
                comMakeSubChangeKey(cmstr_pp->cm_str_make, cmstr_pp->maxBox, 0, 0, 1);
            }
        }
    }

    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -11, 11);
}
#endif

void comMakingNo1(CM_STR_CTRL *cmstr_pp)
{
    setMakingDataCopyCnt(cmstr_pp->cm_str_make, cmstr_pp->cm_str_mt, cmstr_pp->maxBox / 2);
    setMakingDataCopyCnt(&cmstr_pp->cm_str_make[cmstr_pp->maxBox / 2], cmstr_pp->cm_str_mt, cmstr_pp->maxBox / 2);

    if (!comMakeSubUseKeyCode(cmstr_pp->cm_str_make, cmstr_pp->maxBox))
    {
        setMakingDataCopyCnt(cmstr_pp->cm_str_make, &cmstr_pp->cm_str_mt[cmstr_pp->maxBox / 2], cmstr_pp->maxBox / 2);
        setMakingDataCopyCnt(&cmstr_pp->cm_str_make[cmstr_pp->maxBox / 2], &cmstr_pp->cm_str_mt[cmstr_pp->maxBox / 2], cmstr_pp->maxBox / 2);
    }

    if (!comMakeSubUseKeyCode(cmstr_pp->cm_str_make, cmstr_pp->maxBox))
    {
        setMakingDataCopyCnt(cmstr_pp->cm_str_make, cmstr_pp->cm_str_mt, cmstr_pp->maxBox);
    }

    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -11, 11);
}

void comMakingNo2(CM_STR_CTRL *cmstr_pp)
{
    setMakingDataCopy(cmstr_pp->cm_str_make, cmstr_pp->cm_str_now);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -11, 11);
}

INCLUDE_ASM("main/commake", comMakingNo3);

INCLUDE_ASM("main/commake", comMakingNo5);

INCLUDE_ASM("main/commake", comMakingNo6);

INCLUDE_ASM("main/commake", comMakingNo7);

void comMakingNo8(CM_STR_CTRL *cmstr_pp)
{
    comMakingNo3(cmstr_pp);

    comMakeSubYureReset(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -5, 5);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
}

void comMakingNo9(CM_STR_CTRL *cmstr_pp)
{   
    comMakingNo15(cmstr_pp);

    comMakeSubYureReset(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -5, 5);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
}

INCLUDE_ASM("main/commake", comMakingNo15);

void comMakingNo16(CM_STR_CTRL *cmstr_pp)
{
    setMakingDataCopy(cmstr_pp->cm_str_make, cmstr_pp->cm_str_now);

    if (cmstr_pp->keyCnt_now[0] < (cmstr_pp->maxBox / 2))
    {
        comMakeSubDoubleKey(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
    }

    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
}

static void comSelection(LEVEL_VS_ENUM lvl, CM_STR_CTRL *cmstr_pp)
{
    void (**comMakeingTbl_tmp)(CM_STR_CTRL* cmstr_pp);
    int tblcnt;

    switch (lvl)
    {
    default:
    case LVS_1:
        tblcnt = 8;
        comMakeingTbl_tmp = comMakeingTblLevel0;
        break;
    case LVS_2:
        tblcnt = 7;
        comMakeingTbl_tmp = comMakeingTblLevel1;
        break;
    case LVS_3:
        tblcnt = 6;
        comMakeingTbl_tmp = comMakeingTblLevel2;
        break;
    case LVS_4:
        tblcnt = 4;
        comMakeingTbl_tmp = comMakeingTblLevel3;
        break;
    }

    (comMakeingTbl_tmp[randMakeMax(tblcnt)])(cmstr_pp);
}

int computerMaking(COMMAKE_STR *com_pp, int com_cnt, TAPDAT *moto_pp, int moto_cnt, TAPSET *tapset_pp, LEVEL_VS_ENUM clvl)
{
    WorkClear(&cm_str_ctrl, sizeof(cm_str_ctrl));

    cm_str_ctrl.maxBox = ((tapset_pp->taptimeEnd - tapset_pp->taptimeStart) / 24) - 1;

    setMakinDataMoto(tapset_pp->tapdat_pp, tapset_pp->tapdat_size, cm_str_ctrl.cm_str_mt);
    setMakinDataMoto(moto_pp, moto_cnt, cm_str_ctrl.cm_str_now);

    cm_str_ctrl.keyKind = getMakingDataKeyKind(cm_str_ctrl.cm_str_mt);
    cm_str_ctrl.keyKindNum = getMakinKeyKind2KeyKindNum(cm_str_ctrl.keyKind);

    getMakingDataKeyCnt(cm_str_ctrl.cm_str_mt, cm_str_ctrl.keyCnt_mt);
    getMakingDataKeyCnt(cm_str_ctrl.cm_str_now, cm_str_ctrl.keyCnt_now);

    comSelection(clvl, &cm_str_ctrl);
    return setMakingDataCOMMAKE_STR(com_pp, cm_str_ctrl.cm_str_make);
}
