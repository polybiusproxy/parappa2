#ifndef WIPE_H
#define WIPE_H

#include "common.h"

#include <eetypes.h>

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

void WipeOutReq(void);
int WipeEndCheck(void);

#endif