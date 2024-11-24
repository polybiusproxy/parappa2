#include "menu/memc.h"

#include <libmc.h>
#include <string.h>

/* data - static */
extern sceMcIconSys memc_iconsys;
extern u_int _memc_type[];

/* bss - static */
extern MEMC_STAT memc_stat;

static int memc_mansub_Open(char *name, u_int type);

void memc_init(void)
{
    sceMcInit();

    memset(&memc_stat, 0, sizeof(memc_stat));
    memset(memc_iconsys.Reserve3, 0, sizeof(memc_iconsys.Reserve3));
}

void memc_setDirName(char *name)
{
    memcpy(memc_stat.saveDir, name, 63);
    memc_stat.saveDir[63] = '\0';
}

void memc_setSaveTitle(char *name, int nLFPos)
{
    int length = strlen(name);

    if (length > 64)
        length = 64;

    memcpy(memc_iconsys.TitleName, name, 64);

    memc_iconsys.TitleName[length + 1] = '\0';
    memc_iconsys.OffsLF = nLFPos;
}

void memc_setIconSysHed(void *pIhData, int IhSize)
{
    int nLF;

    if (!pIhData || IhSize <= 0)
        return;

    nLF = memc_iconsys.OffsLF;
    
    memcpy(&memc_iconsys, pIhData, IhSize);
    memc_iconsys.OffsLF = nLF;
}

void memc_setSaveIcon(int no, void *pIconData, int nIconSize)
{
    MEMC_STAT *pmw = &memc_stat;

    switch (no)
    {
    case 0:
        pmw->iconData1 = pIconData;
        pmw->iconSize1 = nIconSize;
        
        strcpy(memc_iconsys.FnameView, "icon1.ico");
        break;
    case 1:
        if (pIconData == NULL || nIconSize == 0)
        {
            pmw->iconData2 = 0;
            pmw->iconSize2 = 0;

            strcpy(memc_iconsys.FnameCopy, "icon1.ico");
        }
        else
        {
            pmw->iconData2 = pIconData;
            pmw->iconSize2 = nIconSize;

            strcpy(memc_iconsys.FnameCopy, "icon2.ico");
        }
        break;
    case 2:
        if (pIconData == NULL || nIconSize == 0)
        {
            pmw->iconData3 = 0;
            pmw->iconSize3 = 0;

            strcpy(memc_iconsys.FnameDel, "icon1.ico");
        }
        else
        {
            pmw->iconData3 = pIconData;
            pmw->iconSize3 = nIconSize;

            strcpy(memc_iconsys.FnameDel, "icon3.ico");
        }
        break;
    default:
        break;
    }
}

char* memc_getfilename(int no)
{
    char *fbody;
    extern char tmps0[64];

    switch (no)
    {
    case -1:
        fbody = "icon.sys";
        break;
    case -2:
        fbody = "icon1.ico";
        break;
    case -3:
        if (memc_stat.iconData2 == NULL)
            return NULL;
    case -4:
        if (memc_stat.iconData3 == NULL)
            return NULL;
    default:
        if (no >= 2)
            return NULL;

        if (no == 0)
        {
            fbody = memc_stat.saveDir;
        }
        else
        {
            sprintf(tmps0, "SAVE%03d", no);
            fbody = tmps0;
        }
        break;
    }

    return fbody;
}

extern char D_00399878[]; // sdata - "/"

char* memc_getfilepath(int no)
{
    extern char tmps1[130];
    char *fbody;

    fbody = memc_getfilename(no);
    if (fbody == NULL)
    {
        return NULL;
    }
    else
    {
        strcpy(tmps1, memc_stat.saveDir);
        strcat(tmps1, D_00399878);
        strcat(tmps1, fbody);
        fbody = tmps1;
    }

    return fbody;
}

int memc_checkFormat(void)
{
    return memc_stat.format;
}

int memc_getChangeState(void)
{
    return memc_stat.isChange;
}

void memc_setChangeState(int flg)
{
    memc_stat.isChange = flg;
}

static int memc_SaveFileClust(void)
{
    MEMC_STAT *pmw = &memc_stat;
    int        size;

    if (pmw->seek != 0)
    {
        size = pmw->seek + pmw->size2;
        return (size + 1023) / 1024;
    }
    else
    {
        size = pmw->size;
        return (size + 1023) / 1024;
    }
}

static void memc_clearMEMCINFO(MEMC_INFO *info)
{
    info->flag = 0;
    info->free = 0;
    info->allfile = 0;
    info->savefile = 0;

    if (info->dirfile != NULL)
    {
        if (info->dirfileMax < 1)
            return;

        memset(info->dirfile, 0, info->dirfileMax * 64);
    }

    info->savefile = 0;
}

INCLUDE_ASM("menu/memc", memc_searchDirTbl);

extern char D_00399880[]; // sdata - "/*"

int memc_port_info(int port, MEMC_INFO *info)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;
 
    pmw->port = port;
    pmw->buf = (char*)info;

    pmw->cmd = 14;
    pmw->retry = 0;

    strcpy(pmw->filename, pmw->saveDir);
    strcat(pmw->filename, D_00399880);

    pmw->bChkSys = 1;
    pmw->filename[63] = '\0';

    memc_clearMEMCINFO(info);

    re = sceMcGetInfo(pmw->port, pmw->slot, &pmw->type, &pmw->free, &pmw->format);
    if (re == sceMcResSucceed)
    {
        pmw->func = 11;
    }
    
    return re;
}

int memc_del_file(int port, int no)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;
    char      *tmpp;

    pmw->cmd = 15;
    pmw->retry = 0;

    pmw->port = port;
    pmw->fileNo = no;

    tmpp = memc_getfilepath(no);
    if (tmpp != NULL)
        strcpy(pmw->filename, tmpp);
    else
        pmw->filename[0] = '\0';

    re = sceMcDelete(pmw->port, pmw->slot, pmw->filename);
    if (re == sceMcResSucceed)
    {
        pmw->func = 12;
    }

    return re;
}

int memc_load_file(int port, int no, char *buf, int size)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;
    char      *tmpp;

    pmw->port        = port;
    pmw->buf         = buf;
    pmw->size        = size;

    pmw->retry       = 0;
    pmw->isSyncClose = 0;
    pmw->fileNo      = no;

    tmpp = memc_getfilepath(no);
    if (tmpp != NULL)
        strcpy(pmw->filename, tmpp);
    else
        pmw->filename[0] = '\0';

    re = sceMcGetInfo(pmw->port, pmw->slot, &pmw->type, &pmw->free, &pmw->format);
    if (re == sceMcResSucceed)
    {
        pmw->cmd = 14;
        pmw->func = 3;
    }

    pmw->retry = 0;
    return re;
}

int memc_loadFirst(int port, int no, char *buf, int size)
{
    MEMC_STAT *pmw = &memc_stat;
    char      *tmpp;

    pmw->port        = port;
    pmw->buf         = buf;
    pmw->size        = size;

    pmw->retry       = 0;
    pmw->isSyncClose = 1;
    pmw->fileNo      = no;

    tmpp = memc_getfilepath(no);
    if (tmpp != NULL)
        strcpy(pmw->filename, tmpp);
    else 
        pmw->filename[0] = '\0';

    if (memc_mansub_Open(pmw->filename, SCE_RDONLY) == 0)
    {
        pmw->func = 3;
        return 0;
    }

    return 1;
}

int memc_save_file(int port, int no, char* buf, int size, int bSysRW)
{
    int        re;
    int        n;
    int        isize;

    MEMC_STAT* pmw;
    char*      tmpp;

    pmw = &memc_stat;

    pmw->port    = port;
    pmw->buf     = buf;
    pmw->size    = size;
    pmw->retry   = 0;
    pmw->stat    &= ~0x1f; /* Clear all flags */

    pmw->seek    = 0;
    pmw->size2   = 0;
    pmw->bChkSys = bSysRW;
    pmw->fileNo  = no;

    tmpp = memc_getfilepath(no);
    if (tmpp != NULL)
        strcpy(pmw->filename, tmpp);
    else
        pmw->filename[0] = '\0';

    n = 3;
    isize = (pmw->iconSize1 + 1023) / 1024;

    if (memc_getfilename(-3) != NULL)
    {
        isize += (pmw->iconSize2 + 1023) / 1024;
        n++;
    }

    if (memc_getfilename(-4) != NULL)
    {
        isize += (pmw->iconSize3 + 1023) / 1024;
        n++;
    }

    pmw->sysFileSize = isize + ((n + 1) / 2) + 3;

    re = sceMcGetInfo(pmw->port, pmw->slot, &pmw->type, &pmw->free, &pmw->format);
    if (re == sceMcResSucceed)
    {
        pmw->cmd  = 14;
        pmw->func = 4;
    }

    return re;
}

int memc_seeksave_file(int port, int no, char *buf, int size, int seek, int sizef, int bSysRW)
{
    MEMC_STAT *pmw = &memc_stat;
    int        re;

    re = memc_save_file(port, no, buf, size, bSysRW);

    pmw->seek = seek;
    pmw->size2 = sizef;
    return re;
}

int memc_save_overwrite(void)
{
    int        re;
    int        n;
    int        isize;

    MEMC_STAT *pmw = &memc_stat;

    n = 2;
    isize = (pmw->iconSize1 + 1023) / 1024;

    if (memc_getfilename(-3) != NULL)
    {
        isize += (pmw->iconSize2 + 1023) / 1024;
        n++;
    }

    if (memc_getfilename(-4) != NULL)
    {
        isize += (pmw->iconSize3 + 1023) / 1024;
        n++;
    }

    pmw->sysFileSize = isize + ((n + 1) / 2) + 3;

    re = sceMcGetInfo(pmw->port, pmw->slot, &pmw->type, &pmw->free, &pmw->format);
    if (re == sceMcResSucceed)
    {
        pmw->cmd  = 14;
        pmw->func = 14;
    }

    return re;
}

int memc_port_check(int port, int *type, int *free)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;
  
    re = sceMcGetInfo(port, 0, type, free, &pmw->format);
    if (re == sceMcResSucceed)
    {
        pmw->cmd = 14;
        pmw->func = 1;
    }

    pmw->retry = 0;
    return re;
}

int memc_format(int port)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;

    pmw->port = port;

    re = sceMcGetInfo(port, pmw->slot, &pmw->type, &pmw->free, &pmw->format);
    if (re == sceMcResSucceed)
    {
        pmw->cmd = 14;
        pmw->func = 7;
    }

    pmw->retry = 0;
    return re;
}

int memc_chg_dir(int port, char *name)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;

    pmw->retry = 0;
    strcpy(pmw->filename, name);

    re = sceMcChdir(port, 0, name, NULL);
    if (re == sceMcResSucceed)
    {
        pmw->func = 10;
        pmw->cmd  = 12;
    }

    return re;
}

int memc_get_dir(int port, char *name, sceMcTblGetDir *dir, int max)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;

    memset(dir, 0, max * 64);

    pmw->port = port;

    strcpy(pmw->filename, name);

    pmw->size = max;
    pmw->buf = (char*)dir;

    re = sceMcGetDir(pmw->port, pmw->slot, pmw->filename, 0, max, dir);
    if (re == sceMcResSucceed)
    {
        pmw->func = 9;
        pmw->cmd = 13;
        pmw->retry = 0;
    }

    return re;
}

int memc_get_dir_continue(sceMcTblGetDir *dir, int max)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;

    memset(dir, 0, max * 64);

    pmw->size = max;
    pmw->buf = (char*)dir;

    re = sceMcGetDir(pmw->port, pmw->slot, pmw->filename, 1, max, dir);
    if (re == sceMcResSucceed)
    {
        pmw->func = 9;
        pmw->cmd = 13;
        pmw->retry = 0;
    }

    return re;
}

INCLUDE_ASM("menu/memc", memc_mansub_ErrChk);

static int memc_mansub_Open(char *name, u_int type)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;

    re = sceMcOpen(pmw->port, pmw->slot, name, type);
    if (re == sceMcResSucceed)
    {
        pmw->cmd = 2;
    }

    return re;
}

static int memc_mansub_Close(void)
{
    int        re;
    MEMC_STAT *pmw = &memc_stat;

    re = sceMcClose(pmw->fd);
    if (re == sceMcResSucceed)
    {
        pmw->cmd = 3;
    }

    return re;
}

INCLUDE_ASM("menu/memc", memcsub_fileChk);
/* static */ int memcsub_fileChk(/* a0 4 */ sceMcTblGetDir *dir, /* a1 5 */ unsigned char *name, /* a2 6 */ int max);

extern char D_00399888[]; /* sdata - "SAVE" */

static int memc_mansub_GetInfo(int result)
{
    MEMC_INFO *info;
    MEMC_STAT *pmw = &memc_stat;

    int re;

    switch (pmw->cmd)
    {
    case 14:
        if (pmw->type != sceMcTypePS2)
        {
            pmw->func = 0;
            return 2;
        }
        else
        {
            if (result < 0)
            {
                pmw->func = 0;

                if (result != -2)
                    re = result;
                else
                    re = -2000;

                return memc_mansub_ErrChk(re);
            }
            else
            {
                if (pmw->format == 0)
                {
                    pmw->func = 0;
                    return 3;
                }
                else
                {
                    info = (MEMC_INFO*)pmw->buf;

                    info->free = pmw->free;
                    info->flag |= _memc_type[pmw->type];

                    if (sceMcGetDir(pmw->port, pmw->slot, pmw->filename, 0, info->dirfileMax, info->dirfile) == sceMcResSucceed)
                    {
                        pmw->cmd = sceMcFuncNoGetDir;
                        return 16;
                    }

                    return 1;
                }
            }
        }
        break;
    case 13:
        if (result < 0)
        {
            pmw->func = 0;
            return memc_mansub_ErrChk(result);
        }
        else
        {
            info = (MEMC_INFO*)pmw->buf;

            info->allfile = result;
            info->flag |= 1;

            if (pmw->bChkSys)
            {
                if (memcsub_fileChk(info->dirfile, memc_getfilename(-1), result))
                    info->flag |= 2;
                if (memcsub_fileChk(info->dirfile, memc_getfilename(-2), result))
                    info->flag |= 4;
                if (memcsub_fileChk(info->dirfile, memc_getfilename(-3), result))
                    info->flag |= 8;
                if (memcsub_fileChk(info->dirfile, memc_getfilename(-4), result))
                    info->flag |= 16;
                
                info->savefile  = memcsub_fileChk(info->dirfile, pmw->saveDir, result);
                info->savefile += memcsub_fileChk(info->dirfile, D_00399888, result);
            }

            pmw->func = 0;
            return 0;
        }
        break;
    default:
        return 0;
    }
}

static int memc_mansub_load(int result)
{
    MEMC_STAT *pmw = &memc_stat;

    switch (pmw->cmd)
    {
    case 14:
        if (result < 0)
            return memc_mansub_ErrChk(result);

        if (pmw->type != 2)
        {
            pmw->func = 0;
            return 2;
        }

        if (!memc_mansub_Open(pmw->filename, 1))
        {
            pmw->func = 3;
            break;
        }

        return 1;
    case 2:
        if (result < 0)
            return memc_mansub_ErrChk(result);

        pmw->fd = result;
        if (!sceMcRead(pmw->fd, pmw->buf, pmw->size))
        {
            pmw->cmd = 5;
            return 16;
        }

        return 16;
    case 5:
        if (result < 0)
            return memc_mansub_ErrChk(result);

        if (memc_mansub_Close())
            return 1;

        pmw->func = 6;
        if (!pmw->isSyncClose)
            return 16;

        pmw->isSyncClose = 0;
        if (sceMcSync(0, NULL, &result) != 1)
            return 16;

        return memc_mansub_ErrChk(result);
    }

    return 16;
}

INCLUDE_ASM("menu/memc", memc_manager_save);

INCLUDE_ASM("menu/memc", memc_manager_overwrite);

#if 1
INCLUDE_ASM("menu/memc", memc_manager_chk);
/* static */ int memc_manager_chk(int mode);
#else
static int memc_manager_chk(int mode)
{
    /* s1 17 */ int re;
    /* -0x40(sp) */ int result;
    /* s0 16 */ MEMC_STAT *pmw;

    pmw = &memc_stat;

    re = sceMcSync(mode, 0, &result);

    if (re == sceMcExecFinish)
    {
        switch (memc_stat.func)
        {
        case 1:
            if (result == -2)
            {
                result = -2000;
            }

            return memc_mansub_ErrChk(result);
        case 2:
        case 6:
        case 8:
        case 9:
        case 10:
        case 12:
            return memc_mansub_ErrChk(result);
        case 3:
            return memc_mansub_load(result);
        case 4:
            return memc_manager_save(result);
        case 7:
            break;
        default:
            pmw->func = 0;
            break;
        }
    }

    return re;
}
#endif

int memc_manager(int mode)
{
    int re;

    if (mode == 1)
    {
        re = memc_manager_chk(1);
    }
    else
    {
        do
            re = memc_manager_chk(mode);
        while (re == 16);
    }

    return re;
}
