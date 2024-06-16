#include "main/pack.h"

u_int PackGetDecodeSize(u_char *fp_r)
{
    return *(u_int*)fp_r;
}

INCLUDE_ASM(const s32, "main/pack", PackDecode);

#if 1
INCLUDE_ASM(const s32, "main/pack", PackGetAdrs);
#else
int PackGetAdrs(/* a0 4 */ u_int adrs, /* a1 5 */ int num)
{
    if (PACK(adrs)->pack_header.fcnt <= num)
        return -1;
}
#endif

INCLUDE_ASM(const s32, "main/pack", PackDbgList);
