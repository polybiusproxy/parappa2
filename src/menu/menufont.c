#include "menu/menufont.h"

#include <string.h>

/* data */
extern u_long SubtGsTex0_TmpMenuFont[3];
extern MCODE_ASCII mcode_ascii_TmpMenuFont[];
extern char Tbl_ASC2EUC[193];

/* sdata - static */
int _PadFont_SW;
int _PadArrowState;

/* sbss - static */
int _AnimeFontFlg;
MCODE_STR *kanji_pp_TmpMenuFont;

/* bss - static */
extern MNFONT_INFO MnSubtFontInfo[3];

/* lit4 */
float D_003990C8;
float D_003990CC;
float D_003990D0;
float D_003990D4;
float D_003990D8;

static int _JPFont_GetSubtCode(u_char *str, SUBT_CODE *subt_code);
static int _EGFont_GetSubtCode(u_char *str, SUBT_CODE *subt_code, MCODE_DAT *pfnt_ascii);
static void _PKFontPut(SPR_PKT pk, SPR_PRM *spr, SUBT_CODE *psubt, int line_num, int xp, int yp, int pflg, int hsize, float rtx, float rty);

#if 1
INCLUDE_ASM(const s32, "menu/menufont", MenuFont_ASC2EUC);
#else
void MenuFont_ASC2EUC(/* s0 16 */ char *des, /* s1 17 */ char *src)
{
    /* a3 7 */ u_int  len;
    /* v1 3 */ u_char c;

    len = strlen(Tbl_ASC2EUC);
    c = *src;

    while (1)
    {
        if (c == 0)
        {
            *des = '\0';
            return;
        }

        if (c > 0x20)
        {
            if ((len / 2) <= c - 0x20)
            {
                *des   = -0x5f;
                *des++ = -0x5f;
            }
            else
            {
                *des   = Tbl_ASC2EUC[((c - 0x20 & 0x7f) * 2)];
                *des++ = Tbl_ASC2EUC[((c - 0x20 & 0x7f) * 2) + 1];
            }
        }
        else
        {
            *des   = -0x5f;
            *des++ = -0x5f;
        }

        src++;
        des++;
    }
}
#endif

void MENUSubtSetKanji(void *kanji_data_top)
{
    kanji_pp_TmpMenuFont = (MCODE_STR*)kanji_data_top;
}

void MENUSubt_PadFontSw(int flg)
{
    _PadFont_SW = flg;
}

void MENUSubt_PadFontArrowSet(int flg)
{
    _PadArrowState = 0;

    if (flg & 1)
        _PadArrowState = 1;
    if (flg & 2)
        _PadArrowState |= 2;
    if (flg & 4)
        _PadArrowState |= 4;
    if (flg & 8)
        _PadArrowState |= 8;
}

int MENUSubtGetLine(u_char *str, int lflg)
{
    SUBT_CODE subt_code[16] = {};
    int       line;

    if (lflg == LANG_JAPANESE)
        line = _JPFont_GetSubtCode(str, subt_code);
    else
        line = _EGFont_GetSubtCode(str, subt_code, mcode_ascii_TmpMenuFont);

    return line;
}

#if 1
INCLUDE_ASM(const s32, "menu/menufont", MENUSubtPut);
#else
void MENUSubtPut(SPR_PKT pk, SPR_PRM *spr, int x, int y, u_int abgr, int flg, u_char *str, int lflg)
{
    SUBT_CODE subt_code[16] = {};
    int       line;
    u_long    tex0;

    MnSubtFontInfo[0].tex0 = SubtGsTex0_TmpMenuFont[0];
    MnSubtFontInfo[1].tex0 = SubtGsTex0_TmpMenuFont[1];

    MnSubtFontInfo[0].abgr = abgr;
    MnSubtFontInfo[1].abgr = abgr;

    tex0 = TsGetTM2Hed(1)->GsTex0;
    MnSubtFontInfo[2].tex0 = tex0 | 0x400000000;
    MnSubtFontInfo[2].abgr = 0x80808080;

    if (lflg == LANG_JAPANESE)
        line = _JPFont_GetSubtCode(str, subt_code);
    else
        line = _EGFont_GetSubtCode(str, subt_code, mcode_ascii_TmpMenuFont);

    if (line != 0)
    {
        spr->rgba0 = abgr;

        _AnimeFontFlg = ((MNSceneGetMusicFitTimer() % 40) >= 21);

        spr->zx = 1.0f;
        spr->zy = D_003990C8;
        
        _PKFontPut(pk, spr, subt_code, line, x, y, flg, 26, 1.0f, D_003990C8);
    }
}
#endif

INCLUDE_ASM(const s32, "menu/menufont", MENUFontPutL);

INCLUDE_ASM(const s32, "menu/menufont", MENUFontPutS);

INCLUDE_ASM(const s32, "menu/menufont", MENUFontPutR);

INCLUDE_ASM(const s32, "menu/menufont", _PKFontPut);

INCLUDE_ASM(const s32, "menu/menufont", _PADArrow_Put);

INCLUDE_ASM(const s32, "menu/menufont", _JPFont_GetSubtCode);

INCLUDE_ASM(const s32, "menu/menufont", _EGFont_GetSubtCode);

static MCODE_DAT* codeKanjiCheck(u_char dat0, u_char dat1, MCODE_KANJI *kcode_pp, int kcode_max)
{
    u_short code = dat0 | (dat1 << 8);
    int     i    = 0;       

    for (i = 0; i < kcode_max; i++, kcode_pp++)
    {
        if (kcode_pp->code == code)
            return (MCODE_DAT*)(&kcode_pp->u);
    }

    return NULL;
}

static MCODE_DAT* codeKanjiACheck(u_char dat0, u_char dat1, MCODE_KANJI_ANIME *kcode_pp, int kcode_max)
{
    u_short code = dat0 | (dat1 << 8);
    int     i    = 0;       

    for (i = 0; i < kcode_max; i++, kcode_pp++)
    {
        if (kcode_pp->code == code)
            return (MCODE_DAT*)(&kcode_pp->u);
    }

    return NULL;
}

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
