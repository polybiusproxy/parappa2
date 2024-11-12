#include "main/commake.h"
#include "main/scrctrl.h"

// data
extern void (*comMakeingTblLevel0[8])(CM_STR_CTRL *cmstr_pp);
extern void (*comMakeingTblLevel1[7])(CM_STR_CTRL *cmstr_pp);
extern void (*comMakeingTblLevel2[6])(CM_STR_CTRL *cmstr_pp);
extern void (*comMakeingTblLevel3[4])(CM_STR_CTRL *cmstr_pp);

// bss
extern CM_STR_CTRL cm_str_ctrl;

void comMakingNo15(CM_STR_CTRL *cmstr_pp);

static void setMakinDataMoto(TAPDAT *tapdat_pp, int size, CM_STR *cm_str_pp)
{
    int i;
    int time;

    for (i = 0; i < size; i++, tapdat_pp++)
    {
        if (tapdat_pp->KeyIndex == 0 || tapdat_pp->time < 0)
            continue;

        time = tapdat_pp->time / 24;
        if (time < 32)
        {
            cm_str_pp[time].keyId = tapdat_pp->KeyIndex;
        }
    }
}

static int getMakingDataKeyKind(CM_STR *cm_str_pp)
{
    int i;
    int ret = 0;

    for (i = 0; i < 32; i++, cm_str_pp++)
    {
        if (cm_str_pp->keyId != 0)
        {
            ret |= GetIndex2KeyCode(cm_str_pp->keyId);
        }
    }

    return ret;
}

static void getMakingDataKeyCnt(CM_STR *cm_str_pp, int *dat_pp)
{
    int i;

    for (i = 0; i < 7; i++)
    {
        dat_pp[i] = 0;
    }

    for (i = 0; i < 32; i++, cm_str_pp++)
    {
        if (cm_str_pp->keyId != 0)
        {
            dat_pp[cm_str_pp->keyId]++;
            (*dat_pp)++;
        }
    }
}

static void setMakingDataCopy(CM_STR *saki_pp, CM_STR *moto_pp) 
{
    int i;

    for (i = 0; i < 32; i++)
    {
        *saki_pp = *moto_pp;

        saki_pp++;
        moto_pp++;
    }
}

static void setMakingDataCopyCnt(CM_STR *saki_pp, CM_STR *moto_pp, int cnt)
{
    int i;

    for (i = 0; i < cnt; i++)
    {
        *saki_pp = *moto_pp;

        saki_pp++;
        moto_pp++;
    }
}

static int setMakingDataCOMMAKE_STR(COMMAKE_STR *com_pp, CM_STR *moto_pp)
{
    int i;
    int ret;
    int time;

    ret  = 0;
    time = 0;

    for (i = 0; i < 32; i++, moto_pp++)
    {
        if (moto_pp->keyId != 0)
        {
            com_pp->KeyIndex = moto_pp->keyId;
            com_pp->time = time + moto_pp->timeOfs;

            ret++;
            com_pp++;
        }

        time += 24;
    }

    return ret;
}

static int getMakinKeyKind2KeyKindNum(int keyCode)
{
    int i;
    int ret;

    ret = 0;

    for (i = 0; i < 32u; i++)
    {
        ret += (keyCode >> i) & 1;
    }

    return ret;
}

static int comMakeSSmaxCntGet(int *dat_pp)
{
    int i;
    int maxCnt;
    int ret;

    maxCnt = 0;
    ret    = 0;

    for (i = 1; i < 7; i++)
    {
        if (maxCnt < dat_pp[i])
        {
            maxCnt = dat_pp[i];
            ret = i;
        }
    }

    return ret;
}

static int comMakeSSminCntGet(int *dat_pp)
{
    int i;
    int minCnt;
    int ret;

    minCnt = 100;
    ret    = 0;

    for (i = 1; i < 7; i++)
    {
        if (dat_pp[i] == 0)
            continue;
        
        if (minCnt >= dat_pp[i])
        {
            minCnt = dat_pp[i];
            ret = i;
        }
    }

    return ret;
}

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

void comMakeSubChangeKey(CM_STR *cms_pp, int cnt, int motoKey, int sakiKey, int missCnt)
{
    int i;
    int yari_cnt;

    yari_cnt = 0;

    for (i = 0; i < cnt; i++, cms_pp++)
    {
        if (cms_pp->keyId != 0)
        {
            if (cms_pp->keyId == motoKey)
            {
                cms_pp->keyId = sakiKey;
                yari_cnt++;

                if (missCnt <= yari_cnt)
                    break;
            }
        }
    }
}

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

void comMakeSubSwapCntKey(CM_STR *cms_pp, int cnt, int swKey1, int swKey2, int pos)
{
    int i;
    int cntK;

    cntK = 0;

    for (i = 0; i < cnt; i++, cms_pp++)
    {
        if (cms_pp->keyId == swKey1)
        {
            if (cntK == pos)
            {
                cms_pp->keyId = swKey2;
                break;
            }

            cntK++;
        }
    }
}

void comMakeSubDoubleKey(CM_STR *cms_pp, int cnt)
{
    int i;
    int currentKey;

    currentKey = 0;

    for (i = 0; i < cnt; i++, cms_pp++)
    {
        if (currentKey == 0)
        {
            currentKey = cms_pp->keyId;
            continue;
        }

        if (cms_pp->keyId == 0)
        {
            cms_pp->keyId = currentKey;
            currentKey = 0;
        }
        else
        {
            currentKey = cms_pp->keyId;
        }
    }
}

CM_STR* comMakeSubSpaceSearch(CM_STR *cms_pp, int cnt)
{
    int     i;
    int     cntK;
    CM_STR *ret;

    ret  = NULL;
    cntK = 0;

    for (i = 0; i < cnt; i++, cms_pp++)
    {
        if (cms_pp->keyId == 0)
        {
            cntK++;

            if (cntK == 3)
            {
                ret = cms_pp - 1;
                break;
            }
        }
        else
        {
            cntK = 0;
        }
    }

    return ret;
}

int comMakeSubUseKeyCode(CM_STR *cms_pp, int cnt)
{
    int i;
    int ret;

    ret = 0;

    for (i = 0; i < cnt; i++)
    {
        if (cms_pp[i].keyId != 0)
        {
            ret |= GetIndex2KeyCode(cms_pp[i].keyId);
        }
    }

    return ret;
}

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

void comMakingNo3(CM_STR_CTRL *cmstr_pp)
{
    CM_STR *cm_str_pp;
    int     mabiki;

    setMakingDataCopy(cmstr_pp->cm_str_make, cmstr_pp->cm_str_now);

    cm_str_pp = comMakeSubSpaceSearch(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
    if (cm_str_pp != NULL)
    {
        cm_str_pp->keyId = comMakeSSminCntGet(cmstr_pp->keyCnt_now);
    }
    else
    {
        mabiki = comMakeSSmaxCntGet(cmstr_pp->keyCnt_now);

        if (cmstr_pp->keyCnt_now[mabiki] >= 2)
        {
            comMakeSubSwapCntKey(cmstr_pp->cm_str_make, cmstr_pp->maxBox, mabiki, 0, randMakeMax(cmstr_pp->keyCnt_now[mabiki]));
        }
    }

    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -11, 11);
}

void comMakingNo5(CM_STR_CTRL *cmstr_pp)
{   
    comMakingNo15(cmstr_pp);

    comMakeSubYureReset(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -5, 5);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -5, 5);
}

void comMakingNo6(CM_STR_CTRL *cmstr_pp)
{   
    comMakingNo3(cmstr_pp);

    comMakeSubYureReset(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -5, 5);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -5, 5);
}

void comMakingNo7(CM_STR_CTRL *cmstr_pp)
{
    int     code1;
    CM_STR *cm_str_pp;

    setMakingDataCopy(cmstr_pp->cm_str_make, cmstr_pp->cm_str_now);

    code1 = comMakeSSmaxCntGet(cmstr_pp->keyCnt_now);
    comMakeSubSwapKey(cmstr_pp->cm_str_make, cmstr_pp->maxBox, code1, comMakeSSminCntGet(cmstr_pp->keyCnt_now));

    cm_str_pp = comMakeSubSpaceSearch(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
    if (cm_str_pp != NULL)
    {
        cm_str_pp->keyId = comMakeSSmaxCntGet(cmstr_pp->keyCnt_now);
    }

    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -5, 5);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
}

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

void comMakingNo15(CM_STR_CTRL *cmstr_pp)
{
    CM_STR *cm_str_pp;
    int     posss;
    int     mabiki;

    setMakingDataCopy(cmstr_pp->cm_str_make, cmstr_pp->cm_str_now);

    cm_str_pp = comMakeSubSpaceSearch(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
    if (cm_str_pp != NULL)
    {
        posss = 0;

        cm_str_pp->keyId = comMakeSSmaxCntGet(cmstr_pp->keyCnt_now);

        while (1)
        {
            cm_str_pp = comMakeSubSpaceSearch(cmstr_pp->cm_str_make, cmstr_pp->maxBox);
            if (cm_str_pp == NULL)
                break;

            posss ^= 1;
            cm_str_pp[posss].keyId = comMakeSSminCntGet(cmstr_pp->keyCnt_now);
        }
    }
    else
    {
        mabiki = comMakeSSmaxCntGet(cmstr_pp->keyCnt_now);

        if (cmstr_pp->keyCnt_now[mabiki] >= 2)
        {
            comMakeSubSwapCntKey(cmstr_pp->cm_str_make, cmstr_pp->maxBox, mabiki, 0, randMakeMax(cmstr_pp->keyCnt_now[mabiki]));
        }
    }

    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
    comMakeSubYure(cmstr_pp->cm_str_make, cmstr_pp->maxBox, -2, 2);
}

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
