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

void CG_WaveInitEasy(WAVE_STR *wstr, short int x, short int y, short int w, short int h, WMODE_ENUM wmode)
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
    sceGifPkAddGsAD(wavePkSpr, SCE_GS_RGBAQ, 0x80808080);
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

INCLUDE_ASM(const s32, "main/effect", UG_AlpDisp);

void CG_AlpDisp(PLH_STR *plh_pp, sceGsFrame *frame_pp, int pri)
{
    sceGifPacket alpPkSpr;
    
    CmnGifOpenCmnPk(&alpPkSpr);
    UG_AlpDisp(plh_pp, frame_pp, &alpPkSpr);
    CmnGifCloseCmnPk(&alpPkSpr, pri);
}

INCLUDE_ASM(const s32, "main/effect", UG_MozaikuDisp);

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
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_ALPHA_1, 0x44);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_CLAMP_1, 5);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_COLCLAMP, 1);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_PABE, 0);

    if (texFr_pp == NULL)
    {
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEST_1, 0x31001);
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(fade_pp->r, fade_pp->g, fade_pp->b, fade_pp->alp, 0));
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_PRIM, 0x146);
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, 0x179006c00);
        sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, 0x187009400);
        return;
    }

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEST_1, 0x3000d);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, fade_pp->alp, 0));

    /* FIXME: TEX0 probably wasn't written like this */
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_TEX0_1, SCE_GS_SET_TEX0(texFr_pp->FBP << 5, texFr_pp->FBW, 640, 0, 8, 0, 1, 0, 0, 0, 0, 0));
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_PRIM, 0x156);

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_UV, 0);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, 0x179006c00);

    sceGifPkAddGsAD(fadePkSpr, SCE_GS_UV, 0xe002800);
    sceGifPkAddGsAD(fadePkSpr, SCE_GS_XYZ2, 0x187009400);
}

INCLUDE_ASM(const s32, "main/effect", UG_FadeDisp2);

void CG_FadeDisp(FADE_MAKE_STR *fade_pp, int pri, sceGsFrame *texFr_pp)
{
    sceGifPacket fadePkSpr;
  
    CmnGifOpenCmnPk(&fadePkSpr);
    UG_FadeDisp(fade_pp,&fadePkSpr,texFr_pp);
    CmnGifCloseCmnPk(&fadePkSpr,pri);
}

INCLUDE_ASM(const s32, "main/effect", UG_NoodlesDisp);

void CG_NoodlesDisp(NOODLES_STR *ndl_pp, sceGsFrame *frame_pp, int pri, int time)
{
    sceGifPacket noodlesPkSpr;
  
    CmnGifOpenCmnPk(&noodlesPkSpr);
    UG_NoodlesDisp(ndl_pp, frame_pp, &noodlesPkSpr, time);
    CmnGifCloseCmnPk(&noodlesPkSpr, pri);
}

INCLUDE_RODATA(const s32, "main/effect", D_00393400);

/* NOTE: Uses the scratchpad */
INCLUDE_ASM(const s32, "main/effect", FD_MonocroDisp);
