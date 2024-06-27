#ifndef SUBT_H
#define SUBT_H

#include "common.h"

#include "os/cmngifpk.h"
#include "main/etc.h"

typedef struct { // 0x8
    /* 0x0 */ int cnt;
    /* 0x4 */ int wsize;
} SUBT_CODE;

typedef struct { // 0x10
    /* 0x0 */ u_int starTime;
    /* 0x4 */ u_int endTime;
    /* 0x8 */ u_char *txtData[2];
} JIMAKU_DAT;

typedef struct { // 0x8
    /* 0x0 */ int size;
    /* 0x4 */ JIMAKU_DAT *jimaku_dat_pp;
} JIMAKU_STR;

typedef struct { // 0xa
    /* 0x0 */ u_short u;
    /* 0x2 */ u_short v;
    /* 0x4 */ u_short w;
    /* 0x6 */ u_short h;
    /* 0x8 */ char adjx;
    /* 0x9 */ char adjy;
} MCODE_DAT;

typedef struct { // 0xc
    /* 0x0 */ u_short code;
    /* 0x2 */ u_short u;
    /* 0x4 */ u_short v;
    /* 0x6 */ u_short w;
    /* 0x8 */ u_short h;
    /* 0xa */ char adjx;
    /* 0xb */ char adjy;
} MCODE_KANJI;

typedef struct { // 0x4
    /* 0x0 */ int mcode_max;
    /* 0x4 */ MCODE_KANJI mcode_kanji[0];
} MCODE_STR;

typedef MCODE_DAT MCODE_ASCII;
typedef u_char *MESS[2];

#define CHECK_LANG(lang) (lang == LANG_JAPANESE)

void SubtInit(void);
void* SubtKanjiSet(void *adrs);
void SubtClear();
void SubtFlash();
void SubtMcodeSet(int code);

// poly: comment this out for the moment
// MCODE_DAT* codeKanjiCheck(u_char dat0, u_char dat1);

void SubtMsgPrint(u_char *msg_pp, int xp, int yp, int jap_flag, int mline);
void SubtCtrlInit(void *adrs, int ser_f);
void SubtCtrlPrint(JIMAKU_STR *jstr_pp, int line, int time, int lang);
void SubtTapPrint(u_char *tap_msg_pp, int lang);
void SubtMenuCtrlInit(void *adrs);
void SubtMenuCtrlPrint(u_char *msg_pp, int xp, int yp, int lang);

int SubtMsgDataKaijyouCnt(u_char *msg_pp, int jap_flag);
u_char* SubtMsgDataPos(u_char *msg_pp, int jap_flag, int pos);

void SubtTapPrintWake(u_char *tap_msg_pp, int lang, int lng, int nowp);
void SubtCtrlPrintBoxyWipe(JIMAKU_STR *jstr_pp, int line, int time, int lang, void *code_pp);

#endif