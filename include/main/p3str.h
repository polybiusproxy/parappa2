#ifndef P3STR_H
#define P3STR_H

#include "common.h"

#include <eetypes.h>

#include "os/syssub.h"
#include "os/system.h"
#include "os/tim2.h"
#include "main/drawctrl.h"
#include "main/etc.h"

#include <prlib/prlib.h>

#include <stdio.h>

#define ADRD(x) ((ADRD*)&(x)->adrD)

typedef struct { // 0x10
    /* 0x0 */ u_int trpos;
    /* 0x4 */ u_int read_pos;
    /* 0x8 */ u_int press_size;
    /* 0xc */ u_int pad;
} TRBOX_TR;

typedef struct { // 0x20
    /* 0x00 */ short int od_type;
    /* 0x02 */ short int first;
    /* 0x04 */ u_int start;
    /* 0x08 */ u_int end;
    /* 0x0c */ short int use;
    /* 0x0e */ short int draw;
    /* 0x10 */ u_int pri;
    /* 0x14 */ u_int pad1;
    /* 0x18 */ u_int pad2;
    /* 0x1c */ u_int adr;
} P3SRT_OD;

typedef struct { // 0x110
    /* 0x000 */ u_int read_size;
    /* 0x004 */ u_int buf_size;
    /* 0x008 */ u_int seek;
    /* 0x00c */ u_short ODcnt;
    /* 0x00e */ u_short channel;
    /* 0x010 */ TRBOX_TR trbox_tr[16];
    /* 0x110 */ P3SRT_OD p3srt_od[0];
} P3STR;

typedef struct { // 0x10
    /* 0x0 */ u_int trSize;
    /* 0x4 */ u_int trAdr;
    /* 0x8 */ u_int pad1;
    /* 0xc */ u_int pad2;
} P3STR_TRH;

typedef enum {
    OD_NONE = 0,
    OD_COMMON = 1,
    OD_VRAM = 2,
    OD_SCENE = 3,
    OD_DISPIN = 4,
    OD_VRAMCLR = 5,
    OD_FADE = 6,
    OD_END = 7,
    OD_MAX = 8
} OD_TYPE;

typedef enum {
    AF_SPM = 0,
    AF_SPA = 1,
    AF_SPC = 2,
    AF_SPF = 3,
    AF_TM2 = 4,
    AF_CL2 = 5,
    AF_NONE = 6
} ADRD_FTYPE;

typedef struct { // 0x10
    /* 0x0 */ u_int adrs;
    /* 0x4 */ u_int handle;
    /* 0x8 */ u_int ftype;
    /* 0xc */ int common;
} ADRD;

typedef enum {
    SD_VRAM_AW = 0,
    SD_VRAM_R1 = 1,
    SD_VRAM_R2 = 2,
    SD_VRAM_R3 = 3,
    SD_VRAM_R4 = 4
} SD_VRAM;

typedef enum {
    STE_MDL = 0,
    STE_ANI = 1,
    STE_ANIPOS = 2,
    STE_CAM = 3,
    STE_TM2 = 4,
    STE_CL2 = 5,
    STE_MAX = 6
} SCENE_TYPE_ENUM;

typedef struct { // 0x20
    /* 0x00 */ u_int type;
    /* 0x04 */ u_int start;
    /* 0x08 */ int adr1num;
    /* 0x0c */ int adr2num;
    /* 0x10 */ u_int top;
    /* 0x14 */ u_int end;
    /* 0x18 */ u_int pad1;
    /* 0x1c */ u_int pad2;
} SD_SCENE;

typedef struct { // 0xc
    /* 0x0 */ u_int frame;
    /* 0x4 */ u_int xp;
    /* 0x8 */ u_int yp;
} SD_DISPIN_SUB;

typedef struct { // 0x10
    /* 0x0 */ u_short u;
    /* 0x2 */ u_short v;
    /* 0x4 */ u_short w;
    /* 0x6 */ u_short h;
    /* 0x8 */ u_int subDcnt;
    /* 0xc */ u_int subDadr;
    /* 0x10 */ SD_DISPIN_SUB subD[0];
} SD_DISPIN;

typedef struct { // 0x8
    /* 0x0 */ int frame;
    /* 0x4 */ int alp;
} SD_FADE_SUB;

typedef struct { // 0xc
    /* 0x0 */ u_char r;
    /* 0x1 */ u_char g;
    /* 0x2 */ u_char b;
    /* 0x3 */ u_char pad;
    /* 0x4 */ int subDcnt;
    /* 0x8 */ u_int subDadr;
    /* 0xc */ SD_FADE_SUB subD[0];
} SD_FADE;

typedef struct { // 0x20
    /* 0x00 */ u_short type;
    /* 0x02 */ u_short first;
    /* 0x04 */ int usrD;
    /* 0x08 */ u_int usrDsize;
    /* 0x0c */ u_int adrD;
    /* 0x10 */ u_int adrDsize;
    /* 0x14 */ u_int dataD;
    /* 0x18 */ u_int dataDsize;
    /* 0x1c */ u_int Dcnt;
} P3STR_SD;

typedef struct { // 0xc
    /* 0x0 */ P3SRT_OD *p3o_pp;
    /* 0x4 */ P3STR_SD *p3sd_pp;
    /* 0x8 */ int subtime;
} P3STR_SORT;

#endif