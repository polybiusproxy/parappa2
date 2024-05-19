#include "iop_mdl/wp2cd_rpc.h"

#define DATA_SIZE_STRING (64)
#define DATA_SIZE_NORMAL (16)

static u_int sbuff[64] PR_ALIGNED(64);
static sceSifClientData gCd;

int WP2Init(void)
{
    int i;

    while (1)
    {
        if (sceSifBindRpc(&gCd, WP2_DEV, 0) < 0)
            while (1);

        i = 10000;
        while (i--);

        if (gCd.serve != 0)
            break;
    }

    return 1;
}

/* TODO(poly): Is this really a struct? */
typedef struct
{
    u_char sbuff[32];
    u_int val;
} rpcBuff;

int WP2Ctrl(int command, int data0)
{
    if (command == 0x8013 || command == 0x8014)
    {
        *(rpcBuff*)sbuff = *(rpcBuff*)data0;

        strcpy((char*)sbuff, (char*)data0);
        FlushCache(0);

        while (sceSifCallRpc(&gCd, command, 0, sbuff, DATA_SIZE_STRING, sbuff, 64, NULL, NULL))
            printf("sceSifCallRpc wp2cd miss!\n");
    }
    else
    {
        if (command == 0x8002)
        {
            strcpy((char*)sbuff, (char*)data0);
            FlushCache(0);

            while (sceSifCallRpc(&gCd, command, 0, (void*)data0, DATA_SIZE_STRING, sbuff, 64, 0, 0))
                printf("sceSifCallRpc wp2cd miss!\n");
        }
        else
        {
            sbuff[0] = data0;
            FlushCache(0);

            while (sceSifCallRpc(&gCd, command, 0, sbuff, DATA_SIZE_NORMAL, sbuff, 64, NULL, NULL))
                printf("sceSifCallRpc wp2cd miss!\n");
        }
    }
    
    return sbuff[0];
}
