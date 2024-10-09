#ifndef MBAR_H
#define MBAR_H

#include "common.h"
#include <eetypes.h>

#include "main/scrctrl.h"

typedef enum {
    MBAR_NONE = 0,
    MBAR_TEACHER = 393,
    MBAR_PARAPPA = 745,
    MBAR_TEACHER_HOOK = 393,
    MBAR_PARAPPA_HOOK = 1769,
    MBAR_TEACHER_VS = 105,
    MBAR_PARAPPA_VS = 553,
    MBAR_BOXY_VS = 2089
} MBAR_REQ_ENUM;

typedef struct { // 0x24
    /* 0x00 */ MBAR_REQ_ENUM mbar_req_enum;
    /* 0x04 */ TAPSET *tapset_pp;
    /* 0x08 */ int current_time;
    /* 0x0c */ int scr_tap_memory_cnt;
    /* 0x10 */ SCR_TAP_MEMORY *scr_tap_memory_pp;
    /* 0x14 */ int lang;
    /* 0x18 */ int tapdat_size;
    /* 0x1c */ TAPDAT *tapdat_pp;
    /* 0x20 */ GUI_CURSOR_ENUM gui_cursor_enum;
} MBAR_REQ_STR;

typedef enum {
    MBC_NONE = 0,
    MBC_OTEHON_TOP = 1,
    MBC_TR = 1,
    MBC_CI = 2,
    MBC_XX = 3,
    MBC_SQ = 4,
    MBC_LL = 5,
    MBC_RR = 6,
    MBC_SP = 7,
    MBC_OTEHON_M_TOP = 8,
    MBC_M_TR = 8,
    MBC_M_CI = 9,
    MBC_M_XX = 10,
    MBC_M_SQ = 11,
    MBC_M_LL = 12,
    MBC_M_RR = 13,
    MBC_M_SP = 14,
    MBC_OTEHON_BW_TOP = 15,
    MBC_BW_TR = 15,
    MBC_BW_CI = 16,
    MBC_BW_XX = 17,
    MBC_BW_SQ = 18,
    MBC_BW_LL = 19,
    MBC_BW_RR = 20,
    MBC_BW_SP = 21,
    MBC_BALL = 22,
    MBC_STAR = 23,
    MBC_TEA_FACE_TOP = 24,
    MBC_NONECUR = 24,
    MBC_PARA = 25,
    MBC_BOXY = 26,
    MBC_STAGE1 = 27,
    MBC_STAGE2 = 28,
    MBC_STAGE3 = 29,
    MBC_STAGE4 = 30,
    MBC_STAGE5 = 31,
    MBC_STAGE6 = 32,
    MBC_STAGE7 = 33,
    MBC_STAGE8 = 34,
    MBC_FLASH = 35,
    MBC_GLINE_P = 36,
    MBC_GLINE_T = 37,
    MBC_MAX = 38
} MBC_ENUM;

typedef struct { // 0x18
    /* 0x00 */ MBC_ENUM mbc_enum;
    /* 0x04 */ int xp;
    /* 0x08 */ int yp;
    /* 0x0c */ float sclx;
    /* 0x10 */ float scly;
    /* 0x14 */ u_char r;
    /* 0x15 */ u_char g;
    /* 0x16 */ u_char b;
    /* 0x17 */ u_char a;
} MBARR_CHR;

void MbarSetCtrlTime(int mctime);
void MbarReq(MBAR_REQ_ENUM mm_req, TAPSET *ts_pp, int curr_time, SCR_TAP_MEMORY *tm_pp, int tm_cnt, int lang, int tapdat_size, TAPDAT *tapdat_pp, GUI_CURSOR_ENUM guic);

#endif