#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"

#include <stdio.h>

#include <eekernel.h>
#include <sifrpc.h>
#include <sifdev.h>
#include <../libcdvd.h>
#include <libdma.h>
#include <eestruct.h>
#include <libgraph.h>
#include <devvif0.h>
#include <devvu0.h>
#include <sifcmd.h>

#include "os/mtc.h"
#include "os/syssub.h"
#include "os/cmngifpk.h"
#include "main/main.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448

typedef struct { // 0x100
    /* 0x00 */ sceDmaTag dmatag;
    /* 0x10 */ sceGifTag giftag;
    /* 0x20 */ sceGsDrawEnv1 drEnv;
    /* 0xa0 */ sceGsClear clear;
} CLEAR_VRAM_DMA;

extern sceGsDBuffDc DBufDc;
extern int outbuf_idx;

extern sceGsDrawEnv1* drawEnvP[5];

extern PADD pad[2];

int SetIopModule(void);
void initSystem(void);
void exitSystem(void);
void SetOsFuncAddr(void* func_pp);
void osFunc(void);
void systemCtrlMain(void* xx);
void mallocInit(void);

#endif // SYSTEM_H
