#include "menu/p3mc.h"

#include "menu/memc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* sdata - static */
char *HedderID;
char *FooterID;
GETUSER_WORK *pUChkWork;

/* bss - static */
extern P3MC_WORK P3MC_Work;

/* sbss - static */
int FreeSizeFlg;
int portCheckFlg;
int NeedSize[2];
int UChkSize[2];

/* bss - static */
char filePath[64];
MEMC_INFO *mcmenu_info;
sceMcTblGetDir p3mcTblGetDir[8];

/* sbss - static */
int isFileFlgCash;

/* bss - static */
u_char McLogFileFlg[80];
u_char McReplayFileFlg[80];

static char* _P3MC_GetFilePath(int mode, int fileNo);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_GetIconSize);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396180);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_GetIconPtr);

static void _P3MC_SetUserDirName(/* a0 4 */ int mode, /* a1 5 */ int fileNo)
{
    memc_setDirName(_P3MC_GetFilePath(mode, fileNo));
}

#if 1
INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_GetFilePath);
#else
static char* _P3MC_GetFilePath(/* a0 4 */ int mode, /* a2 6 */ int fileNo)
{
    char *addName = &filePath[12];

    strcpy(filePath, "BISCPS-15017");

    switch (mode)
    {
    case 1:
        if (fileNo >= 0)
            sprintf(addName, "LOG%03d", fileNo);
        else
            strcpy(addName, "LOG???");
        break;
    case 2:
        if (fileNo >= 0)
            sprintf(addName, "REP%03d", fileNo);
        else
            strcpy(addName, "REP???");
        break;
    default:
        strcpy(addName, "??????");
        break;
    }

    return filePath;
}
#endif

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_EUC2SJIS);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_ASC2SJIS);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_UserName_ASC2SJIS);

INCLUDE_RODATA(const s32, "menu/p3mc", D_003961C0);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_SetBrowsInfo);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_mainfile_chk);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_file_chk);

#if 1
INCLUDE_ASM(const s32, "menu/p3mc", P3MC_InitReady);
#else
int P3MC_InitReady(void)
{
    /* v1 3 */ int re;

    portCheckFlg = 0;
    FreeSizeFlg = 0;

    memset(&P3MC_Work, 0, sizeof(P3MC_WORK));
    memset(&mcmenu_info, 0, 0x18);
    memset(p3mcTblGetDir, 0, 0x200);

    isFileFlgCash = 0;

    memset(McLogFileFlg, 0, 0x50);
    memset(McReplayFileFlg, 0, 0x50);
    memc_init();
    //mcmenu_info.dirfile = p3mcTblGetDir;
    //mcmenu_info.dirfileMax = 8;
    memc_port_info(0, &mcmenu_info);

    return re;
}
#endif

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_GetSaveSize);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_SetCheckSaveSize);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_freesize_chk);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_CheckChange);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_CheckChangeClear);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_CheckChangeSet);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_CheckIsNewSave);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_GetSaveDataSize);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_DeleteDataWork);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_MakeDataWork);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MCStrCmpLen);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MCStrNum);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_MemcCheck);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_GetUserStart);

void P3MC_GetUserEnd(void)
{
    if (pUChkWork != NULL)
        free(pUChkWork);

    pUChkWork = NULL;
}

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_GetUserCheck);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_AddUser);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_AddUserBroken);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_SortUser);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_CheckBrokenUser);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_OpeningCheckStart);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_OpeningCheckEnd);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396420);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396448);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396470);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396498);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_OpeningCheck);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_LoadUser);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_LoadCheck);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_loadCheck);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_SetUserWorkTime);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_SaveUser);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_SaveCheck);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_SaveCheck);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_proc);

void _P3MC_dataCheckFunc(P3MC_WORK *pw,P3MCDataCheckFunc funcp)
{
    pw->data_cfunc = funcp;
}

// INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_CheckUserData);
static int _P3MC_CheckUserData(/* a0 4 */ P3MC_WORK *pw)
{
    /* s0 16 */ USER_FOOTER *pfoot = pw->dhdl->pFoot;

    if (_P3MC_CheckUserDataHead(pw))
        return 1;
    else 
        return (strcmp(FooterID, pfoot->footer) != 0);
    
}

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_CheckUserDataHead);
