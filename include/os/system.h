#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"

#include <eekernel.h>
#include <sifrpc.h>
#include <sifdev.h>
#include <../libcdvd.h>
#include <libdma.h>
#include <eestruct.h>
#include <libgraph.h>
#include <devvif0.h>
#include <devvu0.h>

#include "os/mtc.h"
#include "main/main.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448

// TODO(poly): move PAD_ID and PADD to syssub.h, probably?
typedef enum {
	PAD_ENUM_NONE = 0,
	PAD_ENUM_NEG = 32,
	PAD_ENUM_NORMAL = 64,
	PAD_ENUM_DSHOCK = 112,
	PAD_ENUM_DSHOCK2 = 121,
	PAD_ENUM_ANA = 80
} PAD_ID;

typedef struct { // 0x24
	/* 0x00 */ PAD_ID padId;
	/* 0x04 */ u_short old;
	/* 0x06 */ u_short shot;
	/* 0x08 */ u_short one;
	/* 0x0a */ u_short off;
	/* 0x0c */ u_char ana[4];
	/* 0x10 */ u_char press[12];
	/* 0x1c */ u_char padvib[2];
	/* 0x1e */ u_short mshot;
	/* 0x20 */ u_short mone;
} PADD;

typedef struct { // 0x100
	/* 0x00 */ sceDmaTag dmatag;
	/* 0x10 */ sceGifTag giftag;
	/* 0x20 */ sceGsDrawEnv1 drEnv;
	/* 0xa0 */ sceGsClear clear;
} CLEAR_VRAM_DMA;

int SetIopModule(void);
void initSystem(void);
void exitSystem(void);
void SetOsFuncAddr(void *func_pp);
void osFunc(void);
void systemCtrlMain(void *xx);
void mallocInit(void);

// TODO(poly): move to syssub.c
void GPadInit(void);
void GPadRead(PADD *pad_pp);
void GPadExit(void);
void usrMallcInit(void);
void SetBackColor(u_char R, u_char G, u_char B);

// TODO(poly): move to cmngifpk.c
void CmnGifInit(void *buf_adr, int size);
void CmnGifClear(void);

#endif
