#ifndef MEMC_H
#define MEMC_H

#include "common.h"

#include <eetypes.h>
#include <libmc.h>

typedef struct { // 0x340
    /* 0x000 */ u_int flag;
    /* 0x004 */ u_int stat;
    /* 0x008 */ int port;
    /* 0x00c */ int slot;
    /* 0x010 */ int type;
    /* 0x014 */ int free;
    /* 0x018 */ int cmd;
    /* 0x01c */ int func;
    /* 0x020 */ int fd;
    /* 0x024 */ int retry;
    /* 0x028 */ int format;
    /* 0x02c */ char *buf;
    /* 0x030 */ int size;
    /* 0x034 */ int isChange;
    /* 0x038 */ int seek;
    /* 0x03c */ int size2;
    /* 0x040 */ u_int oldOWClust;
    /* 0x044 */ u_int sizeOW;
    /* 0x048 */ int bChkSys;
    /* 0x04c */ int isSyncClose;
    /* 0x050 */ char filename[64];
    /* 0x090 */ int fileNo;
    /* 0x0c0 */ sceMcTblGetDir curDir[8];
    /* 0x2c0 */ char saveDir[64];
    /* 0x300 */ int iconSize1;
    /* 0x304 */ u_char *iconData1;
    /* 0x308 */ int iconSize2;
    /* 0x30c */ u_char *iconData2;
    /* 0x310 */ int iconSize3;
    /* 0x314 */ u_char *iconData3;
    /* 0x318 */ int sysFileSize;
} MEMC_STAT;

typedef struct { // 0x18
    /* 0x00 */ u_int flag;
    /* 0x04 */ int free;
    /* 0x08 */ int allfile;
    /* 0x0c */ int savefile;
    /* 0x10 */ int dirfileMax;
    /* 0x14 */ sceMcTblGetDir *dirfile;
} MEMC_INFO;

#endif