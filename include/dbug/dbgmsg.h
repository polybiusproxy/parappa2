#ifndef DBGMSG_H
#define DBGMSG_H

#include "common.h"

#include <libgraph.h>
#include <libgifpk.h>
#include <libdma.h>

#include "os/tim2.h"

void DbgMsgInit(void);
void DbgMsgClear(void);
void DbgMsgFlash(void);

void DbgMsgSetColor(u_char r, u_char g, u_char b);
void DbgMsgSetSize(u_short sw, u_short sh);

void DbgMsgPrint(u_char* m_pp, u_short x, u_short y);
void DbgMsgPrintUserPkt(u_char* m_pp, u_short x, u_short y, sceGifPacket* usrPacket_pp);
void DbgMsgClearUserPkt(sceGifPacket* usrPacket_pp);

void DbgMsgSetColorUserPkt(u_char r, u_char g, u_char b, sceGifPacket* usrPacket_pp);
void DbgMsgSetZ(int z);

#endif