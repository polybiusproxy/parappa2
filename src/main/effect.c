#include "main/effect.h"

/* lit4 - temp */
float FLT_00398F38;
float FLT_00398F3C;

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

INCLUDE_ASM(const s32, "main/effect", UG_WaveDisp);

INCLUDE_ASM(const s32, "main/effect", CG_WaveDisp);

INCLUDE_ASM(const s32, "main/effect", UG_AlpDisp);

INCLUDE_ASM(const s32, "main/effect", CG_AlpDisp);

INCLUDE_ASM(const s32, "main/effect", UG_MozaikuDisp);

INCLUDE_ASM(const s32, "main/effect", CG_MozaikuDisp);

INCLUDE_ASM(const s32, "main/effect", UG_FadeDisp);

INCLUDE_ASM(const s32, "main/effect", UG_FadeDisp2);

INCLUDE_ASM(const s32, "main/effect", CG_FadeDisp);

INCLUDE_ASM(const s32, "main/effect", UG_NoodlesDisp);

INCLUDE_ASM(const s32, "main/effect", CG_NoodlesDisp);

INCLUDE_RODATA(const s32, "main/effect", D_00393400);

INCLUDE_ASM(const s32, "main/effect", FD_MonocroDisp);
