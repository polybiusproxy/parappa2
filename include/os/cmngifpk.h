#ifndef CMNGIFPK_H
#define CMNGIFPK_H

#include "common.h"

#include <eetypes.h>
#include <libgifpk.h>
#include <libdma.h>
#include <libgraph.h>

typedef struct { // 0x8
	/* 0x0 */ u_long128 *pBase;
	/* 0x4 */ int pri;
} CMNGIF_PRI;

void CmnGifInit(void *buf_adr, int size);
void CmnGifClear(void);
void CmnGifFlush(void);

int CmnGifSetData(sceGifPacket *gifpk_pp, int pri);

int CmnGifOpenCmnPk(sceGifPacket *gifpk_pp);
int CmnGifCloseCmnPk(sceGifPacket *gifpk_pp, int pri);

u_long128* CmnGifAdrsGet(void);
int CmnGifAdrsEnd(u_long128 *adr);

void CmnGifADPacketMake(sceGifPacket *gifP_pp, sceGsFrame *gsframe_pp);
void CmnGifADPacketMake2(sceGifPacket *gifP_pp, sceGsFrame *gsframe_pp);
int CmnGifADPacketMakeTrans(sceGifPacket *gifP_pp);

#endif