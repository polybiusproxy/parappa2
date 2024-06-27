#include "common.h"

#include "main/subt.h"

/* sdata - static */
int _PadFont_SW;
int _PadArrowState;

/* sbss - static */
int _AnimeFontFlg;
MCODE_STR *kanji_pp_TmpMenuFont;

/* lit4 */
float D_003990C8;
float D_003990CC;
float D_003990D0;
float D_003990D4;
float D_003990D8;

INCLUDE_ASM(const s32, "menu/menufont", MenuFont_ASC2EUC);

INCLUDE_ASM(const s32, "menu/menufont", MENUSubtSetKanji);

INCLUDE_ASM(const s32, "menu/menufont", MENUSubt_PadFontSw);

INCLUDE_ASM(const s32, "menu/menufont", MENUSubt_PadFontArrowSet);

INCLUDE_ASM(const s32, "menu/menufont", MENUSubtGetLine);

INCLUDE_ASM(const s32, "menu/menufont", MENUSubtPut);

INCLUDE_ASM(const s32, "menu/menufont", MENUFontPutL);

INCLUDE_ASM(const s32, "menu/menufont", MENUFontPutS);

INCLUDE_ASM(const s32, "menu/menufont", MENUFontPutR);

INCLUDE_ASM(const s32, "menu/menufont", _PKFontPut);

INCLUDE_ASM(const s32, "menu/menufont", _PADArrow_Put);

INCLUDE_ASM(const s32, "menu/menufont", _JPFont_GetSubtCode);

INCLUDE_ASM(const s32, "menu/menufont", _EGFont_GetSubtCode);

INCLUDE_ASM(const s32, "menu/menufont", codeKanjiCheck_TmpMenuFont);

INCLUDE_ASM(const s32, "menu/menufont", codeKanjiACheck);

static void euc2sjis(unsigned char *c1, unsigned char *c2)
{
    if (*c1 % 2 == 0)
        *c2 -= 0x02;
    else
    {
        *c2 -= 0x61;
        if (*c2 > 0x7e)
            (*c2)++;
    }

    if (*c1 < 0xdf)
    {
        (*c1)++;
        *c1 /= 2;
        *c1 += 0x30;
    }
    else
    {
        (*c1)++;
        *c1 /= 2;
        *c1 += 0x70;
    }
}
