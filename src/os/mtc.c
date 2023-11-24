#include "common.h"

int mtcCurrentTask;
int mtcSemaEnd;
short int th_id_Ctrl;

INCLUDE_ASM(const s32, "os/mtc", mtcStackErrorCheck);

INCLUDE_ASM(const s32, "os/mtc", MtcChangeThCtrl);

INCLUDE_ASM(const s32, "os/mtc", MtcInit);

INCLUDE_ASM(const s32, "os/mtc", MtcQuit);

INCLUDE_ASM(const s32, "os/mtc", MtcStart);

INCLUDE_ASM(const s32, "os/mtc", MtcExec);

INCLUDE_ASM(const s32, "os/mtc", MtcWait);

INCLUDE_ASM(const s32, "os/mtc", MtcKill);

INCLUDE_ASM(const s32, "os/mtc", MtcPause);

INCLUDE_ASM(const s32, "os/mtc", MtcContinue);

INCLUDE_ASM(const s32, "os/mtc", MtcExit);

INCLUDE_ASM(const s32, "os/mtc", MtcGetCondition);

INCLUDE_ASM(const s32, "os/mtc", MtcResetCheck);
