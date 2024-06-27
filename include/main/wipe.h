#ifndef WIPE_H
#define WIPE_H

#include "common.h"

#include <prlib/prlib.h>

typedef enum {
    STW_TURN_IN = 0,
    STW_TURN_WAIT = 1,
    STW_TURN_OUT = 2,
    STW_PARAtoMINI = 3,
    STW_PARAtoBIG = 4,
    STW_YESNO = 5,
    STW_BOXY_IN = 6,
    STW_BOXY_WAIT = 7,
    STW_MAX = 8
} SNDTAP_WIPE_ENUM;

typedef enum
{
    WIPE_TYPE_LOADING = 0,
    WIPE_TYPE_SAME = 1,
    WIPE_TYPE_YES_NO = 2,
    WIPE_TYPE_PARA = 3,
    WIPE_TYPE_BOXY = 4,
    WIPE_TYPE_BOXY_WAIT = 5,
    WIPE_TYPE_MAX = 6
} WIPE_TYPE;

typedef struct { // 0x8
    /* 0x0 */ int frame;
    /* 0x4 */ int data;
} WIPE_SCRATCH_TBL;

typedef struct { // 0x8
    /* 0x0 */ int frt_size;
    /* 0x4 */ WIPE_SCRATCH_TBL *frt_pp;
} WIPE_SCRATCH_CTRL;

typedef enum {
    WSHC_IN = 0,
    WSHC_LOOP = 1,
    WSHC_IN_MOVE = 2,
    WSHC_LOOP_MOVE = 3,
    WSHC_OUT = 4,
    WSHC_OUT_MOVE = 5,
    WSHC_MAX = 6
} WSHC_ENUM;

typedef struct { // 0x20
    /* 0x00 */ int spmmap;
    /* 0x04 */ int spamap;
    /* 0x08 */ int spamapP;
    /* 0x0c */ PR_MODELHANDLE spmHdl;
    /* 0x10 */ PR_ANIMATIONHANDLE spaHdl;
    /* 0x14 */ PR_ANIMATIONHANDLE spaHdlP;
    /* 0x18 */ int *frame_pp;
    /* 0x1c */ int *frame_ppP;
} LDMAP;

typedef enum {
    LDMAP_TURN = 0,
    LDMAP_LOGO = 1,
    LDMAP_RECODE = 2,
    LDMAP_RECODE_LT = 3,
    LDMAP_LABEL = 4
} LDMAP_ENUM;

void WipeOutReq(void);
int WipeEndCheck(void);

#endif