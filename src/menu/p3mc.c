#include "menu/p3mc.h"

#include "menu/memc.h"

#include <../libcdvd.h>

#include <stdio.h>
#include <malloc.h>
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
extern char filePath[64];
extern MEMC_INFO *mcmenu_info;
extern sceMcTblGetDir p3mcTblGetDir[8];

/* sbss - static */
int isFileFlgCash;

/* bss - static */
extern u_char McLogFileFlg[80];
extern u_char McReplayFileFlg[80];

/* static */ char* _P3MC_GetFilePath(int mode, int fileNo);
static void  _P3MC_dataCheckFunc(P3MC_WORK *pw, P3MCDataCheckFunc funcp);
static int   _P3MC_CheckUserData(P3MC_WORK *pw);
/* static */ int   _P3MC_CheckUserDataHead(P3MC_WORK *pw);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_GetIconSize);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396180);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_GetIconPtr);

static void _P3MC_SetUserDirName(int mode, int fileNo)
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

void P3MC_CheckChangeClear(void)
{
    memc_setChangeState(0);
}

void P3MC_CheckChangeSet(void)
{
    memc_setChangeState(1);
}

int P3MC_CheckIsNewSave(int mode)
{
    return FreeSizeFlg & mode;
}

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_GetSaveDataSize);

void P3MC_DeleteDataWork(MCRWDATA_HDL *phdl)
{
    if (phdl == NULL)
        return;
    
    if (phdl->pMemTop != NULL)
        free(phdl->pMemTop);
    
    free(phdl);
}

#define USER_HDR(x) ((USER_HEADER*)x)

#if 1
INCLUDE_ASM(const s32, "menu/p3mc", P3MC_MakeDataWork);
#else
MCRWDATA_HDL* P3MC_MakeDataWork(/* s4 20 */ int dsize, /* s5 21 */ USER_DATA *puser)
{
    /* s3 19 */ MCRWDATA_HDL *phdl;
    /* s2 18 */ u_char *pdata;
    /* s1 17 */ int asize;
    /* s0 16 */ int dsize0;

    phdl = malloc(sizeof(MCRWDATA_HDL));

    if (phdl != NULL)
    {
        dsize0 = ((dsize + 15) >> 4) * 16;

        pdata = memalign(16, dsize0 + 0x2694);
        memset(pdata, 0, dsize0 + 0x2694);

        phdl->pMemTop  = pdata; // 1045
        phdl->rwsize   = dsize0 + sizeof(USER_HEADER); // 1046
        phdl->datasize = dsize0 + sizeof(USER_HEADER); // 1047
        phdl->srcsize  = dsize; // 1048

        phdl->pHead = (USER_HEADER*)pdata; // 1050
        phdl->pData = pdata + sizeof(USER_HEADER); // 1051
        phdl->pFoot = USER_HDR(pdata + sizeof(USER_HEADER))->footer[dsize0]; // 1052

        if (puser != NULL)
        {
            USER_HDR(pdata)->user = *puser;
        }

        return phdl;
    }
    else
    {
        P3MC_DeleteDataWork(NULL);
        return NULL;
    }
}
#endif

INCLUDE_ASM(const s32, "menu/p3mc", _P3MCStrCmpLen);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MCStrNum);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_MemcCheck);

int P3MC_GetUserStart(int mode, P3MC_USRLST *pUsrLst, int bFirst)
{
    GETUSER_WORK *pWork;

    if (pUChkWork != NULL)
        free(pUChkWork);
    
    pWork = memalign(16, sizeof(GETUSER_WORK));
    pUChkWork = pWork;

    portCheckFlg = 0;

    if (pWork != NULL)
    {
        pWork->bFirst = bFirst;
        pWork->curState = 0;

        if (mode & 1)
        {
            pWork->curMode = 1;
        }
        else if (mode & 2)
        {
            pWork->curMode = 2;
        }  
        else
        {
            printf("P3MC_GetUser Error Mode is unknown!\n");
            pWork->curMode = 1;
        }
            

        pWork->curFno = 0;
        pWork->curUserMode = mode;
        pWork->pUserLst = pUsrLst;
    }

    return -1;
}

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

int P3MC_CheckBrokenUser(P3MC_USRLST *pUser, int mode)
{
    int         nmuser;
    USER_DATA **pmuser;

    int i;
    int nBrk = 0;

    if (mode & 1)
    {
        pmuser = pUser->plog_user;
        nmuser = pUser->nLogGet;

        for (i = 0; i < nmuser; i++, pmuser++)
        {
            if ((*pmuser)->flg == 2)
                nBrk++;
        }
    }

    if (mode & 2)
    {
        pmuser = pUser->prep_user;
        nmuser = pUser->nRepGet;

        for (i = 0; i < nmuser; i++, pmuser++)
        {
            if ((*pmuser)->flg == 2)
                nBrk++;
        }
    }

    return nBrk;
}

void P3MC_OpeningCheckStart(void)
{
    GETUSER_WORK *pWork;

    if (pUChkWork != NULL)
        free(pUChkWork);

    pWork = memalign(16, sizeof(GETUSER_WORK));
    pUChkWork = pWork;
    
    P3MC_CheckChangeSet();
    portCheckFlg = 0;

    if (pWork != NULL)
        pWork->curState = 0;    
}

void P3MC_OpeningCheckEnd(void)
{
    if (pUChkWork != NULL)
        free(pUChkWork);

    pUChkWork = NULL;
}

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396420);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396448);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396470);

INCLUDE_RODATA(const s32, "menu/p3mc", D_00396498);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_OpeningCheck);

#if 1
INCLUDE_ASM(const s32, "menu/p3mc", P3MC_LoadUser);
#else
int P3MC_LoadUser(/* s5 21 */ int mode, /* s3 19 */ int fileNo, /* s1 17 */ MCRWDATA_HDL *pdhdl, /* s4 20 */ int flg)
{
    P3MC_WORK *pw = &P3MC_Work;

    memset(pdhdl->pMemTop, 0, pdhdl->rwsize);

    _P3MC_SetUserDirName(mode, fileNo);
    _P3MC_dataCheckFunc(pw, _P3MC_CheckUserData);

    pw->prgflag    = flg; // 2114

    pw->dstat      = 0; // 2115
    pw->dhdl       = pdhdl; // 2121
    
    pw->data_mode  = mode; // 2117
    pw->data_no    = fileNo; // 2118
    pw->data_stage = 0; // 2119

    pw->prg = 0x1000; // 2122
    return 0;
}
#endif

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_LoadCheck);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_loadCheck);

void P3MC_SetUserWorkTime(USER_DATA *puser)
{
    int        err;
    sceCdCLOCK clock;

    err = sceCdReadClock(&clock);
    puser->date_pad = rand() % 200;

    if (err != 0 && clock.stat == 0)
    {
        puser->date_second = clock.second;
        puser->date_minute = clock.minute;
        puser->date_hour   = clock.hour;

        puser->date_day    = clock.day;
        puser->date_month  = clock.month;
        puser->date_year   = clock.year + 0x2000;
    }
    else
    {
        puser->date_second = 0;
        puser->date_minute = 0;
        puser->date_hour   = 12;

        puser->date_day    = 1;
        puser->date_month  = 1;
        puser->date_year   = 0x2000;
    }
}

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_SaveUser);

INCLUDE_ASM(const s32, "menu/p3mc", P3MC_SaveCheck);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_SaveCheck);

INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_proc);

void _P3MC_dataCheckFunc(P3MC_WORK *pw, P3MCDataCheckFunc funcp)
{
    pw->data_cfunc = funcp;
}

static int _P3MC_CheckUserData(P3MC_WORK *pw)
{
    USER_FOOTER *pfoot = pw->dhdl->pFoot;

    if (_P3MC_CheckUserDataHead(pw))
        return 1;
    else 
        return (strcmp(FooterID, pfoot->footer) != 0);
}

#if 1
INCLUDE_ASM(const s32, "menu/p3mc", _P3MC_CheckUserDataHead);
#else
static int _P3MC_CheckUserDataHead(/* s1 17 */ P3MC_WORK *pw)
{
	/* s0 16 */ USER_HEADER *hed = (USER_HEADER*)pw->dhdl->pMemTop;

    if (strcmp(hed->header, HedderID) == 0)
    {
        if (strcmp(hed->footer, FooterID) == 0)
        {
            if (!hed->user.flg)
                return 0;

            if (hed->user.fileNo == pw->data_no)
            {
                if (hed->user.mode == pw->data_mode)
                {
                    if (pw->data_stage > 0)
                    {
                        if (hed->user.stageNo != pw->data_stage)
                            return 1;
                    }

                    if (hed->user.name[0] != 0)
                        return (hed->user.name1[0] == 0);
                    else
                        return 0;
                }
            }
        }
    }
    
    return 1;
}
#endif
