#ifndef WIPE_H
#define WIPE_H

#include "common.h"

#include <eetypes.h>

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