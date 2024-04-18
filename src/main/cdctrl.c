#include "main/cdctrl.h"

/* sdata */
void *current_intg_adrs = 0;

/* bss - static */
extern CDCTRL_STR cdctrl_str;

/* sbss - static */
static int cdSampleTmp;

#define N		 4096	/* Size of ring buffer */
#define F		   18	/* Upper limit */
#define THRESHOLD	2

/* bss - static */
extern unsigned char RBuff[N + F - 1]; /* Ring buffer for INT decompression */

#define read()  *(fp_r)++;
#define write(x) *(fp_w)++ = (x);

#define UNCACHED(addr) (u_char*)((u_int)(addr) | 0x20000000)

u_int PackIntGetDecodeSize(u_char *fp_r)
{
    return *(u_int*)fp_r;
}

int PackIntDecode(u_char *fp_r, u_char *fp_w)
{
    u_int        moto_size;     /* Decode size                                  */
    int          rp    = N - F; /* Ring buffer position                         */

    unsigned int flags = 0;     /* LZSS flags                                   */
    u_char      *fp_w_end;      /* End of write buffer (buffer + decode size)   */

    int          i, c, c1, c2;
    
    asm("sync.l");

    moto_size = *(u_int*)fp_r;
    fp_w = UNCACHED(fp_w);
    fp_w_end = fp_w + moto_size;

    fp_r += 8;

    for (i = 0; i < N - F; i++)
    {
        RBuff[i] = 0;
    }

    while (fp_w != fp_w_end)
    {
        if (((flags >>= 1) & 256) == 0)
        {
            c = read();
            flags = c | 0xff00;
        }

        if (flags & 1)
        {
            c = read();
            write(c);

            RBuff[rp++] = c;
            rp &= (N - 1);
        }
        else
        {
            c1 = read();
            c2 = read();

            c1 |= ((c2 & 0xf0) << 4);
            c2 = (c2 & 0x0f) + THRESHOLD;

            for (i = 0; i <= c2; i++)
            {
                c = RBuff[(c1 + i) & (N - 1)];
                write(c);
                
                RBuff[rp++] = c;
                rp &= (N - 1);
            }
        }
    }

    asm("sync.l");
    return 0;
}

int PackIntDecodeWait(u_char *fp_r, u_char *fp_w, int wait_hline)
{
    u_int        moto_size;     /* Decode size                                  */
    u_char      *fp_w_end;      /* End of write buffer (buffer + decode size)   */

    int          rp    = N - F; /* Ring buffer position                         */
    unsigned int flags = 0;     /* LZSS flags                                   */

    int          i, c, c1, c2;
    
    printf("decode moto[%08x] saki[%08x]\n", fp_r, fp_w);
    asm("sync.l");

    moto_size = *(u_int*)fp_r;
    fp_w = UNCACHED(fp_w);
    fp_w_end = fp_w + moto_size;

    fp_r += 8;

    for (i = 0; i < N - F; i++)
    {
        RBuff[i] = 0;
    }

    while (1)
    {
        if (fp_w > fp_w_end)
        {
            printf(" over data pointer\n");
            break;
        }

        if (fp_w == fp_w_end)
        {
            break;
        }

        if (wait_hline < *T0_COUNT)
            MtcWait(1);

        if (((flags >>= 1) & 256) == 0)
        {
            c = read();
            flags = c | 0xff00;
        }

        if (flags & 1)
        {
            c = read();
            write(c);

            RBuff[rp++] = c;
            rp &= (N - 1);
        }
        else
        {
            c1 = read();
            c2 = read();

            c1 |= ((c2 & 0xf0) << 4);
            c2 = (c2 & 0x0f) + THRESHOLD;

            for (i = 0; i <= c2; i++)
            {
                c = RBuff[(c1 + i) & (N - 1)];
                write(c);
                
                RBuff[rp++] = c;
                rp &= (N - 1);
            }
        }
    }

    asm("sync.l");
    return 0;
}

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
#define WP2_SETMODE             0x800c /* Arg -> Mode           */
#define WP2_GETTIME             0x8010 /*        No args        */
#define WP2_READBUF             0x8017 /*        No args        */

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

#define PACK(x) ((PACKINT_FILE_STR*)x)

void intReadSub(void)
{
    int     read_pos;
    u_char *read_tmp_pp;
    u_char *head_read_pp;

    read_pos = 0;
    head_read_pp = usrMalloc(8192);

    printf("intg in!!!\n");

    while (1)
    {
        /* Read 2048 bytes into our header buffer */
        while (!cdctrlReadSub(cdctrl_str.fstr_pp, read_pos, 2048, (int)head_read_pp))
            MtcWait(1);

        /* Check if we loaded a valid INT */
        if (PACK(head_read_pp)->id != 0x44332211)
        {
            printf("INT FILE ERROR!![%s]\n", cdctrl_str.fstr_pp->fname);
            while (1)
                MtcWait(1);
        }

        FlushCache(0);

        if (PACK(head_read_pp)->head_size > 2048)
        {
            while (!cdctrlReadSub(cdctrl_str.fstr_pp, read_pos + 2048, ((PACK(head_read_pp)->head_size - 1) / 2048) * 2048, (int)(head_read_pp + 2048)))
                MtcWait(1);
        }

        read_pos += PACK(head_read_pp)->head_size + PACK(head_read_pp)->name_size;
        FlushCache(0);

        /* Check if there's data present */
        if (PACK(head_read_pp)->data_size != 0)
        {
            /* If we don't have a temp buffer, allocate it ourselves */
            if (cdctrl_str.tmp_area != NULL)
            {
                read_tmp_pp = (u_char*)(cdctrl_str.tmp_area - PACK(head_read_pp)->data_size);
                printf("buf use:%08x size:%08x\n", cdctrl_str.tmp_area - PACK(head_read_pp)->data_size, PACK(head_read_pp)->data_size);
            }
            else
            {
                read_tmp_pp = usrMalloc(PACK(head_read_pp)->data_size);
            }

            /* Read the data... */
            FlushCache(0);
            while (!cdctrlReadSub(cdctrl_str.fstr_pp, read_pos, PACK(head_read_pp)->data_size, (int)read_tmp_pp))
                MtcWait(1);

            /* ...and decode it */
            FlushCache(0);
            PackIntDecodeWait(read_tmp_pp, (u_char*)UsrMemAllocNext(), 230);

            FlushCache(0);
            if (cdctrl_str.tmp_area == NULL)
                usrFree(read_tmp_pp);

            read_pos += PACK(head_read_pp)->data_size;
        }

        /* Handle the decoded data */
        switch (PACK(head_read_pp)->ftype)
        {
            /* TIM2 textures */
            case FT_VRAM:
            {
                int i;
                printf("int file tim2 file in\n");

                for (i = 0; i < PACK(head_read_pp)->fnum; i++)
                {
                    Tim2Trans((void*)PACK(head_read_pp)->adr[i] + UsrMemAllocNext());
                }

                printf("int file tim2 file out\n");
                break;
            }

            /* Sounds */
            case FT_SND:
            {
                int i;
                printf("int file snd file in\n");

                /* TODO: name TAPCT commands */
                while (TapCt(0x8070, 0, 0))
                    MtcWait(1);

                for (i = 0; i < PACK(head_read_pp)->fnum / 2; i++)
                {
                    TapCt(i | 0x8030, PACK(head_read_pp)->adr[i] + UsrMemAllocNext(),
                        PACK(head_read_pp)->adr[i + 1] - PACK(head_read_pp)->adr[i]);

                    TapCt(i | 0x8040, PACK(head_read_pp)->adr[i + PACK(head_read_pp)->fnum / 2] + UsrMemAllocNext(),
                        PACK(head_read_pp)->adr[i + 1 + PACK(head_read_pp)->fnum / 2] - PACK(head_read_pp)->adr[i + PACK(head_read_pp)->fnum / 2]);
                }

                while (TapCt(0x8070, 0, 0))
                    MtcWait(1);

                printf("int file snd file out\n");
                break;
            }

            /* PaRappa's hat textures (TIM2) */
            case FT_R1:
            case FT_R2:
            case FT_R3:
            case FT_R4:
            {
                int i;

                if (GetHatRound() == PACK(head_read_pp)->ftype - 4)
                {
                    printf("int file tim2 round:%d file in\n", PACK(head_read_pp)->ftype - 3);

                    for (i = 0; i < PACK(head_read_pp)->fnum; i++)
                    {
                        Tim2Trans((void*)PACK(head_read_pp)->adr[i] + UsrMemAllocNext());
                    }

                    printf("int file tim2 roud file out\n");
                }

                break;
            }

            /* Models, animations, etc. */
            case FT_ONMEM:
            {
                int i;
                printf("mem cnt [%d]\n", PACK(head_read_pp)->fnum);

                for (i = 0; i < PACK(head_read_pp)->fnum; i++)
                {
                    UsrMemAlloc(PACK(head_read_pp)->adr[i + 1] - PACK(head_read_pp)->adr[i]);
                }
            }

            default:
            {
                printf("intg exit!!\n");
                usrFree(head_read_pp);
                return;
            }
        }
    }
}

void cdctrlReadData(void *x)
{
    while (!CdctrlSerch(cdctrl_str.fstr_pp))
        MtcWait(1);

    switch (cdctrl_str.fstr_pp->ftmode)
    {
        case FTMODE_INTG:
        {
            intReadSub();
            current_intg_adrs = (void*)cdctrl_str.read_area;
            break;
        }

        case FTMODE_ETC:
        {
            while (!cdctrlReadSub(cdctrl_str.fstr_pp, 0, cdctrl_str.fstr_pp->fpCd.size, cdctrl_str.read_area))
                MtcWait(1);
            
            break;
        }
    }

    FlushCache(0);
    MtcWait(1);

    cdctrl_str.status = 0;
    MtcExit();
}

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
    int     i;
    int *s_pp = sakip;
    int *m_pp = motop;

    for (i = (size >> 2) - 1; i != 1; i--)
    {
        *s_pp = *m_pp;

        s_pp++;
        m_pp++;
    }
}
#endif

void CdctrlMemIntgDecode(u_int rbuf, u_int setbuf)
{
    u_char *head_read_pp;
    u_int   next_rp;

    current_intg_adrs   = (void*)setbuf;
    head_read_pp        = usrMalloc(8192);
    next_rp             = PACK(rbuf)->head_size;

    while (1)
    {
        /* Copy the INT to our buffer */
        usrMemcpy(head_read_pp, (void*)rbuf, PACK(rbuf)->head_size);
        FlushCache(0);
        
        /* Check if we loaded a valid INT */
        if (PACK(head_read_pp)->id != 0x44332211)
        {
            printf("INT FILE ERROR!![%s]\n", cdctrl_str.fstr_pp->fname);
            while (1)
                MtcWait(1);
        }

        rbuf += PACK(head_read_pp)->head_size + PACK(head_read_pp)->name_size;
        FlushCache(0);

        /* Check if there's data present */
        if (PACK(head_read_pp)->data_size != 0)
        {
            /* Decode the data */
            PackIntDecodeWait((u_char*)rbuf, (u_char*)UsrMemAllocNext(), 230);
            rbuf += PACK(head_read_pp)->data_size;
        }

        FlushCache(0);

        /* Handle the decoded data */
        switch (PACK(head_read_pp)->ftype)
        {
            /* TIM2 textures */
            case FT_VRAM:
            {
                int i;
                printf("tim trans in\n");

                for (i = 0; i < PACK(head_read_pp)->fnum; i++)
                {
                    Tim2Trans((void*)PACK(head_read_pp)->adr[i] + UsrMemAllocNext());
                }

                printf("tim trans out\n");
                break;
            }

            /* Sounds */
            case FT_SND:
            {
                int i;

                for (i = 0; i < PACK(head_read_pp)->fnum / 2; i++)
                {
                    /* TODO: name TAPCT commands */
                    TapCt(i | 0x8030, PACK(head_read_pp)->adr[i] + UsrMemAllocNext(),
                        PACK(head_read_pp)->adr[i+1] - PACK(head_read_pp)->adr[i]);

                    TapCt(i | 0x8040, PACK(head_read_pp)->adr[i + PACK(head_read_pp)->fnum / 2] + UsrMemAllocNext(),
                        PACK(head_read_pp)->adr[i+1+PACK(head_read_pp)->fnum / 2] - PACK(head_read_pp)->adr[i + PACK(head_read_pp)->fnum / 2]);
                }

                while (TapCt(0x8070, 0, 0))
                    MtcWait(1);

                break;
            }

            /* PaRappa's hat textures (TIM2) */
            case FT_R1:
            case FT_R2:
            case FT_R3:
            case FT_R4:
            {
                int i;

                if (GetHatRound() == PACK(head_read_pp)->ftype - 4)
                {
                    printf("int file tim2 round:%d file in\n", PACK(head_read_pp)->ftype - 3);

                    for (i = 0; i < PACK(head_read_pp)->fnum; i++)
                    {
                        Tim2Trans((void*)PACK(head_read_pp)->adr[i] + UsrMemAllocNext());
                    }

                    printf("int file tim2 roud file out\n");
                }

                break;
            }

            /* Models, animations, etc. */
            case FT_ONMEM:
            {
                int i;
                printf("mem cnt [%d]\n", PACK(head_read_pp)->fnum);

                for (i = 0; i < PACK(head_read_pp)->fnum; i++)
                {
                    UsrMemAlloc(PACK(head_read_pp)->adr[i+1] - PACK(head_read_pp)->adr[i]);
                }

                break;
            }

            default:
            {
                printf("intg exit!!\n");
                usrFree(head_read_pp);
                return;
            }
        }
    }
}

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

    printf("channel change L[%d] R[%d]\n", Lchan, Rchan);
}

void CdctrlWP2Set(FILE_STR *fstr_pp)
{
    cdctrl_str.status = 1;
    cdctrl_str.error_status = 0;
    cdctrl_str.fstr_pp = fstr_pp;
    
    WP2Ctrl(WP2_SETMODE, fstr_pp->mchan);

    while (!CdctrlSerch(cdctrl_str.fstr_pp));

    WP2Ctrl(WP2_BGMSETTRPOINT, WP2_NONE);
    if (fstr_pp->frmode == FRMODE_PC)
        WP2Ctrl(0x8013, (int)cdctrl_str.fstr_pp->fname);
    else
        WP2Ctrl(0x8013, (int)&cdctrl_str.fstr_pp->fpCd);

    CdctrlWP2SetVolume(0);
    
    cdctrl_str.read_area = WP2Ctrl(0x8011,0);
    cdctrl_str.wp2chan[0] = 0;
    cdctrl_str.wp2chan[1] = 1;

    WP2Ctrl(WP2_SETCHANNEL, WP2_CONCAT(cdctrl_str.wp2chan[0], cdctrl_str.wp2chan[1]));
    WP2Ctrl(WP2_PRELOADBACK, WP2_NONE);
}

void CdctrlWP2SetFileSeek(FILE_STR *fstr_pp, int seek_pos)
{
    /* Stop the current WP2 playing */
    CdctrlWP2SetVolume(0);
    WP2Ctrl(WP2_STOP, WP2_NONE);

    if (cdctrl_str.fstr_pp != fstr_pp)
    {
        cdctrl_str.fstr_pp = fstr_pp;
        printf("!!max chan [%d]\n", fstr_pp->mchan);

        WP2Ctrl(WP2_SETMODE, cdctrl_str.fstr_pp->mchan);
        if (fstr_pp->frmode == FRMODE_PC)
            WP2Ctrl(0x8014, (int)cdctrl_str.fstr_pp->fname);
        else
            WP2Ctrl(0x8014, (int)&cdctrl_str.fstr_pp->fpCd);

        cdctrl_str.wp2chan[1] = 0;
        cdctrl_str.wp2chan[0] = 0;
    }

    /* Seek and preload */
    WP2Ctrl(WP2_SEEK, seek_pos);
    WP2Ctrl(WP2_PRELOADBACK, WP2_NONE);
}

void CdctrlWP2SetFileSeekChan(FILE_STR *fstr_pp, int seek_pos, u_char Lch, u_char Rch)
{
    /* Stop the current WP2 playing */
    CdctrlWP2SetVolume(0);
    WP2Ctrl(WP2_STOP, WP2_NONE);

    cdctrl_str.fstr_pp = fstr_pp;
    printf("!!max chan [%d]\n", fstr_pp->mchan);

    WP2Ctrl(WP2_SETMODE, cdctrl_str.fstr_pp->mchan);
    if (fstr_pp->frmode == FRMODE_PC)
        WP2Ctrl(0x8014, (int)cdctrl_str.fstr_pp->fname);
    else
        WP2Ctrl(0x8014, (int)&cdctrl_str.fstr_pp->fpCd);

    cdctrl_str.wp2chan[1] = 0;
    cdctrl_str.wp2chan[0] = 0;

    /* Seek, set the channels, and preload */
    WP2Ctrl(WP2_SEEK, seek_pos);
    CdctrlWP2SetChannel(Lch, Rch);
    WP2Ctrl(WP2_PRELOADBACK, WP2_NONE);
}

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
    vol <<= 8;
    
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

static int sndFadeTime = 0;

static void cdctrlSndFadeOut(void *x)
{
    int timer;

    for (timer = 0; timer < sndFadeTime; timer++)
    {
        CdctrlMasterVolSet(((sndFadeTime - timer) * 0x3fff) / sndFadeTime);
        MtcWait(1);
    }

    sndFadeTime = 0;
    MtcExit();
}

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

/* VRAM 0x00399374 on the NTSC-J proto */
int _sdata_pad_cdctrl_ = 0;
