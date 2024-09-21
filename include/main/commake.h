#ifndef COMMAKE_H
#define COMMAKE_H

#include "common.h"

#include <eetypes.h>

typedef struct { // 0x4
	/* 0x0 */ u_short keyId;
	/* 0x2 */ short int timeOfs;
} CM_STR;

typedef struct { // 0x1c4
	/* 0x000 */ CM_STR cm_str_mt[32];
	/* 0x080 */ CM_STR cm_str_now[32];
	/* 0x100 */ CM_STR cm_str_make[32];
	/* 0x180 */ int keyKind;
	/* 0x184 */ int keyKindNum;
	/* 0x188 */ int keyCnt_mt[7];
	/* 0x1a4 */ int keyCnt_now[7];
	/* 0x1c0 */ int maxBox;
} CM_STR_CTRL;

#endif