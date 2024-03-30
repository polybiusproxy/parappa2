#include "main/subt.h"

/* data */
u_long SubtGsTex0[3];

/* sdata */
int SUBT_POSX;
int SUBT_POSY;
/* static */ int subtSetNum;

/* sbss */
/* static */ MCODE_STR *kanji_pp;

/* bss - static */
sceGifPacket subtPkSpr;
SUBT_CODE subt_code[16];
MCODE_DAT *mcode_dat_pp[256];

void SubtInit(void)
{
    subtSetNum = 0;
}

void* SubtKanjiSet(void *adrs)
{
	void *ret = kanji_pp;

    kanji_pp = adrs;
    return ret;
}

INCLUDE_ASM(const s32, "main/subt", SubtClear);

void SubtFlash(void)
{
    if (subtSetNum)
        CmnGifCloseCmnPk(&subtPkSpr, 9);
}

void SubtMcodeSet(int code)
{
    sceGifPkAddGsAD(&subtPkSpr, SCE_GS_TEX0_1, SubtGsTex0[code]);
    sceGifPkAddGsAD(&subtPkSpr, SCE_GS_TEX1_1, SCE_GS_SET_TEX1_1(0, 0, 0, 1, 1, 0, 0));
    sceGifPkAddGsAD(&subtPkSpr, SCE_GS_TEXA, SCE_GS_SET_TEXA(0, 1, 128));
}

MCODE_DAT* codeKanjiCheck(u_char dat0, u_char dat1)
{
    u_short      code     = dat0 | (dat1 << 8);
    MCODE_KANJI *kcode_pp = &kanji_pp->mcode_kanji;
    int          i        = 0;

    for (i = 0; i < kanji_pp->mcode_max; i++, kcode_pp++)
    {
        if (kcode_pp->code == code)
            return (MCODE_DAT*)(&kcode_pp->u);
    }

    return NULL;
}

/* Same function, matching lines too */
/* https://github.com/JunkBox-Library/JunkBox_Lib/blob/255f34bccfd0fab817d71730f6b45cab52062ec8/Lib/tools.c#L2579 */
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

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/subt", SubtMsgPrint);
#else
void SubtMsgPrint(/* s0 16 */ u_char *msg_pp, /* -0xac(sp) */ int xp, /* -0xa8(sp) */ int yp, 
                    /* s3 19 */ int jap_flag, /* s4 20 */ int mline)
{
    /* s0 16 */ u_char *tmp_pp;
    /* s6 22 */ int line_num;
    /* a2 6 */ int i;
    /* s2 18 */ int j;
    /* -0xa4(sp) */ int hsize;
    /* s2 18 */ int cnt_all;
    /* -0xb0(sp) */ u_char dat0;
    /* -0xaf(sp) */ u_char dat1;
    /* s1 17 */ int posx;
    /* s5 21 */ int posy;
    /* s0 16 */ MCODE_DAT *mcode_pp;

    posx = xp;
    posy = yp;

    SubtMcodeSet(jap_flag);

    if (*msg_pp != '\0')
    {
        cnt_all = 0;
        WorkClear(subt_code, sizeof(SUBT_CODE));

        tmp_pp = msg_pp;
        hsize = 13; // 0xD

        if (tmp_pp)
        {
            while (tmp_pp != '\0')
            {
                line_num = 1;
                if (jap_flag)
                {
                    dat0 = msg_pp[0];
                    dat1 = msg_pp[1];

                    // New line
                    if (dat0 == '@')
                    {
                        line_num++;
                        cnt_all = line_num;
                    }
                    else if (dat0 != ' ')
                    {
                        euc2sjis(&dat0, (dat0 | 1));

                        if ((dat0 == '\x81') && (dat1 == '\x97'))
                        {
                            line_num++;
                        }
                        else
                        {
                            mcode_pp = codeKanjiCheck(dat0, dat1);
                            mcode_dat_pp[cnt_all] = mcode_pp;

                            if (mcode_pp != NULL)
                            {
                                subt_code[cnt_all].cnt++;
                                subt_code[cnt_all].wsize += mcode_pp->w;
                            }
                        }

                        tmp_pp++;
                    }
                }
            }
        }
        else
            line_num = 1;

        if (mline && mline < line_num)
            line_num = mline;

        for (i = 0; i < line_num; i++)
        {
            for (j = 0; j < subt_code[i].cnt; j++)
            {
                sceGifPkAddGsAD(&subtPkSpr, SCE_GS_PRIM, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 1, 0, 0, 0, 0));

                sceGifPkAddGsAD(&subtPkSpr, SCE_GS_UV, SCE_GS_SET_UV(mcode_dat_pp[i]->u << 4, mcode_dat_pp[i]->v << 4));
                sceGifPkAddGsAD(&subtPkSpr, SCE_GS_XYZ2, 0); // TODO

                sceGifPkAddGsAD(&subtPkSpr, SCE_GS_UV, SCE_GS_SET_UV(mcode_dat_pp[i]->u << 4, mcode_dat_pp[i]->v << 4));
                sceGifPkAddGsAD(&subtPkSpr, SCE_GS_XYZ2, 0); // TODO
                subtSetNum = 1;
            }
        }
    }
}
#endif

void SubtCtrlInit(void *adrs, int ser_f)
{
    SubtInit();
    SubtKanjiSet(adrs);

    if (ser_f)
    {
        SUBT_POSX = 2048;
        SUBT_POSY = 2122;
    }    
    else
    {
        SUBT_POSX = 2048;
        SUBT_POSY = 2104;
    }
}

void SubtCtrlPrint(JIMAKU_STR *jstr_pp, int line, int time, int lang)
{
    JIMAKU_STR *jstr_tmp_pp = &jstr_pp[line];
    int         i;

    for (i = 0; i < jstr_tmp_pp->size; i++)
    {
        if ((time >= jstr_tmp_pp->jimaku_dat_pp[i].starTime) 
         && (time <  jstr_tmp_pp->jimaku_dat_pp[i].endTime))
        {
            SubtClear();
            SubtMsgPrint(jstr_tmp_pp->jimaku_dat_pp[i].txtData[lang], SUBT_POSX, SUBT_POSY, CHECK_LANG(lang), 0);
            SubtFlash();
            return;
        }
    }
}

void SubtTapPrint(u_char *tap_msg_pp, int lang)
{
    // BUG: should be logical OR instead of bitwise?
    if ( tap_msg_pp == NULL | *tap_msg_pp == '\0' )
        return;

    SubtClear();
    SubtMsgPrint(tap_msg_pp, SUBT_POSX, SUBT_POSY, CHECK_LANG(lang), 2);
    SubtFlash();
}

void SubtMenuCtrlInit(void *adrs)
{
    SubtInit();
    SubtKanjiSet(adrs);
}

void SubtMenuCtrlPrint(u_char *msg_pp, int xp, int yp, int lang)
{
    xp += 1728;
    yp += 1936;

    SubtClear();
    SubtMsgPrint(msg_pp, xp, yp, CHECK_LANG(lang), 0);
    SubtFlash();
}

int SubtMsgDataKaijyouCnt(u_char *msg_pp, int jap_flag)
{
    u_char *tmp_pp;
    u_char  dat0;
    u_char  dat1;
    int     ret = 1;

    if (*msg_pp == '\0')
        return NULL;
    else
    {
        tmp_pp = msg_pp;

        for (; *tmp_pp != '\0';)
        {
            dat0 = tmp_pp[0];
            dat1 = tmp_pp[1];

            tmp_pp++;

            /* New line */
            if (dat0 == '@')
            {
                ret++;
            }
            /* Japanese subt. new line */
            else if (jap_flag)
            {
                euc2sjis(&dat0, &dat1);
                tmp_pp++;

                // '@' (SHIFT-JIS)
                if ( dat0 == 0x81 && dat1 == 0x97 )
                    ret++;
            }
        }
    }

    return ret;
}

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/subt", SubtMsgDataPos);
#else
u_char* SubtMsgDataPos(/* a0 4 */ u_char *msg_pp, /* s3 19 */ int jap_flag, /* s2 18 */ int pos)
{
    u_char *tmp_pp;
    u_char  dat0;
    u_char  dat1;
    int     ret = 0;

    if (*msg_pp == '\0')
        return NULL;
    else
    {
        tmp_pp = msg_pp;

        for (; *tmp_pp != '\0';)
        {
            dat0 = tmp_pp[0];
            dat1 = tmp_pp[1];

            tmp_pp++;

            /* New line */
            if (dat0 == '@')
            {
                ret++;
            }
            /* Japanese subt. new line */
            else if (jap_flag)
            {
                euc2sjis(&dat0, &dat1);
                tmp_pp++;

                // '@' (SHIFT-JIS)
                if ( dat0 == 0x81 && dat1 == 0x97 )
                    ret++;

                if (ret == pos)
                    return tmp_pp;
            }
        }
    }

    return NULL;

    //return NULL;
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/subt", SubtTapPrintWake);
#else
void SubtTapPrintWake(/* s1 17 */ u_char *tap_msg_pp, /* s4 20 */ int lang, /* s2 18 */ int lng, /* s0 16 */ int nowp)
{
    int cntmax;
    int selpos;

    if (nowp >= 0)
    {
        if (nowp > lng)
            selpos = nowp;
        else
            selpos = lng - 1;

        if (tap_msg_pp != NULL)
        {
            cntmax = SubtMsgDataKaijyouCnt(tap_msg_pp, CHECK_LANG(lang));
            if (cntmax >= 3) // (cntmax > 2) LOGIC
                selpos = SubtMsgDataPos(tap_msg_pp, CHECK_LANG(lang), (((cntmax + 1) / 2) * selpos) / lng << 1);

            SubtTapPrint(selpos, lang);
        }
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/subt", SubtCtrlPrintBoxyWipe);
#else
void SubtCtrlPrintBoxyWipe(/* a0 4 */ JIMAKU_STR *jstr_pp, /* a1 5 */ int line, 
/* a2 6 */ int time, /* s4 20 */ int lang, /* s0 16 */ void *code_pp)
{
	/* a0 4 */ int i;
	/* s1 17 */ void *kanjiset_tmp_pp;
	/* s3 19 */ JIMAKU_STR *jstr_tmp_pp = &jstr_pp[line];
    /* s0 16 */ int lang_f;

    for ( i = 0; i < jstr_tmp_pp->jimaku_dat_pp; i++ )
    {
        if ((time >= jstr_tmp_pp->jimaku_dat_pp[i].starTime) &&
            (time <  jstr_tmp_pp->jimaku_dat_pp[i].endTime))
        {
            SubtInit();
            kanjiset_tmp_pp = SubtKanjiSet(code_pp);

            lang_f = (lang == LANG_ENGLISH) ? 0 : 2;

            SubtClear();
            SubtMsgPrint(jstr_tmp_pp->jimaku_dat_pp[i].txtData[lang], 2048,
                        2122, lang_f, 0);

            SubtFlash();
            SubtKanjiSet(kanjiset_tmp_pp);
            return;
        }
    }
}
#endif