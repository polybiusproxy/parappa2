#include "main/cdctrl.h"
#include <stdio.h>

/* sdata */
void *current_intg_adrs = 0;
static int sndFadeTime = 0;
static int _pad_sdata_cdctrl_ = 0;

/* bss - static */
extern CDCTRL_STR cdctrl_str;

/* sbss - static */
static int cdSampleTmp;

/* .lit4 */
//float D_00398F00;
//float D_00398F04;
//float D_00398F08;
//float D_00398F0C;

/* Concats two shorts into an integer; for use on the WP2_SETMASTERVOL command */
#define WP2_CONCAT(x, y)        ((x << 16) | (y))
#define WP2_NONE                (0)

// WP2 commands
#define WP2_QUIT                0x0001 /*        No args        */
#define WP2_CLOSE               0x0003 /*        No args        */
#define WP2_PRELOAD             0x0004 /*        No args        */
#define WP2_STOP                0x0006 /*        No args        */
#define WP2_SETVOLDIRECT        0x0009 /* Arg -> Direct volume  */
#define WP2_SETMASTERVOL        0x000a /* Arg -> Master volume  */
#define WP2_SDINIT              0x000d /* Arg -> Status         */
#define WP2_INIT                0x000e /* Arg -> Mode           */
#define WP2_SETCHANNEL          0x000f /* Arg -> Channels       */
#define WP2_PRELOADBACK         0x0015 /*        No args        */
#define WP2_BGMSETTRPOINT       0x0016 /* Arg -> Transfer pos   */
#define WP2_BGMINIT             0x8000 /* Arg -> Block size     */
#define WP2_START               0x8005 /* Arg -> Seek position  */
#define WP2_SEEK                0x8007 /*        No args        */
#define WP2_GETMODE             0x800b /*        No args        */
#define WP2_GETTIME             0x8010 /*        No args        */
#define WP2_READBUF             0x8017 /*        No args        */

u_int PackIntGetDecodeSize(u_char *fp_r)
{
    return *(u_int*)fp_r;
}

#define N		 4096	/* Size of ring buffer */
#define F		   18	/* Upper limit for match_length */
#define THRESHOLD	2   /* encode string into position and length
						   if match_length is greater than this */

/* bss - static */
extern unsigned char RBuff[N + F - 1]; /* Ring buffer for INT decompression */

INCLUDE_ASM(const s32, "main/cdctrl", PackIntDecode);
#if 0
#define read(x)  if (fp_r < moto_size) break; x = *fp_r++;
#define write(x) if (fp_w < fp_w_end) break; *fp_w++ = x;

// INCLUDE_ASM(const s32, "main/cdctrl", PackIntDecode);
int PackIntDecode(/* a3 7 */ u_char *fp_r, /* a1 5 */ u_char *fp_w)
{
    
    
    /* t1 9 */ int c1;
    /* a2 6 */ int c2;
    /* a0 4 */ int c;


    /* a0 4 */ u_int moto_size = *(u_int*)fp_r;
    /* t3 11 */ int rp = N - F;
    /* t4 12 */ unsigned int flags = 0;
    /* t5 13 */ u_char *fp_w_end;

    /* t0 8 */ int i;

    asm("sync");
    moto_size = *(u_int*)fp_r;

    fp_w_end = ((u_int)fp_w | 0x20000000) + fp_r;

    for (i = 0; i < N - F; i++)
    {
        RBuff[i] = '\0';
    }

    //rp = N - F;
    //flags = 0;

    for (;;)
    {
        if (((flags >>= 1) & 256) == 0)
        {
            read(c);
            flags = c | 0xff00; /* Use higher byte cleverly to count eight */
        }

        if (flags & 1)
        {
            read(c);
            write(c);

            RBuff[rp++] = c;
            rp &= (N - 1);
        }
        else
        {
            read(c1);
            read(c2);

            c1 |= ((c2 & 0xf0) << 4);
            c2 = (c2 & 0x0f) + THRESHOLD;

            for (i = 0; i <= c2; i++)
            {
                c = RBuff[(c1 + c2) & (N - 1)];
                write(c);

                RBuff[rp++] = c;
                rp &= (N - 1);
            }
        }
    }

    asm("sync");
    return 0;
}
#endif

INCLUDE_ASM(const s32, "main/cdctrl", PackIntDecodeWait);

void CdctrlInit(void)
{
    WorkClear(&cdctrl_str, sizeof(cdctrl_str));

    printf("in wp2 init\n");
    WP2Init();
    printf("out wp2 init\n");

    WP2Ctrl(WP2_INIT, SCECdDVD);
    WP2Ctrl(WP2_SDINIT, WP2_NONE);
    WP2Ctrl(WP2_SETMASTERVOL, WP2_CONCAT(0x3fff, 0x3fff));
    WP2Ctrl(WP2_BGMINIT, 0x300);
}

void CdctrlQuit(void)
{
    WorkClear(&cdctrl_str, sizeof(cdctrl_str));
    WP2Ctrl(WP2_QUIT, WP2_NONE);
}

void CdctrlMasterVolSet(u_int vol)
{
    WP2Ctrl(WP2_SETMASTERVOL, WP2_CONCAT(vol, vol));
}

int CdctrlSerch(FILE_STR *fstr_pp)
{
    int ret = 0;
    int rfd;
    int readsize;

    if (fstr_pp->search != 0)
        return 1;

    if (fstr_pp->frmode == FRMODE_CD)
    {
        /* Search file from CD */
        ret = sceCdSearchFile(&fstr_pp->fpCd, fstr_pp->fname);
        if (ret)
        {
            fstr_pp->search = 1;
        }
        else
        {
            printf("CD SEARCH ERROR!![%s]\n", fstr_pp->fname);
        }
    }
    else
    {
        /* Search file from PC */
        rfd = sceOpen(fstr_pp->fname, SCE_RDONLY);
        if (rfd < 0)
        {
            printf("Can\'t open %s\n", fstr_pp->fname);
        }
        else
        {
            readsize = sceLseek(rfd, 0, SCE_SEEK_END);
            if (readsize < 0)
            {
                printf("Can\'t open %s\n", fstr_pp->fname);
            }
            else
            {
                fstr_pp->fpCd.size = readsize;
                fstr_pp->search = 1;

                ret = 1;
            }
        
            sceClose(rfd);
        }
    }

    return ret;
}

//INCLUDE_RODATA(const s32, "main/cdctrl", D_00391AF0);
static int cdctrlReadSub(FILE_STR *fstr_pp, int ofs, int size, int buf)
{
    if (fstr_pp->frmode == FRMODE_CD)
    {
        sceCdRMode cdmode   = { .spindlctrl = SCECdSpinNom };
        int        read_lsn = fstr_pp->fpCd.lsn + (ofs / 2048);

        if (!sceCdSeek(read_lsn))
        {
            printf("sceCdSeek Error! %s\n", fstr_pp->fname);
            return 0;
        }
        else 
        {
            while (sceCdSync(1))
                MtcWait(1);

            if (!sceCdRead(read_lsn, (size + 2047) / 2048, (void*)buf, &cdmode))
            {
                printf("sceCdRead Error! %s\n", fstr_pp->fname);
                return 0;
            }

            while (sceCdSync(1))
                MtcWait(1);

            if (sceCdGetError())
                return 0;
            
            FlushCache(0);
            return 1;
        }
    }
    else
    {
        int rfd = sceOpen(fstr_pp->fname, SCE_RDONLY);
        if (rfd < 0)
        {
            printf("Can\'t open %s\n", fstr_pp->fname);
            return 0;
        }

        if (sceLseek(rfd, ofs, SCE_SEEK_SET) < 0)
        {
            printf("Can\'t open %s\n", fstr_pp->fname);
            sceClose(rfd);
            return 0;
        }

        if (sceRead(rfd, (void*)buf, size) < 0)
        {
            printf("read error!! \n");
            sceClose(rfd);
            return 0;
        }

        sceClose(rfd);
        FlushCache(0);
        return 1;
    }

    return 0;
}

INCLUDE_RODATA(const s32, "main/cdctrl", D_00391B40);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391B50);
#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/cdctrl", intReadSub);
#else
void intReadSub(void)
{
    /* s5 21 */ int read_pos;
    /* s0 16 */ u_char *read_tmp_pp;
    /* s4 20 */ u_char *head_read_pp;
    /* s0 16 */ //int i;
    /* s2 18 */ //int i;
    /* s0 16 */ //int i;
    /* v0 2 */ //int i;

    head_read_pp = usrMalloc(0x2000);
    printf("intg in!!!\n");

    while (1)
    {
        while (!cdctrlReadSub(cdctrl_str.fstr_pp, read_pos, 2048, (int)head_read_pp))
            MtcWait(1);

        if (((PACKINT_FILE_STR*)head_read_pp)->id != 0x44332211)
        {
            printf("INT FILE ERROR!![%s]\n", cdctrl_str.fstr_pp->fname);
            while (1)
                MtcWait(1);
        }

        FlushCache(0);

        if (((PACKINT_FILE_STR*)head_read_pp)->head_size > 2048)
        {
            if ( cdctrlReadSub(cdctrl_str.fstr_pp, (int)read_pos + 2048, v5 >> 11 << 11, (int)&head_read_pp->adr[504]) )
                break;
        }
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/cdctrl", cdctrlReadData);
#else
void cdctrlReadData(void *x)
{
    while (!CdctrlSerch(cdctrl_str.fstr_pp))
        MtcWait(1);

    if (cdctrl_str.fstr_pp->ftmode == FTMODE_INTG)
    {
        intReadSub();
        current_intg_adrs = (void*)cdctrl_str.read_area;
    }
    else if (cdctrl_str.fstr_pp->ftmode == FTMODE_ETC)
    {
        while (!cdctrlReadSub(cdctrl_str.fstr_pp, 0, cdctrl_str.fstr_pp->fpCd.size, cdctrl_str.read_area))
            MtcWait(1);
    }

    FlushCache(0);

    MtcWait(1);
    cdctrl_str.status = 0;

    MtcExit();
}
#endif

static void cdctrlReadDataOne(void *x)
{
    while (!CdctrlSerch(cdctrl_str.fstr_pp))
        MtcWait(1);

    while (!cdctrlReadSub(cdctrl_str.fstr_pp, 0, cdctrl_str.fstr_pp->fpCd.size, cdctrl_str.read_area))
        MtcWait(1);

    MtcWait(1);
    cdctrl_str.status = 0;
    MtcExit();
}

void CdctrlRead(FILE_STR *fstr_pp, u_int buf, int tmpbuf)
{
    cdctrl_str.status = 1;
    cdctrl_str.error_status = 0;
    cdctrl_str.read_area = buf;
    cdctrl_str.tmp_area = tmpbuf;
    cdctrl_str.fstr_pp = fstr_pp;
    MtcExec(cdctrlReadData, MTC_TASK_CDCTRL);
}

void CdctrlReadOne(FILE_STR *fstr_pp, u_int buf, int tmpbuf)
{
    cdctrl_str.status = 1;
    cdctrl_str.error_status = 0;
    cdctrl_str.read_area = buf;
    cdctrl_str.tmp_area = tmpbuf;
    cdctrl_str.fstr_pp = fstr_pp;
    MtcExec(cdctrlReadDataOne, MTC_TASK_CDCTRL);
}

#ifndef NON_MATCHING
INCLUDE_ASM(const s32, "main/cdctrl", usrMemcpy);
#else
// さき(saki) -> Destination / もと(moto) -> Source
void usrMemcpy(/* a0 4 */ void *sakip, /* a1 5 */ void *motop, /* a2 6 */ int size)
{
    /* a2 6 */ int i;
    /* a1 5 */ int *m_pp = motop;
    /* a0 4 */ int *s_pp = sakip;

    for (i = size >> 2; i != 1 /* ? */; i--)
    {
        *s_pp = *m_pp;
    }
}
#endif

INCLUDE_RODATA(const s32, "main/cdctrl", D_00391BE0);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391C00);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391C20);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391C30);
INCLUDE_RODATA(const s32, "main/cdctrl", D_00391C40); // jumptable
INCLUDE_ASM(const s32, "main/cdctrl", CdctrlMemIntgDecode);

int CdctrlStatus(void)
{
    return cdctrl_str.status;
}

void CdctrlReadWait(void)
{
    do
    {
        MtcWait(1);
    } while (CdctrlStatus());  
}

u_int CdctrlGetFileSize(FILE_STR *fstr_pp)
{
    while (!CdctrlSerch(fstr_pp))
        MtcWait(1);

    return fstr_pp->fpCd.size;
}

void CdctrlWP2SetChannel(u_char Lchan, u_char Rchan)
{
    if (cdctrl_str.wp2chan[0] == Lchan &&
        cdctrl_str.wp2chan[1] == Rchan)
        return;

    WP2Ctrl(WP2_SETCHANNEL, WP2_CONCAT(Lchan, Rchan));
    cdctrl_str.wp2chan[0] = Lchan;
    cdctrl_str.wp2chan[1] = Rchan;

    printf("channel change L[%d] R[%d]\n\0", Lchan, Rchan);
}

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2Set);
#if 0
void CdctrlWP2Set(FILE_STR *fstr_pp)
{
    cdctrl_str.fstr_pp = fstr_pp;
    cdctrl_str.status = 1;
    cdctrl_str.error_status = 0;
    

    WP2Ctrl(0x800c, fstr_pp->mchan);

    while (!CdctrlSerch(fstr_pp));
    WP2Ctrl(0x16, 0);

    if (fstr_pp->frmode == FRMODE_PC)
        WP2Ctrl(0x8013, cdctrl_str.fstr_pp->fname);
    else
        WP2Ctrl(0x8013, &cdctrl_str.fstr_pp->fpCd);

    CdctrlWP2SetVolume(0);
    
    cdctrl_str.read_area = WP2Ctrl(0x8011,0);
    cdctrl_str.wp2chan[1] = 1;
    cdctrl_str.wp2chan[0] = 0;

    WP2Ctrl(WP2_SETCHANNEL, 1);
    WP2Ctrl(WP2_PRELOADBACK, 0);
}
#endif

INCLUDE_RODATA(const s32, "main/cdctrl", D_00391CC0);
INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2SetFileSeek);

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2SetFileSeekChan);

void CdctrlWP2Seek(int seek_pos)
{
    WP2Ctrl(WP2_STOP, WP2_NONE);
    WP2Ctrl(WP2_SEEK, seek_pos);
    WP2Ctrl(WP2_PRELOAD, WP2_NONE);
}

void CdctrlWP2Play(void)
{
    WP2Ctrl(WP2_START, WP2_NONE);
}

int CdctrlWP2PlayEndCheck(void)
{
    // TODO(poly): Name WP2 mode flags
    return WP2Ctrl(WP2_GETMODE, WP2_NONE) & 0x8000;
}

int CdctrlWP2CheckBuffer(void)
{
    return WP2Ctrl(WP2_READBUF, WP2_NONE);
}

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlWP2SetVolume);
#if 0
void CdctrlWP2SetVolume(u_short vol)
{
    cdctrl_str.volume = vol;
    WP2Ctrl(WP2_SETVOLDIRECT, WP2_CONCAT(vol, vol));
}
#endif

u_short CdctrlWP2GetVolume(void)
{
    return cdctrl_str.volume;
}

void CdctrlWp2Stop(void)
{
    WP2Ctrl(WP2_STOP, WP2_NONE);
}

void CdctrlWp2FileEnd(void)
{
    WP2Ctrl(WP2_STOP, WP2_NONE);
    WP2Ctrl(WP2_CLOSE, WP2_NONE);
}

int CdctrlWp2GetSample(void)
{
    return WP2Ctrl(WP2_GETTIME, WP2_NONE);
}

int CdctrlWp2GetFrame(void)
{
    return (WP2Ctrl(WP2_GETTIME, WP2_NONE) * 24) / 75;
}

int CdctrlFrame2WP2sample(int frame)
{
    return (frame * 75) / 24;
}

long int CdctrlWp2GetSndTime(float tempo)
{
    float beat = WP2Ctrl(WP2_GETTIME, WP2_NONE);

    beat = (tempo * 16.0f * beat) / 1875.0f;
    return (int)beat;
}

long int CdctrlSndTime2WP2sample(float tempo, long int beat)
{
    return (beat * 1875.0f) / (tempo * 16.0f);
}

void CdctrlWp2GetSampleTmpBuf(void)
{
    static int time_tmp_max = 0;
    int time_tmp = *T0_COUNT;

    cdSampleTmp = WP2Ctrl(WP2_GETTIME, WP2_NONE);
    time_tmp = *T0_COUNT - time_tmp;

    if (time_tmp > time_tmp_max)
    {
        printf("max cd time get[%d]\n", time_tmp);
        time_tmp_max = time_tmp;
    }
}

int CdctrlWp2GetSampleTmp(void)
{
    return cdSampleTmp;
}

int CdctrlWp2GetFrameTmp(void)
{
    int frame = cdSampleTmp;
    return (frame * 24) / 75;
}

long int CdctrlWp2GetSndTimeTmp(float tempo)
{
    float beat = cdSampleTmp;

    beat = (beat * tempo * 16.0f) / 1875.0f;
    return (int)beat;
}

long int CdctrlWp2CdSample2SndTime(long int samplecnt, float tempo)
{
    float beat = samplecnt;

    beat = (beat * tempo * 16.0f) / 1875.0f;
    return (int)beat;
}

long int CdctrlWp2CdSample2Frame(long int samplecnt)
{
    return (samplecnt * 24) / 75;
}

INCLUDE_ASM(const s32, "main/cdctrl", CdctrlXTRset);

void* GetIntAdrsCurrent(u_short num)
{
    return (void*)UsrMemGetAdr(num);
}

int GetIntSizeCurrent(u_short num)
{
    return UsrMemGetSize(num);
}

INCLUDE_ASM(const s32, "main/cdctrl", cdctrlSndFadeOut);
void cdctrlSndFadeOut(void *x);

void CdctrlSndFadeOut(int time)
{
    sndFadeTime = time;
    MtcExec(cdctrlSndFadeOut, MTC_TASK_CDCTRL);
}

int CdctrlSndFadeOutCheck(void)
{
    return (sndFadeTime == 0);
}

void CdctrlSndFadeOutWait(int time)
{
    CdctrlSndFadeOut(time);
    
    while (!CdctrlSndFadeOutCheck())
        MtcWait(1);
}
