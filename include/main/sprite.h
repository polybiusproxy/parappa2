#ifndef SPRITE_H
#define SPRITE_H

#include "common.h"

#include <eetypes.h>
#include <libgifpk.h>
#include <libdma.h>
#include <libgraph.h>

typedef struct { // 0x4
    /* 0x0 */ u_char r;
    /* 0x1 */ u_char g;
    /* 0x2 */ u_char b;
    /* 0x3 */ u_char a;
} VCLR_PARA;

typedef struct { // 0xc
    /* 0x0 */ u_short u0;
    /* 0x2 */ u_short v0;
    /* 0x4 */ u_short u1;
    /* 0x6 */ u_short v1;
    /* 0x8 */ u_short w;
    /* 0xa */ u_short h;
} LERO_TIM2_PT;

enum {
    LERO_ENUM_1 = 0,
    LERO_ENUM_2 = 1,
    LERO_ENUM_3 = 2,
    LERO_ENUM_4 = 3,
    LERO_ENUM_5 = 4,
    LERO_ENUM_LESSON = 5,
    LERO_ENUM_ROUND = 6
};

typedef struct { // 0xc
    /* 0x0 */ int tim2_num;
    /* 0x4 */ int posx;
    /* 0x8 */ int posy;
} LERO_POS_STR;

typedef struct { // 0x20
    /* 0x00 */ u_long GsTex0;
    /* 0x08 */ u_long GsTex1;
    /* 0x10 */ u_int GsRegs;
    /* 0x14 */ u_int GsTexClut;
    /* 0x18 */ u_short w;
    /* 0x1a */ u_short h;
    /* 0x1c */ u_short pad[2];
} TIM2_DAT;

typedef struct { // 0x18
    /* 0x00 */ u_long GsTex0;
    /* 0x08 */ u_long GsTex1;
    /* 0x10 */ u_int GsRegs;
    /* 0x14 */ u_int GsTexClut;
} SPR_DAT;

typedef struct { // 0x4
    /* 0x0 */ u_int ta0:8;
    /* 0x1 */ u_int res0:7;
    /* 0x1 */ u_int aem:1;
    /* 0x2 */ u_int ta1:8;
    /* 0x3 */ u_int res1:6;
    /* 0x3 */ u_int pabe:1;
    /* 0x3 */ u_int fba:1;
} SPR_REGS;

typedef struct { // 0x10
    /* 0x0 */ short int x;
    /* 0x2 */ short int y;
    /* 0x4 */ short int scalex;
    /* 0x6 */ short int scaley;
    /* 0x8 */ short int u;
    /* 0xa */ short int v;
    /* 0xc */ short int w;
    /* 0xe */ short int h;
} SPR_PRIM;

void SprInit(void);
void SprClear(void);
void SprPackSet(SPR_DAT *spr_pp);
void SprFlash(void);
void SprSetColor(u_char r, u_char g, u_char b, u_char a);
void SprDatPrint(SPR_DAT *spr_pp);

void SprDisp(SPR_PRIM *prm_pp);
void SprDispAlp(SPR_PRIM *prm_pp);
void SprDispZABnclr(void);
void SprDispZBnclr(void);
void SprDispZcheck(void);
void SprDispAcheck(int flg);
void SprDispAlphaSet(void);

void SprBox(SPR_PRIM *prm_pp);
void SprWindow(u_int x, u_int y, u_int w, u_int h);
void SprWindowDf(void);

#endif