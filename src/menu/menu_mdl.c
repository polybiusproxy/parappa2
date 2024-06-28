#include "common.h"

#include <eetypes.h>

/* sbss - static */
u_int AMusicFitTime;

/* lit4 */
float D_00398F4C;
float D_00398F50;
float D_00398F54;
float D_00398F58;
float D_00398F5C;

INCLUDE_ASM(const s32, "menu/menu_mdl", MNSceneMusicFitTimerClear);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNSceneMusicFitTimerFrame);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNSceneGetMusicFitTimer);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_Init);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_End);

INCLUDE_ASM(const s32, "menu/menu_mdl", SetDrawEnv12);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_Draw);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_DispSw);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_SetAnimete);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_StartAnime);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_ContinueAnime);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_StopAnime);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_ExecAnime);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_CopyState);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_CopyStateMdl);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_SetAnimeSpeed);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_SetAnimeEnd);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_SetAnimeBankEnd);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_isAnime);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_isAnimeBank);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_isSeniAnime);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_ModelDispSw);

INCLUDE_ASM(const s32, "menu/menu_mdl", _myVu0Length);

INCLUDE_ASM(const s32, "menu/menu_mdl", MnMoveMode_InitRoot);

INCLUDE_ASM(const s32, "menu/menu_mdl", _MnParMovRoot_GetPos);

INCLUDE_ASM(const s32, "menu/menu_mdl", MnMoveModelPosition);
