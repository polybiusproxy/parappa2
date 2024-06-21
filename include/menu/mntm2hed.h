#ifndef MNTM2HED_H
#define MNTM2HED_H

#include "common.h"
#include <eetypes.h>

typedef struct { // 0x30
    /* 0x00 */ int w;
    /* 0x04 */ int h;
    /* 0x08 */ int type;
    /* 0x0c */ int pad;
    /* 0x10 */ u_long GsTex0;
    /* 0x18 */ u_long GsTex1;
    /* 0x20 */ u_long GsReg;
    /* 0x28 */ u_long GsClut;
} MENU_TM2_HED;

MENU_TM2_HED* TsGetTM2Hed(int no);

#endif