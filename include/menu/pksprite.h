#ifndef PKSPRITE_H
#define PKSPRITE_H

#include "common.h"

#include <eetypes.h>
#include <eestruct.h>
#include <libdma.h>
#include <libvu0.h>

typedef struct { // 0x40
    /* 0x00 */ sceDmaTag tag[3];
    /* 0x30 */ u_int PaketTop;
} TSPAKET;

typedef struct { // 0xc
    /* 0x0 */ u_int isAlloc;
    /* 0x4 */ u_int *top;
    /* 0x8 */ u_int size;
} TS_WORKMEM;

typedef struct { // 0xa0
    /* 0x00 */ TSPAKET pkt[2];
    /* 0x80 */ u_int idx;
    /* 0x84 */ u_int size;
    /* 0x88 */ u_int ptop;
    /* 0x8c */ u_int btop;
    /* 0x90 */ u_int next;
    /* 0x94 */ TS_WORKMEM mem;
} TsUSERPKT;

typedef u_long128 **SPR_PKT;

typedef struct { // 0x18
    /* 0x00 */ float centerX;
    /* 0x04 */ float centerY;
    /* 0x08 */ float zoomX;
    /* 0x0c */ float zoomY;
    /* 0x10 */ int isOn;
    /* 0x14 */ int pad;
} SPR_ZOOM;

typedef struct { // 0x90
    /* 0x00 */ int px;
    /* 0x04 */ int py;
    /* 0x08 */ int sw;
    /* 0x0c */ int sh;
    /* 0x10 */ float ofsx;
    /* 0x14 */ float ofsy;
    /* 0x18 */ float zx;
    /* 0x1c */ float zy;
    /* 0x20 */ int ux;
    /* 0x24 */ int uy;
    /* 0x28 */ int uw;
    /* 0x2c */ int uh;
    /* 0x30 */ float cx;
    /* 0x34 */ float cy;
    /* 0x38 */ float rot;
    /* 0x3c */ u_int zdepth;
    /* 0x40 */ u_int rgba0;
    /* 0x44 */ u_int rgba1;
    /* 0x48 */ u_int rgba2;
    /* 0x4c */ u_int rgba3;
    /* 0x50 */ float px0;
    /* 0x54 */ float py0;
    /* 0x58 */ float px1;
    /* 0x5c */ float py1;
    /* 0x60 */ float px2;
    /* 0x64 */ float py2;
    /* 0x68 */ float px3;
    /* 0x6c */ float py3;
    /* 0x70 */ SPR_ZOOM zoom;
} SPR_PRM;

typedef struct { // 0x18
    /* 0x00 */ float u;
    /* 0x04 */ float v;
    /* 0x08 */ float x;
    /* 0x0c */ float y;
    /* 0x10 */ float ofsx;
    /* 0x14 */ float ofsy;
} PKMSPT;

typedef struct { // 0x1c
    /* 0x00 */ int mw;
    /* 0x04 */ int mh;
    /* 0x08 */ int px;
    /* 0x0c */ int py;
    /* 0x10 */ int sw;
    /* 0x14 */ int sh;
    /* 0x18 */ PKMSPT *pmspt;
} PKMESH;

typedef struct { // 0x40
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long prim;
    /* 0x18 */ u_long rgba;
    /* 0x20 */ u_long uv0;
    /* 0x28 */ u_long xyz2_0;
    /* 0x30 */ u_long uv1;
    /* 0x38 */ u_long xyz2_1;
} SprTagTF;

typedef struct { // 0x30
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long prim;
    /* 0x18 */ u_long rgba;
    /* 0x20 */ u_long xyz2_0;
    /* 0x28 */ u_long xyz2_1;
} SprTagCF;

typedef struct { // 0x60
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long prim;
    /* 0x18 */ u_long rgba;
    /* 0x20 */ u_long uv0;
    /* 0x28 */ u_long xyz2_0;
    /* 0x30 */ u_long uv1;
    /* 0x38 */ u_long xyz2_1;
    /* 0x40 */ u_long uv2;
    /* 0x48 */ u_long xyz2_2;
    /* 0x50 */ u_long uv3;
    /* 0x58 */ u_long xyz2_3;
} SprTagTFR;

typedef struct { // 0x40
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long prim;
    /* 0x18 */ u_long rgba;
    /* 0x20 */ u_long xyz2_0;
    /* 0x28 */ u_long xyz2_1;
    /* 0x30 */ u_long xyz2_2;
    /* 0x38 */ u_long xyz2_3;
} SprTagCFR;

typedef struct { // 0x40
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long prim;
    /* 0x18 */ u_long rgba;
    /* 0x20 */ u_long xyz2_0;
    /* 0x28 */ u_long xyz2_1;
    /* 0x30 */ u_long xyz2_2;
    /* 0x38 */ u_long pad;
} TriTagCFR;

typedef struct { // 0x30
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long prim;
    /* 0x18 */ u_long rgba;
    /* 0x20 */ u_long xyz2_0;
    /* 0x28 */ u_long xyz2_1;
} SprTagLF;

typedef struct { // 0x40
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long prim;
    /* 0x18 */ u_long rgba;
    /* 0x20 */ qword GifCord2;
    /* 0x30 */ u_long xyz3;
    /* 0x38 */ u_long reg;
} SprTagLSF;

typedef struct { // 0x20
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long rgba;
    /* 0x18 */ u_long xyz2_0;
} SprTagLSFN;

typedef struct { // 0x60
    /* 0x00 */ qword GifCord;
    /* 0x10 */ u_long prim;
    /* 0x18 */ u_long rgba0;
    /* 0x20 */ u_long xyz2_0;
    /* 0x28 */ u_long rgba1;
    /* 0x30 */ u_long xyz2_1;
    /* 0x38 */ u_long rgba2;
    /* 0x40 */ u_long xyz2_2;
    /* 0x48 */ u_long rgba3;
    /* 0x50 */ u_long xyz2_3;
    /* 0x58 */ u_long pad;
} SprTagCG;

#endif