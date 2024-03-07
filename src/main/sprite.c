#include "main/sprite.h"

/* sdata - static */
int sprSetNum;

/* bss - static */
u_long128 sprPacket[2048];
sceGifPacket gifPkSpr;

/* sbss - static */
sceDmaChan *sprDmaC;

#define PR_TEX0(x) (*(sceGsTex0*)&x->GsTex0)
#define PR_TEX1(x) (*(sceGsTex1*)&x->GsTex1)
#define PR_REGS(x) (*(SPR_REGS*)&x->GsRegs)

void SprInit(void)
{
    sceGifPkInit(&gifPkSpr, sprPacket);
    sprDmaC = sceDmaGetChan(SCE_DMA_GIF);
    sprSetNum = 0;
}

void SprClear(void)
{
    u_long giftag[2] = { SCE_GIF_SET_TAG(0, 0, 0, 0, 0, 1), SCE_GIF_PACKED_AD };

    sceGifPkReset(&gifPkSpr);
    sceGifPkCnt(&gifPkSpr, 0, 0, 0);

    sceGifPkOpenGifTag(&gifPkSpr, *(u_long128*)giftag);
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, 128, 0));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(0, 0, 0, 0, 0, 0, 1, SCE_GS_ZALWAYS));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_PRMODECONT, SCE_GS_SET_PRMODECONT(1));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_CLAMP_1, SCE_GS_SET_CLAMP_1(1, 1, 0, 0, 0, 0));

    sprSetNum = 0;
}

void SprPackSet(SPR_DAT *spr_pp)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEX0_1, spr_pp->GsTex0);
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEX1_1, spr_pp->GsTex1);

    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEXA, SCE_GS_SET_TEXA(PR_REGS(spr_pp).ta0, PR_REGS(spr_pp).aem, PR_REGS(spr_pp).ta1));

    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_PABE,  PR_REGS(spr_pp).pabe);
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_FBA_1, PR_REGS(spr_pp).fba );
}

void SprFlash(void)
{
    u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1), SCE_GIF_PACKED_AD };

    if (sprSetNum != 0)
    {
        sceGifPkCloseGifTag(&gifPkSpr);
        sceGifPkOpenGifTag(&gifPkSpr, *(u_long128*)giftag);
        sceGifPkCloseGifTag(&gifPkSpr);

        sceGifPkEnd(&gifPkSpr, 0, 0, 0);
        sceGifPkTerminate(&gifPkSpr);

        FlushCache(0);
        sceDmaSend(sprDmaC, gifPkSpr.pBase);
        sceGsSyncPath(0, 0);
    }
}

void SprSetColor(u_char r, u_char g, u_char b, u_char a)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(r, g, b, a, 0x3f800000));
}

void SprDatPrint(SPR_DAT *spr_pp)
{
    printf("TBP0[%x] TBW[%x] PSM[%x] TW[%x] TH[%x] TCC[%x] \nTFX[%x] CBP[%x] CPSM[%x] CSM[%x] CSA[%x] CLD[%x]\n\n",
        PR_TEX0(spr_pp).TBP0, PR_TEX0(spr_pp).TBW, PR_TEX0(spr_pp).PSM,  PR_TEX0(spr_pp).TW,  PR_TEX0(spr_pp).TH,  PR_TEX0(spr_pp).TCC,
        PR_TEX0(spr_pp).TFX,  PR_TEX0(spr_pp).CBP, PR_TEX0(spr_pp).CPSM, PR_TEX0(spr_pp).CSM, PR_TEX0(spr_pp).CSA, PR_TEX0(spr_pp).CLD);

    printf("LCM[%x] MXL[%x] MMAG[%x] MMIN[%x] MTBA[%x] L[%x] K[%x]\n\n",
        PR_TEX1(spr_pp).LCM,  PR_TEX1(spr_pp).MXL, PR_TEX1(spr_pp).MMAG, PR_TEX1(spr_pp).MMIN, 
        PR_TEX1(spr_pp).MTBA, PR_TEX1(spr_pp).L,   PR_TEX1(spr_pp).K);

    printf("TA0[%x] AEM[%x] TA1[%x] PABE[%x] FBA[%x]\n",
        PR_REGS(spr_pp).ta0,  PR_REGS(spr_pp).aem, PR_REGS(spr_pp).ta1,  PR_REGS(spr_pp).pabe, PR_REGS(spr_pp).fba);
}

void SprDisp(SPR_PRIM *prm_pp)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_PRIM, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0));

    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_UV, SCE_GS_SET_UV((prm_pp->u + 1) << 4, (prm_pp->v + 1) << 4));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((prm_pp->x * 16) - (((prm_pp->w * prm_pp->scalex) / 2) >> 4),
                                                            (prm_pp->y * 16) - (((prm_pp->h * prm_pp->scaley) / 2) >> 4), 1));

    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_UV, SCE_GS_SET_UV((prm_pp->u + prm_pp->w - 1) << 4, (prm_pp->v + prm_pp->h - 1) << 4));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((prm_pp->x * 16) + (((prm_pp->w * prm_pp->scalex) / 2) >> 4),
                                                            (prm_pp->y * 16) + (((prm_pp->h * prm_pp->scaley) / 2) >> 4), 1));

    sprSetNum++;
}

void SprDispAlp(SPR_PRIM *prm_pp)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_PRIM, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 0, 0));

    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_UV, SCE_GS_SET_UV((prm_pp->u + 1) << 4, (prm_pp->v + 1) << 4));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((prm_pp->x * 16) - (((prm_pp->w * prm_pp->scalex) / 2) >> 4),
                                                            (prm_pp->y * 16) - (((prm_pp->h * prm_pp->scaley) / 2) >> 4), 1));

    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_UV, SCE_GS_SET_UV((prm_pp->u + prm_pp->w - 1) << 4, (prm_pp->v + prm_pp->h - 1) << 4));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((prm_pp->x * 16) + (((prm_pp->w * prm_pp->scalex) / 2) >> 4),
                                                            (prm_pp->y * 16) + (((prm_pp->h * prm_pp->scaley) / 2) >> 4), 1));

    sprSetNum++;
}

void SprDispZABnclr(void)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(1, 0, 0, 3, 0, 0, 1, SCE_GS_ZALWAYS));
}

void SprDispZBnclr(void)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(1, 0, 0, 1, 0, 0, 1, SCE_GS_ZALWAYS));
}

void SprDispZcheck(void)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(0, 0, 0, 0, 0, 0, 1, SCE_GS_ZGEQUAL));
}

void SprDispAcheck(int flg)
{
    if (flg)
        sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(1, 6, 0, 0, 0, 0, 1, SCE_GS_ZALWAYS)); // GREATER ATST register
    else
        sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(1, 3, 0, 0, 0, 0, 1, SCE_GS_ZALWAYS)); // LEQUAL ATST register
}

void SprDispAlphaSet(void)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA_1(0, 1, 0, 1, 0));
}

void SprBox(SPR_PRIM *prm_pp)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_TEST_1, SCE_GS_SET_TEST(1, 1, 0, 0, 0, 0, 1, 1));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA(1, 2, 0, 0, 0));
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_PRIM, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 1, 0, 0, 0, 0));

    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((prm_pp->x * 16) - (((prm_pp->w * prm_pp->scalex) / 2) >> 4),
                                                            (prm_pp->y * 16) - (((prm_pp->h * prm_pp->scaley) / 2) >> 4), 1));

    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((prm_pp->x * 16) + (((prm_pp->w * prm_pp->scalex) / 2) >> 4),
                                                            (prm_pp->y * 16) + (((prm_pp->h * prm_pp->scaley) / 2) >> 4), 1));

    sprSetNum++;
}

void SprWindow(u_int x, u_int y, u_int w, u_int h)
{
    sceGifPkAddGsAD(&gifPkSpr, SCE_GS_SCISSOR_1, SCE_GS_SET_SCISSOR_1(x, (x + w) - 1, y, (y + h) - 1));
}

void SprWindowDf(void)
{
    SprWindow(0, 0, 640, 224);
}
