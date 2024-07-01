#ifndef P3MC_H
#define P3MC_H

#include "common.h"

#include <eetypes.h>
#include <libmc.h>

typedef struct { // 0x14
    /* 0x00 */ u_int scDate[2];
    /* 0x08 */ u_char name[8];
    /* 0x10 */ u_int score;
} P3MC_RANKSCORE;

typedef struct { // 0x4c4
    /* 0x000 */ int nSplay;
    /* 0x004 */ P3MC_RANKSCORE splay[20];
    /* 0x194 */ int nVplay[4];
    /* 0x1a4 */ P3MC_RANKSCORE vplay[4][10];
} P3MC_STAGERANK;

typedef struct { // 0x2664
    /* 0x0000 */ char name[12];
    /* 0x000c */ char name1[12];
    /* 0x0018 */ char name2[12];
    /* 0x0024 */ u_short stageNo;
    /* 0x0026 */ u_short fileNo;
    /* 0x0028 */ u_char flg;
    /* 0x0029 */ u_char mode;
    /* 0x002a */ u_char isVs;
    /* 0x002b */ u_char vsLev;
    /* 0x002c */ u_int score;
    /* 0x0030 */ u_int score2;
    /* 0x0034 */ u_short roundNo;
    /* 0x0036 */ u_char winner;
    /* 0x0037 */ u_char pads;
    /* 0x0038 */ u_int pad[1];
    /* 0x003c */ u_char date_day;
    /* 0x003d */ u_char date_month;
    /* 0x003e */ u_short date_year;
    /* 0x0040 */ u_char date_pad;
    /* 0x0041 */ u_char date_second;
    /* 0x0042 */ u_char date_minute;
    /* 0x0043 */ u_char date_hour;
    /* 0x0044 */ P3MC_STAGERANK stageRank[8];
} USER_DATA;

typedef struct { // 0x2684
    /* 0x0000 */ char header[16];
    /* 0x0010 */ USER_DATA user;
    /* 0x2674 */ char footer[16];
} USER_HEADER;

typedef struct { // 0x10
    /* 0x0 */ char footer[16];
} USER_FOOTER;

typedef struct { // 0x1c
    /* 0x00 */ u_char *pMemTop;
    /* 0x04 */ u_int rwsize;
    /* 0x08 */ u_int datasize;
    /* 0x0c */ u_int srcsize;
    /* 0x10 */ USER_HEADER *pHead;
    /* 0x14 */ void *pData;
    /* 0x18 */ USER_FOOTER *pFoot;
} MCRWDATA_HDL;

typedef struct { // 0xc2980
    /* 0x00000 */ int nGetUser;
    /* 0x00004 */ USER_DATA getUser[81];
    /* 0xc25a8 */ int logPage_flg;
    /* 0xc25ac */ int repPage_flg;
    /* 0xc25b0 */ int nLogGet;
    /* 0xc25b4 */ int nRepGet;
    /* 0xc25b8 */ USER_DATA *plog_user[80];
    /* 0xc26f8 */ USER_DATA *prep_user[80];
    /* 0xc2838 */ int nUserMax;
    /* 0xc283c */ USER_DATA *pUserTbl[81];
} P3MC_USRLST;

typedef struct { // 0x20
    /* 0x00 */ int prg;
    /* 0x04 */ int dstat;
    /* 0x08 */ int data_no;
    /* 0x0c */ int data_mode;
    /* 0x10 */ int data_stage;
    /* 0x14 */ int prgflag;
    /* 0x18 */ MCRWDATA_HDL *dhdl;
    /* 0x1c */ void *data_cfunc;
} P3MC_WORK;

typedef int (*P3MCDataCheckFunc)();

typedef struct { // 0x3b00
    /* 0x0000 */ int curState;
    /* 0x0004 */ int curMode;
    /* 0x0008 */ int curFno;
    /* 0x000c */ int curUserMode;
    /* 0x0010 */ int bFirst;
    /* 0x0014 */ MCRWDATA_HDL chkData;
    /* 0x0030 */ P3MC_USRLST *pUserLst;
    /* 0x0034 */ u_char UserHeadTmp[9860];
    /* 0x26c0 */ sceMcTblGetDir dirTable[81];
} GETUSER_WORK;

#endif