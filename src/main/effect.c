#include "main/effect.h"

/* lit4 - temp */
float FLT_00398F38;
float FLT_00398F3C;

void CG_WaveInit(WAVE_STR *wstr)
{
    wstr->currentAng = 0.0f;
}

INCLUDE_ASM(const s32, "main/effect", CG_WaveInitEasy);

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
