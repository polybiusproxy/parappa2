#include "dbug/dbgmsg.h"

extern int msg8x8data[0];
extern TIM2INFO tinfo;
extern sceGifPacket gifPacket;
extern u_long128 dbgPacket[4096];
extern sceDmaChan* dbgDmaC;
extern u_char MSGCOL[3];
extern u_short MSGSIZE[2];
extern u_int MSGZPOP;

// INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgInit);
void DbgMsgInit(void) {
	GetTim2Info(&msg8x8data, &tinfo, 1);
    Tim2Load(&tinfo, 0x3fef, 0x3fff);
	
    sceGifPkInit(&gifPacket, dbgPacket);
    dbgDmaC = sceDmaGetChan(SCE_DMA_GIF);

    MSGCOL[0] = 128;
    MSGCOL[1] = 128;
    MSGCOL[2] = 128;
    
    MSGSIZE[0] = 0x100;
    MSGSIZE[1] = 0xa0;
    
    MSGZPOP = 0x7fffff;
}

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgClear);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgFlash);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgSetColor);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgSetSize);

INCLUDE_ASM(const s32, "dbug/dbgmsg", msgOutYY);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgPrint);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgPrintUserPkt);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgClearUserPkt);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgSetColorUserPkt);

INCLUDE_ASM(const s32, "dbug/dbgmsg", DbgMsgSetZ);

INCLUDE_RODATA(const s32, "dbug/dbgmsg", D_00390F00);

INCLUDE_RODATA(const s32, "dbug/dbgmsg", D_00390F10);
