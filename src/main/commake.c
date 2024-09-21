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

INCLUDE_ASM(const s32, "main/commake", comMakingNo0);

INCLUDE_ASM(const s32, "main/commake", comMakingNo1);

INCLUDE_ASM(const s32, "main/commake", comMakingNo2);

INCLUDE_ASM(const s32, "main/commake", comMakingNo3);

INCLUDE_ASM(const s32, "main/commake", comMakingNo5);

INCLUDE_ASM(const s32, "main/commake", comMakingNo6);

INCLUDE_ASM(const s32, "main/commake", comMakingNo7);

INCLUDE_ASM(const s32, "main/commake", comMakingNo8);

INCLUDE_ASM(const s32, "main/commake", comMakingNo9);

INCLUDE_ASM(const s32, "main/commake", comMakingNo15);

INCLUDE_ASM(const s32, "main/commake", comMakingNo16);

INCLUDE_ASM(const s32, "main/commake", comSelection);

INCLUDE_ASM(const s32, "main/commake", computerMaking);
