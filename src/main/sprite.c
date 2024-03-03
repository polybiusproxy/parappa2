#include "main/sprite.h"

/* sdata - static */
int sprSetNum;

/* bss - static */
u_long128 sprPacket[2048];
sceGifPacket gifPkSpr;

/* sbss - static */
sceDmaChan *sprDmaC;

void SprInit(void)
{
    sceGifPkInit(&gifPkSpr, &sprPacket);
    sprDmaC = sceDmaGetChan(SCE_DMA_GIF);
    sprSetNum = 0;
}

INCLUDE_ASM(const s32, "main/sprite", SprClear);

INCLUDE_ASM(const s32, "main/sprite", SprPackSet);

INCLUDE_ASM(const s32, "main/sprite", SprFlash);

INCLUDE_ASM(const s32, "main/sprite", SprSetColor);

INCLUDE_RODATA(const s32, "main/sprite", D_00393018);

INCLUDE_RODATA(const s32, "main/sprite", D_00393028);

INCLUDE_ASM(const s32, "main/sprite", SprDatPrint);

INCLUDE_ASM(const s32, "main/sprite", SprDisp);

INCLUDE_ASM(const s32, "main/sprite", SprDispAlp);

INCLUDE_ASM(const s32, "main/sprite", SprDispZABnclr);

INCLUDE_ASM(const s32, "main/sprite", SprDispZBnclr);

INCLUDE_ASM(const s32, "main/sprite", SprDispZcheck);

INCLUDE_ASM(const s32, "main/sprite", SprDispAcheck);

INCLUDE_ASM(const s32, "main/sprite", SprDispAlphaSet);

INCLUDE_ASM(const s32, "main/sprite", SprBox);

INCLUDE_ASM(const s32, "main/sprite", SprWindow);

INCLUDE_ASM(const s32, "main/sprite", SprWindowDf);
