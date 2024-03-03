#include "common.h"

#include <eetypes.h> /* tmp */
#include <sifdev.h>

u_int BekiDat(u_int size)
{
    u_int mot = 2;
    u_int i;
    
    for (i = 1; size > mot; i++)
    {
        mot = mot << 1;
    }

    return i;
}

INCLUDE_ASM(const s32, "dbug/vramsave", VramSave);

INCLUDE_RODATA(const s32, "dbug/vramsave", D_00390F30);

INCLUDE_RODATA(const s32, "dbug/vramsave", D_00390F48);

INCLUDE_ASM(const s32, "dbug/vramsave", VramSaveBMP);

INCLUDE_ASM(const s32, "dbug/vramsave", VramSaveBMPDouble);

INCLUDE_ASM(const s32, "dbug/vramsave", VramTmpSave);

INCLUDE_ASM(const s32, "dbug/vramsave", VramTmpSaveOutBMP);

INCLUDE_RODATA(const s32, "dbug/vramsave", D_00390F80);
