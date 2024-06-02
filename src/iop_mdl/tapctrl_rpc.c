#include "iop_mdl/tapctrl_rpc.h"

#include <stdio.h>

#define DATA_SIZE_NORMAL (16)
#define TAPCT_DEV (0x8001)

static u_int sbuff[16] PR_ALIGNED(64);
static sceSifClientData gCd;

int TapCtInit(void)
{
    int i;

    while (1)
    {
        if (sceSifBindRpc(&gCd, TAPCT_DEV, 0) < 0)
            while (1);

        i = 10000;
        while (i--);

        if (gCd.serve != 0)
            break;
    }

    return 1;
}

int TapCt(int command, int data1, int data2)
{
    int rsize = 0;

    if (command & 0x8000)
        rsize = 64;

    sbuff[0] = data1;
    sbuff[1] = data2;

    while (sceSifCallRpc(&gCd, command, 0, sbuff, DATA_SIZE_NORMAL, sbuff, rsize, NULL, NULL))
        printf("sceSifCallRpc tapctrl miss!\n");

    return sbuff[0];
}
