#ifndef CDCTRL_H
#define CDCTRL_H

#include "common.h"

#include <eetypes.h>
#include <libcdvd.h>

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

void intReadSub(void);

void CdctrlMemIntgDecode(u_int rbuf, u_int setbuf);

#endif