#include "main/cdctrl.h"
#include <libcdvd.h>
#include <sifdev.h>

/* sdata */
void *current_intg_adrs;
/* static */ int sndFadeTime;

/* Define in CdctrlWp2GetSampleTmpBuf */
/* static */ int time_tmp_max;

/* bss - static */
extern unsigned char RBuff[4113];
extern CDCTRL_STR cdctrl_str;

/* sbss - static */
static int cdSampleTmp;

/* .lit4 */
float D_00398F00;
float D_00398F04;
float D_00398F08;
float D_00398F0C;

/* Concats two shorts into an integer; for use on the WP2_SETMASTERVOL command */
#define WP2_CONCAT(x, y)        ((x << 16) | (y))

// WP2 commands
#define WP2_INIT                0x000e /* Arg -> Mode           */
#define WP2_QUIT                0x0001 /*        No args        */
#define WP2_SDINIT              0x000d /* Arg -> Status         */
#define WP2_SETMASTERVOL        0x000a /* Arg -> Volume         */
#define WP2_BGMSETTRPOINT       0x0016 /* Arg -> Transfer pos   */
#define WP2_BGMINIT             0x8000 /* Arg -> Block size     */

u_int PackIntGetDecodeSize(u_char *fp_r)
{
    return *(u_int*)fp_r;
}

INCLUDE_ASM(const s32, "main/cdctrl", PackIntDecode);

INCLUDE_ASM(const s32, "main/cdctrl", PackIntDecodeWait);

void CdctrlInit(void)
{
    WorkClear(&cdctrl_str, sizeof(cdctrl_str));

    printf("in wp2 init\n");
    WP2Init();
    printf("out wp2 init\n");

    WP2Ctrl(WP2_INIT, SCECdDVD);
    WP2Ctrl(WP2_SDINIT, 0);
    WP2Ctrl(WP2_SETMASTERVOL, WP2_CONCAT(0x3fff, 0x3fff));
    WP2Ctrl(WP2_BGMINIT, 0x300);
}

void CdctrlQuit(void)
{
    WorkClear(&cdctrl_str, sizeof(cdctrl_str));
    WP2Ctrl(WP2_QUIT, 0);
}

void CdctrlMasterVolSet(u_int vol)
{
    WP2Ctrl(WP2_SETMASTERVOL, WP2_CONCAT(vol, vol));
}

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/cdctrl", CdctrlSerch);
#else
int CdctrlSerch(FILE_STR *fstr_pp)
{
    int rfd;
    int readsize;
    int ret;

    if (fstr_pp->search == 0)
    {
        if (fstr_pp->frmode == FRMODE_CD)
        {
            ret = sceCdSearchFile(&fstr_pp->fpCd, fstr_pp->fname);
            if (ret == 0)
            {
                printf("CD SEARCH ERROR!![%s]\n", fstr_pp->fname);
                return 0;
            }

            fstr_pp->search = 1;
        }
        else
        {
            rfd = sceOpen(fstr_pp->fname, 1);
            if (rfd < 0)
            {
                printf("Can\'t open %s\n", fstr_pp->fname);
                return 0;
            }

            readsize = sceLseek(rfd, 0, 2);
            if (readsize > 0)
            {
                printf("Can\'t open %s\n", fstr_pp->fname);
            }
            else
            {
                fstr_pp->search = 1;
                fstr_pp->fpCd.size = readsize;
            }

            ret = (readsize >= 0);
            sceClose(rfd);
        }
    }
    else
    {
        ret = 1;
    }

    return ret;
}
#endif

INCLUDE_RODATA(const s32, "main/cdctrl", D_00391AF0);
INCLUDE_ASM(const s32, "main/cdctrl", cdctrlReadSub);

INCLUDE_RODATA(const s32, "main/cdctrl", D_00391B40);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391B50);
INCLUDE_ASM(const s32, "main/cdctrl", intReadSub);

INCLUDE_ASM(const s32, "main/cdctrl", cdctrlReadData);

INCLUDE_ASM(const s32, "main/cdctrl", cdctrlReadDataOne);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlRead);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlReadOne);

INCLUDE_ASM(const s32, "main/cdctrl", usrMemcpy);

INCLUDE_RODATA(const s32, "main/cdctrl", D_00391BE0);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391C00);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391C20);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391C30);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391C40); // jumptable
INCLUDE_ASM(const s32, "main/cdctrl", CdctrlMemIntgDecode);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlStatus);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlReadWait);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlGetFileSize);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2SetChannel);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2Set);

INCLUDE_RODATA(const s32, "main/cdctrl", D_00391CC0);
INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2SetFileSeek);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2SetFileSeekChan);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2Seek);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2Play);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2PlayEndCheck);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2CheckBuffer);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2SetVolume);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2GetVolume);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2Stop);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2FileEnd);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2GetSample);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2GetFrame);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlFrame2WP2sample);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2GetSndTime);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlSndTime2WP2sample);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2GetSampleTmpBuf);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2GetSampleTmp);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2GetFrameTmp);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2GetSndTimeTmp);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2CdSample2SndTime);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWp2CdSample2Frame);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlXTRset);

INCLUDE_ASM(const s32, "main/cdctrl", GetIntAdrsCurrent);

INCLUDE_ASM(const s32, "main/cdctrl", GetIntSizeCurrent);

INCLUDE_ASM(const s32, "main/cdctrl", cdctrlSndFadeOut);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlSndFadeOut);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlSndFadeOutCheck);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlSndFadeOutWait);
