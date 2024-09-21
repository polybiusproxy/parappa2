#include "menu/menufont.h"

#include <string.h>

/* data */
extern u_long SubtGsTex0_TmpMenuFont[3];
extern MCODE_ASCII mcode_ascii_TmpMenuFont[];
extern char Tbl_ASC2EUC[193];

extern MCODE_CHAR mcode_dat_pp_TmpMenuFont[512];
extern MCODE_KANJI_ANIME PadSymbolFontA[12];

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

/* static */ int  _JPFont_GetSubtCode(u_char *str, SUBT_CODE *subt_code);
static int  _EGFont_GetSubtCode(u_char *str, SUBT_CODE *subt_code, MCODE_DAT *pfnt_ascii);
/* static */ void _PKFontPut(SPR_PKT pk, SPR_PRM *spr, SUBT_CODE *psubt, int line_num, int xp, int yp, int pflg, int hsize, float rtx, float rty);

static MCODE_DAT* codeKanjiACheck(u_char dat0, u_char dat1, MCODE_KANJI_ANIME *kcode_pp, int kcode_max);
static void euc2sjis(unsigned char *c1, unsigned char *c2);

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

#ifndef NON_MATCHING
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
    tex0 |= SCE_GS_SET_TEX0(0, 0, 0, 0, 0, 1 /* RGBA */, 0, 0, 0, 0, 0, 0);

    MnSubtFontInfo[2].tex0 = tex0;
    MnSubtFontInfo[2].abgr = SCE_GS_SET_RGBAQ(128, 128, 128, 128, 0);

    if (lflg == LANG_JAPANESE)
        line = _JPFont_GetSubtCode(str, subt_code);
    else
        line = _EGFont_GetSubtCode(str, subt_code, mcode_ascii_TmpMenuFont);

    if (line != 0)
    {
        spr->rgba0 = abgr;

        _AnimeFontFlg = ((MNSceneGetMusicFitTimer() % 40) >= 21);

        spr->zx = 1.0f;
        spr->zy = D_003990C8; /* Requires lit4 to be split to match */
        
        _PKFontPut(pk, spr, subt_code, line, x, y, flg, 26, spr->zx, spr->zy);
    }
}
#endif

void MENUFontPutL(SPR_PKT pk, SPR_PRM *spr, int x, int y, u_int abgr, int flg, u_char *str)
{
    SUBT_CODE subt_code[16] = {};
    int       line;
    u_long    tex0;

    line = _EGFont_GetSubtCode(str, subt_code, mcode_ascii_TmpMenuFont);
    if (line != 0)
    {
        MnSubtFontInfo[0].tex0 = SubtGsTex0_TmpMenuFont[0];
        MnSubtFontInfo[0].abgr = abgr;

        MnSubtFontInfo[1].tex0 = SubtGsTex0_TmpMenuFont[1];
        MnSubtFontInfo[1].abgr = abgr;

        tex0 = TsGetTM2Hed(1)->GsTex0;
        tex0 |= SCE_GS_SET_TEX0(0, 0, 0, 0, 0, 1 /* RGBA */, 0, 0, 0, 0, 0, 0);

        MnSubtFontInfo[2].tex0 = tex0;
        MnSubtFontInfo[2].abgr = SCE_GS_SET_RGBAQ(128, 128, 128, 128, 0);

        _AnimeFontFlg = ((MNSceneGetMusicFitTimer() % 40) >= 21);

        spr->zx = 1.0f;
        spr->zy = 0.5f;

        _PKFontPut(pk, spr, subt_code, line, x, y, flg, 24, spr->zx, spr->zy);
    }
}

INCLUDE_ASM(const s32, "menu/menufont", MENUFontPutS);

INCLUDE_ASM(const s32, "menu/menufont", MENUFontPutR);

INCLUDE_ASM(const s32, "menu/menufont", _PKFontPut);

INCLUDE_ASM(const s32, "menu/menufont", _PADArrow_Put);
#if 0
static void _PADArrow_Put(/* s6 22 */ SPR_PKT pk, /* s0 16 */ SPR_PRM *spr, /* a2 6 */ MCODE_DAT *pfnt, /* s5 21 */ int x, /* s4 20 */ int y)
{
    /* s3 19 */ int i;
    /* s2 18 */ int aflg;
    /* s1 17 */ MCODE_DAT *pat;
    
    if (pfnt->v & 0x10)
    {
        aflg = _PadArrowState;
    }
    else
    {
        aflg = pfnt->v;
    }

    pat = ArowPat;

    spr->px = x;
    spr->py = y;

    spr->ux = pat->u;
    spr->uy = pat->v;

    spr->uw = pat->w;
    spr->uh = pat->h;
    spr->sw = pat->w;
    spr->sh = pat->h;

    PkNSprite_Add2(pk, spr, 3);

    for (i = 0; i < 3; i++)
    {
        if (aflg & 1)
        {
            spr->px = x;
            spr->py = y;

            spr->ux = pat[i].u;
            spr->uy = pat[i].v;

            spr->uw = pat[i].w;
            spr->uh = pat[i].h;
            spr->sw = pat[i].w;
            spr->sh = pat[i].h;
    
            PkNSprite_Add2(pk, spr, 3);
        }

        aflg >>= 1;
    }
}
#endif

INCLUDE_ASM(const s32, "menu/menufont", _JPFont_GetSubtCode);

static int _EGFont_GetSubtCode(u_char *str, SUBT_CODE *subt_code, MCODE_DAT *pfnt_ascii)
{
    /* s3 19 */ int line_num;
    /* s2 18 */ MCODE_CHAR *ppMcode;
    /* s0 16 */ u_char c;
    /* -0x80(sp) */ u_char dat0;
    /* -0x7f(sp) */ u_char dat1;

    if (*str == '\0')
        return 0;

    line_num = 0;
    ppMcode = mcode_dat_pp_TmpMenuFont;

    while (1)
    {
        c = *str++;

        if (c == 0)
        {
            line_num++;
            break;
        }

        if (c == '@')
            line_num++;

        if (c >= 32)
        {
            if (_PadFont_SW != 0)
            {
                dat0 = c;
                dat1 = *str;
                euc2sjis(&dat0, &dat1);

                ppMcode->pmcode = codeKanjiACheck(dat0, dat1, PadSymbolFontA, 12);
                if (ppMcode->pmcode != NULL)
                {
                    str++;

                    subt_code[line_num].cnt++;
                    subt_code[line_num].wsize += ppMcode->pmcode->w;

                    ppMcode->flg = 2;
                    ppMcode++;
                    continue;
                }
            }

            ppMcode->pmcode = &pfnt_ascii[c - 32];
            subt_code[line_num].wsize += ppMcode->pmcode->w;
            subt_code[line_num].cnt++;

            ppMcode->flg = 0;
            ppMcode++;
        }
    }

    return line_num;
}

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
