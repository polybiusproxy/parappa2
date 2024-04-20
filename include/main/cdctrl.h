#ifndef CDCTRL_H
#define CDCTRL_H

#include "common.h"

#include <eetypes.h>
#include <libcdvd.h>
#include <sifdev.h>
#include <eekernel.h>

#include <stdio.h>

#include "os/syssub.h"
#include "os/usrmem.h"
#include "os/tim2.h"
#include "os/mtc.h"

#include "main/etc.h"

typedef enum {
    FRMODE_PC = 0,
    FRMODE_CD = 1,
    FRMODE_MAX = 2
} FREAD_MODE;

typedef enum {
    FTMODE_INTG = 0,
    FTMODE_WP2 = 1,
    FTMODE_ETC = 2,
    FTMODE_XTR = 3,
    FTMODE_MAX = 4
} FTYPE_MODE;

typedef struct { // 0x2c
    /* 0x00 */ u_char frmode; /* use with FREAD_MODE */
    /* 0x01 */ u_char ftmode; /* use with FTYPE_MODE */
    /* 0x02 */ u_char mchan;
    /* 0x03 */ u_char search;
    /* 0x04 */ u_char *fname;
    /* 0x08 */ sceCdlFILE fpCd;
} FILE_STR;

typedef enum {
    FT_NONE = 0,
    FT_VRAM = 1,
    FT_SND = 2,
    FT_ONMEM = 3,
    FT_R1 = 4,
    FT_R2 = 5,
    FT_R3 = 6,
    FT_R4 = 7,
    FT_PAD0 = 8,
    FT_PAD1 = 9,
    FT_PAD2 = 10,
    FT_PAD3 = 11,
    FT_PAD4 = 12,
    FT_PAD5 = 13,
    FT_PAD6 = 14,
    FT_PAD7 = 15,
    FT_MAX = 16
} FILE_TYPE_ENUM;

typedef struct { // 0x10
    /* 0x0 */ int fnum;
    /* 0x4 */ int ftype;
    /* 0x8 */ int f_size;
    /* 0xc */ int pad;
    /* 0x10 */ int adr[0];
} INT_FILE_STR;

typedef unsigned int u_adr;

typedef struct { // 0x20
    /* 0x00 */ int id;
    /* 0x04 */ int fnum;
    /* 0x08 */ int ftype;
    /* 0x0c */ int head_size;
    /* 0x10 */ int name_size;
    /* 0x14 */ int data_size;
    /* 0x18 */ int pad[2];
    /* 0x20 */ u_adr adr[0];
} PACKINT_FILE_STR;

typedef struct { // 0x18
    /* 0x00 */ u_int status;
    /* 0x04 */ u_int error_status;
    /* 0x08 */ u_int read_area;
    /* 0x0c */ int tmp_area;
    /* 0x10 */ FILE_STR *fstr_pp;
    /* 0x14 */ u_char wp2chan[2];
    /* 0x16 */ u_short volume;
} CDCTRL_STR;

u_int PackIntGetDecodeSize(u_char *fp_r);
int PackIntDecode(u_char *fp_r, u_char *fp_w);
int PackIntDecodeWait(u_char *fp_r, u_char *fp_w, int wait_hline);

void CdctrlInit(void);
void CdctrlQuit(void);
void CdctrlMasterVolSet(u_int vol);
int CdctrlSerch(FILE_STR *fstr_pp);

void intReadSub(void);

void cdctrlReadData(void *x);
void CdctrlRead(FILE_STR *fstr_pp, u_int buf, int tmpbuf);
void CdctrlReadOne(FILE_STR *fstr_pp, u_int buf, int tmpbuf);

void usrMemcpy(void *sakip, void *motop, int size);

void CdctrlMemIntgDecode(u_int rbuf, u_int setbuf);
int CdctrlStatus(void);

void CdctrlWP2SetVolume(u_short vol);

void CdctrlWp2GetSampleTmpBuf(void);
int CdctrlWp2GetSampleTmp(void);

void CdctrlSndFadeOut(int time);
int CdctrlSndFadeOutCheck(void);
void CdctrlSndFadeOutWait(int time);

// TODO(poly): move to iop_mdl/wp2cd_rpc.c once splitted
int WP2Init(void);
int WP2Ctrl(int command, int data0);

// TODO(poly): move to iop_mdl/tapctrl_rpc.c once splitted
int TapCt(int command, int data1, int data2);

#endif