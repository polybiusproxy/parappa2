#include "main/stdat.h"

FILE_STR file_str_logo_file; /*= {
    FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATA\\LOGO.INT;1", {}
};*/

FILE_STR file_str_menu_file; /*= {
    FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATA\\STMENU.INT;1", {}
};*/

FILE_STR file_str_extra_file[10]; /*= {
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT00.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT01.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT02.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT03.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT04.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT05.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT06.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT07.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT08.WP2;1", {} },
    { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\EXT09.WP2;1", {} },
};*/

#if 0
static STDAT_DAT stdat_dat_st00[] = 
{
    {
        PSTEP_XTR, "SER XTR 0", 93.0, 0, 0x1CA9138, 0x1CA9000, 0x1CA9130,
        { FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATAS\\ST00SR0.INT;1", {} },
        {
            { FRMODE_CD, FTMODE_XTR, 2, 0, "\\XTR\\ST00SR0.XTR;1", {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
        },
        NULL
    }
};
#endif

#if 0
static STDAT_DAT stdat_dat_title[] = 
{
    {
        PSTEP_SERIAL, "TITLE", 100.0, 0, 0x1CA45B0, 0x1CA4598, 0x1CA45A8,
        { FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATA\\TITLE.INT;1", {} },
        {
            { FRMODE_CD, 0, 0, 0, NULL, {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
        },
        NULL
    },
    {
        PSTEP_SERIAL, "TITLE DERA", 100.0, 0, 0x1CA8FB0, 0x1CA8F98, 0x1CA8FA8,
        { FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATA\\TITLE.INT;1", {} },
        {
            { FRMODE_CD, FTMODE_WP2, 2, 0, "\\EXT\\TITLE.WP2;1", {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
        },
        NULL
    }
};
#endif

#if 0
static STDAT_DAT stdat_dat_st01[] = 
{
    {
        PSTEP_XTR, "SER XTR 0", 93.0, 1, 0x1CA0140, 0x1CA0048, 0x1CA0138,
        { FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATAS\\ST01SR0.INT;1", {} },
        {
            { FRMODE_CD, FTMODE_XTR, 2, 0, "\\XTR\\ST01SR0.XTR;1", {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
        },
        NULL
    },
    {
        PSTEP_HOOK, "HOOK 0", 68.0, 1, 0x1CB2348, 0x1CB22E0, 0x1CB22F0,
        { FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATA\\ST01HK0.INT;1", {} },
        {
            { FRMODE_CD, 0, 0, 0, NULL, {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
        },
        NULL
    },
    {
        PSTEP_XTR, "SER XTR 1", 93.0, 1, 0x1CA0358, 0x1CA0190, 0x1CA0350,
        { FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATAS\\ST01SR1.INT;1", {} },
        {
            { FRMODE_CD, FTMODE_XTR, 2, 0, "\\XTR\\ST01SR1.XTR;1", {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
            { FRMODE_CD, 0, 0, 0, NULL, {} },
        },
        NULL
    },
    {
        PSTEP_XTR, "GAME 0", 108.0, 1, 0x1D2BAF0, 0x1D2A9C8, 0x1D2AEA8,
        { FRMODE_CD, FTMODE_INTG, 0, 0, "\\DATA\\ST01GM0.INT;1", {} },
        {
            { FRMODE_CD, FTMODE_WP2, 2, 0, "\\SND\\ST01GM0C.WP2;1", {} },
            { FRMODE_CD, FTMODE_WP2, 2, 0, "\\SND\\ST01GM0G.WP2;1", {} },
            { FRMODE_CD, FTMODE_WP2, 4, 0, "\\SND\\ST01GM0N.WP2;1", {} },
        },
        0x1D2AEF0
    }
};
#endif

STDAT_REC stdat_rec[19];

/* sdata */
int stdat_rec_num; /* = 14; */

static void stDatFileSearch(FILE_STR *fs_pp)
{
    if ((fs_pp != NULL) && (fs_pp->fname != NULL))
    {
        while (CdctrlSerch(fs_pp) == 0)
            MtcWait(1);
    }
}

void stDatFirstFileSearch(void)
{
    int i, j, k;

    stDatFileSearch(&file_str_logo_file);
    stDatFileSearch(&file_str_menu_file);

    for (i = 0; i < 10U; i++)
    {
        stDatFileSearch(&file_str_extra_file[i]);
    }

    for (i = 0; i < stdat_rec_num; i++)
    {
        stDatFileSearch(&stdat_rec[i].ovlfile);

        for (j = 0; j < stdat_rec[i].stdat_dat_num; j++)
        {
            stDatFileSearch(&stdat_rec[i].stdat_dat_pp[j].intfile);

            for (k = 0; k < 3; k++)
            {
                stDatFileSearch(&stdat_rec[i].stdat_dat_pp[j].sndfile[k]);
            }
        }
    }
}

INCLUDE_RODATA(const s32, "main/stdat", D_00391DA8);

INCLUDE_RODATA(const s32, "main/stdat", D_00391DC0);

INCLUDE_RODATA(const s32, "main/stdat", D_00391DD8);

INCLUDE_RODATA(const s32, "main/stdat", D_00391DF0);

INCLUDE_RODATA(const s32, "main/stdat", D_00391E08);

INCLUDE_RODATA(const s32, "main/stdat", D_00391E20);

INCLUDE_RODATA(const s32, "main/stdat", D_00391E38);

INCLUDE_RODATA(const s32, "main/stdat", D_00391E50);

INCLUDE_RODATA(const s32, "main/stdat", D_00391E68);

INCLUDE_RODATA(const s32, "main/stdat", D_00391E80);

INCLUDE_RODATA(const s32, "main/stdat", D_00391E98);

INCLUDE_RODATA(const s32, "main/stdat", D_00391EB0);

INCLUDE_RODATA(const s32, "main/stdat", D_00391EC8);

INCLUDE_RODATA(const s32, "main/stdat", D_00391EE0);

INCLUDE_RODATA(const s32, "main/stdat", D_00391EF8);

INCLUDE_RODATA(const s32, "main/stdat", D_00391F08);

INCLUDE_RODATA(const s32, "main/stdat", D_00391F20);

INCLUDE_RODATA(const s32, "main/stdat", D_00391F30);

INCLUDE_RODATA(const s32, "main/stdat", D_00391F48);

INCLUDE_RODATA(const s32, "main/stdat", D_00391F60);

INCLUDE_RODATA(const s32, "main/stdat", D_00391F78);

INCLUDE_RODATA(const s32, "main/stdat", D_00391F90);

INCLUDE_RODATA(const s32, "main/stdat", D_00391FA8);

INCLUDE_RODATA(const s32, "main/stdat", D_00391FC0);

INCLUDE_RODATA(const s32, "main/stdat", D_00391FD8);

INCLUDE_RODATA(const s32, "main/stdat", D_00391FE8);

INCLUDE_RODATA(const s32, "main/stdat", D_00392000);

INCLUDE_RODATA(const s32, "main/stdat", D_00392018);

INCLUDE_RODATA(const s32, "main/stdat", D_00392030);

INCLUDE_RODATA(const s32, "main/stdat", D_00392048);

INCLUDE_RODATA(const s32, "main/stdat", D_00392060);

INCLUDE_RODATA(const s32, "main/stdat", D_00392078);

INCLUDE_RODATA(const s32, "main/stdat", D_00392090);

INCLUDE_RODATA(const s32, "main/stdat", D_003920A8);

INCLUDE_RODATA(const s32, "main/stdat", D_003920C0);

INCLUDE_RODATA(const s32, "main/stdat", D_003920D8);

INCLUDE_RODATA(const s32, "main/stdat", D_003920F0);

INCLUDE_RODATA(const s32, "main/stdat", D_00392108);

INCLUDE_RODATA(const s32, "main/stdat", D_00392120);

INCLUDE_RODATA(const s32, "main/stdat", D_00392138);

INCLUDE_RODATA(const s32, "main/stdat", D_00392150);

INCLUDE_RODATA(const s32, "main/stdat", D_00392168);

INCLUDE_RODATA(const s32, "main/stdat", D_00392180);

INCLUDE_RODATA(const s32, "main/stdat", D_00392198);

INCLUDE_RODATA(const s32, "main/stdat", D_003921B0);

INCLUDE_RODATA(const s32, "main/stdat", D_003921C8);

INCLUDE_RODATA(const s32, "main/stdat", D_003921E0);

INCLUDE_RODATA(const s32, "main/stdat", D_003921F8);

INCLUDE_RODATA(const s32, "main/stdat", D_00392210);

INCLUDE_RODATA(const s32, "main/stdat", D_00392228);

INCLUDE_RODATA(const s32, "main/stdat", D_00392240);

INCLUDE_RODATA(const s32, "main/stdat", D_00392258);

INCLUDE_RODATA(const s32, "main/stdat", D_00392270);

INCLUDE_RODATA(const s32, "main/stdat", D_00392288);

INCLUDE_RODATA(const s32, "main/stdat", D_003922A0);

INCLUDE_RODATA(const s32, "main/stdat", D_003922B8);

INCLUDE_RODATA(const s32, "main/stdat", D_003922D0);

INCLUDE_RODATA(const s32, "main/stdat", D_003922E8);

INCLUDE_RODATA(const s32, "main/stdat", D_00392300);

INCLUDE_RODATA(const s32, "main/stdat", D_00392318);

INCLUDE_RODATA(const s32, "main/stdat", D_00392330);

INCLUDE_RODATA(const s32, "main/stdat", D_00392348);

INCLUDE_RODATA(const s32, "main/stdat", D_00392360);

INCLUDE_RODATA(const s32, "main/stdat", D_00392378);

INCLUDE_RODATA(const s32, "main/stdat", D_00392390);

INCLUDE_RODATA(const s32, "main/stdat", D_003923A8);

INCLUDE_RODATA(const s32, "main/stdat", D_003923C0);

INCLUDE_RODATA(const s32, "main/stdat", D_003923D8);

INCLUDE_RODATA(const s32, "main/stdat", D_003923F0);

INCLUDE_RODATA(const s32, "main/stdat", D_00392408);

INCLUDE_RODATA(const s32, "main/stdat", D_00392420);

INCLUDE_RODATA(const s32, "main/stdat", D_00392438);

INCLUDE_RODATA(const s32, "main/stdat", D_00392450);

INCLUDE_RODATA(const s32, "main/stdat", D_00392468);

INCLUDE_RODATA(const s32, "main/stdat", D_00392480);

INCLUDE_RODATA(const s32, "main/stdat", D_00392498);

INCLUDE_RODATA(const s32, "main/stdat", D_003924B0);

INCLUDE_RODATA(const s32, "main/stdat", D_003924C8);

INCLUDE_RODATA(const s32, "main/stdat", D_003924E0);

INCLUDE_RODATA(const s32, "main/stdat", D_003924F8);

INCLUDE_RODATA(const s32, "main/stdat", D_00392510);

INCLUDE_RODATA(const s32, "main/stdat", D_00392528);

INCLUDE_RODATA(const s32, "main/stdat", D_00392540);

INCLUDE_RODATA(const s32, "main/stdat", D_00392558);

INCLUDE_RODATA(const s32, "main/stdat", D_00392570);

INCLUDE_RODATA(const s32, "main/stdat", D_00392588);

INCLUDE_RODATA(const s32, "main/stdat", D_003925A0);

INCLUDE_RODATA(const s32, "main/stdat", D_003925B8);

INCLUDE_RODATA(const s32, "main/stdat", D_003925D0);

INCLUDE_RODATA(const s32, "main/stdat", D_003925E8);

INCLUDE_RODATA(const s32, "main/stdat", D_00392600);

INCLUDE_RODATA(const s32, "main/stdat", D_00392618);

INCLUDE_RODATA(const s32, "main/stdat", D_00392630);

INCLUDE_RODATA(const s32, "main/stdat", D_00392648);

INCLUDE_RODATA(const s32, "main/stdat", D_00392660);

INCLUDE_RODATA(const s32, "main/stdat", D_00392678);

INCLUDE_RODATA(const s32, "main/stdat", D_00392690);

INCLUDE_RODATA(const s32, "main/stdat", D_003926A8);

INCLUDE_RODATA(const s32, "main/stdat", D_003926C0);

INCLUDE_RODATA(const s32, "main/stdat", D_003926D8);

INCLUDE_RODATA(const s32, "main/stdat", D_003926F0);

INCLUDE_RODATA(const s32, "main/stdat", D_00392708);

INCLUDE_RODATA(const s32, "main/stdat", D_00392720);

INCLUDE_RODATA(const s32, "main/stdat", D_00392738);

INCLUDE_RODATA(const s32, "main/stdat", D_00392750);

INCLUDE_RODATA(const s32, "main/stdat", D_00392768);

INCLUDE_RODATA(const s32, "main/stdat", D_00392780);

INCLUDE_RODATA(const s32, "main/stdat", D_00392798);

INCLUDE_RODATA(const s32, "main/stdat", D_003927B0);

INCLUDE_RODATA(const s32, "main/stdat", D_003927C8);

INCLUDE_RODATA(const s32, "main/stdat", D_003927E0);

INCLUDE_RODATA(const s32, "main/stdat", D_003927F8);

INCLUDE_RODATA(const s32, "main/stdat", D_00392810);

INCLUDE_RODATA(const s32, "main/stdat", D_00392828);

INCLUDE_RODATA(const s32, "main/stdat", D_00392840);

INCLUDE_RODATA(const s32, "main/stdat", D_00392858);

INCLUDE_RODATA(const s32, "main/stdat", D_00392870);

INCLUDE_RODATA(const s32, "main/stdat", D_00392888);

INCLUDE_RODATA(const s32, "main/stdat", D_003928A0);

INCLUDE_RODATA(const s32, "main/stdat", D_003928B8);

INCLUDE_RODATA(const s32, "main/stdat", D_003928D0);

INCLUDE_RODATA(const s32, "main/stdat", D_003928E8);

INCLUDE_RODATA(const s32, "main/stdat", D_00392900);

INCLUDE_RODATA(const s32, "main/stdat", D_00392918);

INCLUDE_RODATA(const s32, "main/stdat", D_00392930);

INCLUDE_RODATA(const s32, "main/stdat", D_00392948);

INCLUDE_RODATA(const s32, "main/stdat", D_00392958);

INCLUDE_RODATA(const s32, "main/stdat", D_00392970);

INCLUDE_RODATA(const s32, "main/stdat", D_00392988);

INCLUDE_RODATA(const s32, "main/stdat", D_003929A0);

INCLUDE_RODATA(const s32, "main/stdat", D_003929B8);

INCLUDE_RODATA(const s32, "main/stdat", D_003929D0);

INCLUDE_RODATA(const s32, "main/stdat", D_003929E8);

INCLUDE_RODATA(const s32, "main/stdat", D_00392A00);

INCLUDE_RODATA(const s32, "main/stdat", D_00392A18);

INCLUDE_RODATA(const s32, "main/stdat", D_00392A30);

INCLUDE_RODATA(const s32, "main/stdat", D_00392A48);
