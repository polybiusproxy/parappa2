#include "main/commake.h"

INCLUDE_ASM(const s32, "main/commake", setMakinDataMoto);

INCLUDE_ASM(const s32, "main/commake", getMakingDataKeyKind);

INCLUDE_ASM(const s32, "main/commake", getMakingDataKeyCnt);

INCLUDE_ASM(const s32, "main/commake", setMakingDataCopy);

INCLUDE_ASM(const s32, "main/commake", setMakingDataCopyCnt);

INCLUDE_ASM(const s32, "main/commake", setMakingDataCOMMAKE_STR);

INCLUDE_ASM(const s32, "main/commake", getMakinKeyKind2KeyKindNum);

INCLUDE_ASM(const s32, "main/commake", comMakeSSmaxCntGet);

INCLUDE_ASM(const s32, "main/commake", comMakeSSminCntGet);

INCLUDE_ASM(const s32, "main/commake", comMakeSubYure);

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

INCLUDE_ASM(const s32, "main/commake", comMakeSubChangeKey);

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

INCLUDE_ASM(const s32, "main/commake", comMakeSubSwapCntKey);

INCLUDE_ASM(const s32, "main/commake", comMakeSubDoubleKey);

INCLUDE_ASM(const s32, "main/commake", comMakeSubSpaceSearch);

INCLUDE_ASM(const s32, "main/commake", comMakeSubUseKeyCode);

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/commake", comMakingNo0);
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

INCLUDE_ASM(const s32, "main/commake", comMakingNo1);

INCLUDE_ASM(const s32, "main/commake", comMakingNo2);

INCLUDE_ASM(const s32, "main/commake", comMakingNo3);

INCLUDE_ASM(const s32, "main/commake", comMakingNo5);

INCLUDE_ASM(const s32, "main/commake", comMakingNo6);

INCLUDE_ASM(const s32, "main/commake", comMakingNo7);

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

INCLUDE_ASM(const s32, "main/commake", comMakingNo15);

INCLUDE_ASM(const s32, "main/commake", comMakingNo16);

INCLUDE_ASM(const s32, "main/commake", comSelection);

INCLUDE_ASM(const s32, "main/commake", computerMaking);
