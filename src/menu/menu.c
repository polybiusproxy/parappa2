#include "common.h"

static int bFirst;
static int VsLev;

INCLUDE_ASM(const s32, "menu/menu", menuDraw);

INCLUDE_ASM(const s32, "menu/menu", menuDrawReq);

INCLUDE_ASM(const s32, "menu/menu", menuDrawQuit);

INCLUDE_ASM(const s32, "menu/menu", MenuMemCardCheck);

INCLUDE_ASM(const s32, "menu/menu", MenuCtrl);

INCLUDE_ASM(const s32, "menu/menu", P3LogInit);
