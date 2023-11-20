#include "dbug/dbgmsg.h"

extern int msg8x8data[0];
extern TIM2INFO tinfo;
extern sceGifPacket gifPacket;
extern u_long128 dbgPacket[4096];

sceDmaChan* dbgDmaC;
u_char MSGCOL[3];
u_short MSGSIZE[2];
u_int MSGZPOP;

void DbgMsgInit(void) {
	GetTim2Info(&msg8x8data, &tinfo, 1);
	Tim2Load(&tinfo, 0x3fef, 0x3fff);
	
	sceGifPkInit(&gifPacket, dbgPacket);
	dbgDmaC = sceDmaGetChan(SCE_DMA_GIF);
	
	MSGCOL[2] = 128;
	MSGCOL[1] = 128;
	MSGCOL[0] = 128;
	
	MSGSIZE[0] = 0x100;
	MSGSIZE[1] = 0xa0;
	
	MSGZPOP = 0x7fffff;
}

void DbgMsgClear(void) {
	u_long giftag[2] = { SCE_GIF_SET_TAG(0, 0, 0, 0, 0, 1), 0x000000000000000eL };
	
	sceGifPkReset(&gifPacket);
	sceGifPkCnt(&gifPacket, 0, 0, 0);
	
	sceGifPkOpenGifTag(&gifPacket, *(u_long128*)&giftag);
	
	sceGifPkAddGsAD(&gifPacket, SCE_GS_TEXFLUSH, 0);
	sceGifPkAddGsAD(&gifPacket, SCE_GS_TEX0_1, tinfo.picturH->GsTex0);
	sceGifPkAddGsAD(&gifPacket, SCE_GS_TEX1_1, tinfo.picturH->GsTex1);
	sceGifPkAddGsAD(&gifPacket, SCE_GS_CLAMP_1, 0);
	sceGifPkAddGsAD(&gifPacket, SCE_GS_TEXCLUT, tinfo.picturH->GsTexClut);
	sceGifPkAddGsAD(&gifPacket, SCE_GS_ALPHA_1, 0x44);
	sceGifPkAddGsAD(&gifPacket, SCE_GS_PRIM, 0x156);
	
	sceGifPkAddGsAD(&gifPacket, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(MSGCOL[0], MSGCOL[1], MSGCOL[2], 128, 0x3f800000));
	sceGifPkAddGsAD(&gifPacket, SCE_GS_PABE, 0);
	sceGifPkAddGsAD(&gifPacket, SCE_GS_TEST_1, 0x3000d);
	sceGifPkAddGsAD(&gifPacket, SCE_GS_TEXA, 0x8000008000);
}

void DbgMsgFlash(void) {
	u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1), 0x000000000000000eL };
	
	sceGifPkCloseGifTag(&gifPacket);
	sceGifPkOpenGifTag(&gifPacket, *(u_long128*)&giftag);
	sceGifPkCloseGifTag(&gifPacket);
	
	sceGifPkEnd(&gifPacket, 0, 0, 0);
	sceGifPkTerminate(&gifPacket);
	
	FlushCache(0);
	
	sceDmaSend(dbgDmaC, gifPacket.pBase);
	sceGsSyncPath(0, 0);
}

/* (poly): should we use u_char? */
void DbgMsgSetColor(char r, char g, char b) {
	MSGCOL[0] = r;
	MSGCOL[1] = g;
	MSGCOL[2] = b;
	
	sceGifPkAddGsAD(&gifPacket, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(MSGCOL[0], MSGCOL[1], MSGCOL[2], 128, 0x3f800000));
}

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgSetSize);

INCLUDE_ASM(const s32, "dbug/dbgmsg", msgOutYY);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgPrint);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgPrintUserPkt);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgClearUserPkt);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgSetColorUserPkt);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgSetZ);
