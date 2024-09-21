#include "main/effect.h"

/* lit4 - temp */
float FLT_00398F38; /* UG_WaveDisp    -> 6.2831855  */
float FLT_00398F3C; /* UG_NoodlesDisp -> 0.12822828 */

#define WV_SCREEN_W 640
#define WV_SCREEN_H 224

void CG_WaveInit(WAVE_STR *wstr)
{
    wstr->currentAng = 0.0f;
}

void CG_WaveInitEasy(WAVE_STR *wstr, short x, short y, short w, short h, WMODE_ENUM wmode)
{
    wstr->wmode = wmode;
    wstr->x = x;
    wstr->y = y;

    if (wmode == WM_WSLICE)
    {
        wstr->linecnt = h - 1;
        wstr->sizeW = w;
        wstr->sizeH = 1;
        
        wstr->addW = 0;
        wstr->addH = 1;

        wstr->u = x + ((WV_SCREEN_W / 2) - 2048);
        wstr->v = y + ((WV_SCREEN_H / 2) - 2047);

        wstr->addU = 0;
        wstr->addV = 1;
    }
    else
    {
        wstr->linecnt = w - 1;
        wstr->sizeW = 1;
        wstr->sizeH = h;

        wstr->addW = 1;
        wstr->addH = 0;

        wstr->u = x + ((WV_SCREEN_W / 2) - 2047);
        wstr->v = y + ((WV_SCREEN_H / 2) - 2048);

        wstr->addU = 1;
        wstr->addV = 0;
    }

    wstr->currentAng = 0.0f;
}

void UG_WaveDisp(WAVE_STR *wstr, sceGsFrame *frame_pp, sceGifPacket *wavePkSpr)
{
    int i;

    float tmpAngle;
    int haba_now_u, haba_now_v;
    int tmp_u, tmp_v;
    int tmp_x, tmp_y;

    sceGifPkAddGsAD(wavePkSpr, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(wavePkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, 128, 0));
    sceGifPkAddGsAD(wavePkSpr, SCE_GS_TEST_1, 0x30000);
    sceGifPkAddGsAD(wavePkSpr, SCE_GS_TEX0_1, SCE_GS_SET_TEX0(frame_pp->FBP << 5, frame_pp->FBW, 640, 0, 8, 0, 0, 0, 0, 0, 0, 0));
    sceGifPkAddGsAD(wavePkSpr, SCE_GS_TEX1_1, 0);
    sceGifPkAddGsAD(wavePkSpr, SCE_GS_PRIM, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 0, 1, 1, 0, 0));
    sceGifPkAddGsAD(wavePkSpr, SCE_GS_PRMODECONT, 1);

    tmpAngle = wstr->currentAng;

    for (i = 0; i < wstr->linecnt; i++)
    {
        haba_now_u = (wstr->mvSize * sinf(tmpAngle) + wstr->mvSize) * 16.0f;
        haba_now_v = haba_now_u;

        if (wstr->wmode == WM_WSLICE)
            haba_now_v = 0;
        else
            haba_now_u = 0;

        tmp_u = wstr->addU * i + wstr->u;
        tmp_v = wstr->addV * i + wstr->v;
        sceGifPkAddGsAD(wavePkSpr, SCE_GS_UV,   SCE_GS_SET_UV((tmp_u << 4) + haba_now_u, (tmp_v << 4) + haba_now_v));

        tmp_x = wstr->x + wstr->addW * i;
        tmp_y = wstr->y + wstr->addH * i;
        sceGifPkAddGsAD(wavePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ(tmp_x << 4, tmp_y << 4, 1));

        tmp_u += (wstr->sizeW - wstr->mvSize * 2);
        tmp_v += (wstr->sizeH - wstr->mvSize * 2);
        sceGifPkAddGsAD(wavePkSpr, SCE_GS_UV,   SCE_GS_SET_UV((tmp_u << 4) + haba_now_u, (tmp_v << 4) + haba_now_v));
        sceGifPkAddGsAD(wavePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ((tmp_x + wstr->sizeW) << 4, (tmp_y + wstr->sizeH) << 4, 1));

        tmpAngle += wstr->plsAng1line;
    }

    wstr->currentAng += wstr->plsAng1time;

    if (wstr->currentAng >= FLT_00398F38)
        wstr->currentAng -= FLT_00398F38;
}

void CG_WaveDisp(WAVE_STR *wstr, sceGsFrame *frame_pp, int pri)
{
    sceGifPacket wavePkSpr;

    CmnGifOpenCmnPk(&wavePkSpr);
    UG_WaveDisp(wstr, frame_pp, &wavePkSpr);
    CmnGifCloseCmnPk(&wavePkSpr, pri);
}

INCLUDE_ASM("main/effect", UG_AlpDisp);
#if 0
void UG_AlpDisp(/* s0 16 */ PLH_STR *plh_pp, /* s1 17 */ sceGsFrame *frame_pp, /* s2 18 */ sceGifPacket *alpPkSpr)
{
    /* s7 23 */ int i;
    /* -0xc0(sp) */ short int ofs_tbl[4][2];
    /* t0 8 */ int tmp0;
    /* a2 6 */ int tmp1;

    sceGifPkAddGsAD(alpPkSpr, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(alpPkSpr, SCE_GS_PRMODECONT, 1);
    sceGifPkAddGsAD(alpPkSpr, SCE_GS_TEST_1, 0x30000);
    // alpha_1
    sceGifPkAddGsAD(alpPkSpr, SCE_GS_CLAMP_1, 0x37c009fc00a);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_COLCLAMP,1);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_PABE,0);
    // tex0_1
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_TEX1_1,0x60);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_TEXA,0x8000000080);
    sceGifPkAddGsAD(alpPkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(plh_pp->r, plh_pp->g, plh_pp->b, plh_pp->a, 0x3f800000));
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_PRIM,0x15c);

    for (i = 0; i < 4; i++)
    {

    }

    #if 0

    pasVar7 = ofs_tbl;
    pPVar12 = plh_pp->uvOfs;
    pfVar11 = &plh_pp->xyOfs[0].ofs1;
    pPVar10 = plh_pp->xyOfs;
    pfVar9 = &plh_pp->uvOfs[0].ofs1;
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_TEXFLUSH,0);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_PRMODECONT,1);
    i = 3;

    sceGifPkAddGsAD(alpPkSpr,SCE_GS_TEST_1,0x30000);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_ALPHA_1,(ulong)plh_pp->alp << 0x20 | 100);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_CLAMP_1,0x37c009fc00a);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_COLCLAMP,1);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_PABE,0);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_TEX0_1,
                    (long)(int)((SUB84(*frame_pp,0) & 0x1ff) << 5 |
                               (SUB84(*frame_pp,2) & 0x3f) << 0xe) | 0x228000000);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_TEX1_1,0x60);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_TEXA,0x8000000080);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_RGBAQ,
                    (ulong)plh_pp->r | 0x3f80000000000000 |
                    (ulong)plh_pp->b << 0x10 | (ulong)plh_pp->g << 8);
    sceGifPkAddGsAD(alpPkSpr,SCE_GS_PRIM,0x15c);
    psVar8 = (short *)((uint)ofs_tbl | 2);
    fVar15 = *pfVar9;
    while( true )
    {
        fVar14 = pPVar12->ofs0;
        puVar1 = (undefined *)((int)ofs_tbl[1] + 3);
        uVar4 = (uint)puVar1 & 7;
        puVar5 = (ulong *)(puVar1 + -uVar4);
        *puVar5 = *puVar5 & -1L << (uVar4 + 1) * 8 | 0x28000000000U >> (7 - uVar4) * 8;
        ofs_tbl[0][0] = 0;
        ofs_tbl[0][1] = 0;
        ofs_tbl[1][0] = 0x280;
        ofs_tbl[1][1] = 0;
        puVar1 = (undefined *)((int)ofs_tbl[3] + 3);
        uVar4 = (uint)puVar1 & 7;
        puVar5 = (ulong *)(puVar1 + -uVar4);
        *puVar5 = *puVar5 & -1L << (uVar4 + 1) * 8 | 0xe0028000e00000U >> (7 - uVar4) * 8;
        ofs_tbl[2][0] = 0;
        ofs_tbl[2][1] = 0xe0;
        ofs_tbl[3][0] = 0x280;
        ofs_tbl[3][1] = 0xe0;
        pfVar9 = pfVar9 + 2;
        pPVar12 = pPVar12 + 1;
        i = i + -1;
        sceGifPkAddGsAD(alpPkSpr,SCE_GS_UV,
                        (long)((int)(fVar14 * 16.0) + *(short *)pasVar7 * 0x10) & 0xffffU |
                        ((long)((int)(fVar15 * 16.0) + *psVar8 * 0x10) & 0xffffU) << 0x10);
        fVar15 = *pfVar11;
        pfVar6 = &pPVar10->ofs0;
        sVar2 = *psVar8;
        sVar3 = *(short *)pasVar7;
        pfVar11 = pfVar11 + 2;
        pPVar10 = pPVar10 + 1;
        psVar8 = psVar8 + 2;
        pasVar7 = (short int (*) [2])((int)pasVar7 + 4);
        sceGifPkAddGsAD(alpPkSpr,SCE_GS_XYZ2,
                        (long)((int)(*pfVar6 * 16.0) + (sVar3 + 0x6c0) * 0x10) & 0xffffU |
                        ((long)((int)(fVar15 * 16.0) + (sVar2 + 0x790) * 0x10) & 0xffffU) << 0x10 |
                        0x100000000);
        if (i < 0) break;
        fVar15 = *pfVar9;
    }

    #endif
}
#endif

void CG_AlpDisp(PLH_STR *plh_pp, sceGsFrame *frame_pp, int pri)
{
    sceGifPacket alpPkSpr;
    
    CmnGifOpenCmnPk(&alpPkSpr);
    UG_AlpDisp(plh_pp, frame_pp, &alpPkSpr);
    CmnGifCloseCmnPk(&alpPkSpr, pri);
}

/* TODO: Use the GS macros */
void UG_MozaikuDisp(MOZAIKU_STR *moz_pp, sceGsFrame *frame_pp, sceGifPacket *mozPkSpr)
{
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_PRMODECONT, 1);
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_TEST_1, 0x30000);
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, 128, 0x3f800000));

    sceGifPkAddGsAD(mozPkSpr, SCE_GS_CLAMP_1, SCE_GS_SET_CLAMP(15, 0, moz_pp->umsk & 0x3ff, moz_pp->ufix & 0x3ff, moz_pp->vmsk & 0x3ff, moz_pp->vfix & 0x3ff));

    sceGifPkAddGsAD(mozPkSpr, SCE_GS_COLCLAMP, 0);
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_PABE, 0);

    sceGifPkAddGsAD(mozPkSpr, SCE_GS_TEX0_1, SCE_GS_SET_TEX0(frame_pp->FBP << 5, frame_pp->FBW, 0, 10, 8, 0, 0, 0, 0, 0, 0, 0));
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_TEX1_1, SCE_GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_PRIM, 0x116);

    sceGifPkAddGsAD(mozPkSpr, SCE_GS_UV, 0xe002800);
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(GS_X_COORD(640), GS_Y_COORD(224), 0));

    sceGifPkAddGsAD(mozPkSpr, SCE_GS_UV, 0);
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(GS_X_COORD(0), GS_Y_COORD(0), 1));

    sceGifPkAddGsAD(mozPkSpr, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(mozPkSpr, SCE_GS_CLAMP_1, 0);
}

void CG_MozaikuDisp(MOZAIKU_STR *moz_pp, sceGsFrame *frame_pp, int pri)
{
    sceGifPacket mozPkSpr;
    
    CmnGifOpenCmnPk(&mozPkSpr);
    UG_MozaikuDisp(moz_pp, frame_pp, &mozPkSpr);
    CmnGifCloseCmnPk(&mozPkSpr, pri);
}

/* TODO: Use the GS macros */
void UG_FadeDisp(FADE_MAKE_STR *fade_pp, sceGifPacket *fadePkSpr, sceGsFrame *texFr_pp)
{
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_PRMODECONT, 1);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA(0, 1, 0, 1, 0));
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_CLAMP_1, 5);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_COLCLAMP, 1);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_PABE, 0);

    if (texFr_pp == NULL)
    {
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEST_1, 0x31001);
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(fade_pp->r, fade_pp->g, fade_pp->b, fade_pp->alp, 0));
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_PRIM, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 1, 0, 1, 0, 0));
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(GS_X_COORD(0), GS_Y_COORD(0), 1));
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(GS_X_COORD(640), GS_Y_COORD(224), 1));
        return;
    }

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEST_1, 0x3000d);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, fade_pp->alp, 0));

    /* FIXME: TEX0 probably wasn't written like this */
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEX0_1, SCE_GS_SET_TEX0(texFr_pp->FBP << 5, texFr_pp->FBW, 640, 0, 8, 0, 1, 0, 0, 0, 0, 0));
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_PRIM, 0x156);

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_UV, 0);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(GS_X_COORD(0), GS_Y_COORD(0), 1));

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_UV, 0xe002800);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(GS_X_COORD(640), GS_Y_COORD(224), 1));
}

#if 1
INCLUDE_ASM("main/effect", UG_FadeDisp2);
#else
void UG_FadeDisp2(/* s0 16 */ FADE_MAKE_STR *fade_pp, /* s3 19 */ sceGifPacket *fadePkSpr, /* s1 17 */ sceGsFrame *texFr_pp, /* f20 58 */ float scale)
{
    /* s1 17 */ int xp;
    /* s0 16 */ int yp;

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_PRMODECONT, 1);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA(0, 1, 0, 1, 0));
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_CLAMP_1, SCE_GS_SET_CLAMP(0, 0, 0, 640 - 1, 0, 224 - 1));
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_COLCLAMP, 1);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_PABE, 0);

    if (texFr_pp == NULL)
    {
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEST_1, 0x31001);

        sceGifPkAddGsAD(fadePkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(fade_pp->r, fade_pp->g, fade_pp->b, fade_pp->alp, 0));

        sceGifPkAddGsAD(fadePkSpr, SCE_GS_PRIM, 0x146);

        sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ(GS_X_COORD(0), GS_Y_COORD(0), 1));
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ(GS_X_COORD(640), GS_Y_COORD(224), 1));
        return;
    }

    #define PR_TXINT(x) (*(int*)&x)

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEST_1, 0x3000d);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, fade_pp->alp, 0));

    //sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEX0_1,
    //              (long)(int)((PR_TXINT(texFr_pp) & 0x1ff) << 5 | (PR_TXINT(texFr_pp) & 0x3f) << 0xe
    //                         ) | 0x628000000);

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEX0_1, SCE_GS_SET_TEX0(texFr_pp->FBP << 5, texFr_pp->FBW, 640, 0, 8, 0, 1, 0, 0, 0, 0, 0));
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_PRIM, 0x156);

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_UV, SCE_GS_SET_UV(0, 0));

    xp = scale * (640.0f / 2);
    yp = scale * (224.0f / 2);
    

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ((2048 - xp) << 4, (2048 - yp) << 4, 1));

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_UV, SCE_GS_SET_UV(0x2800, 0xe00));
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ((xp + 2048) << 4, (yp + 2048) << 4, 1));
}
#endif

void CG_FadeDisp(FADE_MAKE_STR *fade_pp, int pri, sceGsFrame *texFr_pp)
{
    sceGifPacket fadePkSpr;
  
    CmnGifOpenCmnPk(&fadePkSpr);
    UG_FadeDisp(fade_pp, &fadePkSpr, texFr_pp);
    CmnGifCloseCmnPk(&fadePkSpr,pri);
}

INCLUDE_ASM("main/effect", UG_NoodlesDisp);

void CG_NoodlesDisp(NOODLES_STR *ndl_pp, sceGsFrame *frame_pp, int pri, int time)
{
    sceGifPacket noodlesPkSpr;
  
    CmnGifOpenCmnPk(&noodlesPkSpr);
    UG_NoodlesDisp(ndl_pp, frame_pp, &noodlesPkSpr, time);
    CmnGifCloseCmnPk(&noodlesPkSpr, pri);
}

INCLUDE_RODATA("main/effect", D_00393400);

/* NOTE: Uses the scratchpad */
INCLUDE_ASM("main/effect", FD_MonocroDisp);
