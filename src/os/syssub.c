#include "os/syssub.h"

#include "os/system.h"

PAD_SYSD sysPad[2];

u_long128 pad_dma_buf[2][16];
u_long128 ChangeDrawAreaPacket[12];
sceDmaTag exl_dmatag;
USR_MALLOC_STR usr_malloc_str[256];

void WorkClear(void *clr_adrs, int size) {
    u_char *clr_pp = (char*)clr_adrs;

    while (size--) {
        *clr_pp++ = 0;
    }
}

void GPadInit(void) {
    int i;

    WorkClear(pad, sizeof(pad));
    WorkClear(sysPad, sizeof(sysPad));

    scePadInit(0);

    // Open each controller port
    for (i = 0; i < 2; i++) {
        if (!scePadPortOpen(i, 0, pad_dma_buf[i])) {
            printf("ERROR: scePadPortOpen[%d]\n", i);
        }
    }
}

void GPadExit(void) {
    int i;

    // Close each port
    for (i = 0; i < 2; i++) {
        scePadPortClose(i, 0);
    }

    scePadEnd();
}

INCLUDE_ASM(const s32, "os/syssub", GPadSysRead);

void padMakeData(PADD *pad_pp, u_short paddata) {
    pad_pp->old = pad_pp->shot;
    pad_pp->shot = paddata;
    pad_pp->one = (pad_pp->old ^ pad_pp->shot) & pad_pp->shot;
    pad_pp->off = (pad_pp->old ^ pad_pp->shot) & ~pad_pp->shot;
    
}

void pad0Clear(PADD *pad_pp) {
    padMakeData(pad_pp, 0);
}

void padOneOffBitCLear(PADD *pad_pp) {
    pad_pp->one = 0;
    pad_pp->off = 0;
}

void padNormalRead(PADD *pad_pp, u_char *rdata_pp) {
    padMakeData(pad_pp, ~((rdata_pp[2] << 8) | rdata_pp[3])); // what is this concat
}

INCLUDE_ASM(const s32, "os/syssub", padAnaRead);

INCLUDE_ASM(const s32, "os/syssub", padAnaRead0Clear);

INCLUDE_ASM(const s32, "os/syssub", padPrsRead);

INCLUDE_ASM(const s32, "os/syssub", padPrsRead0Clear);

INCLUDE_RODATA(const s32, "os/syssub", D_00391128);

INCLUDE_ASM(const s32, "os/syssub", padPrsTreate);
/*void padPrsTreate(PADD *pad_pp) {
    u_short pad_pos[12] = { 0x2000, 0x8000, 0x1000, 0x4000,
                            0x10, 0x20, 0x40, 0x80,
                            0x4, 0x8, 0x1, 0x2 };

    
    int i;
    for (i; i < 12; i++) {
        if ((pad_pp->shot & *pad_pos) != 0) {}
    }
}*/

void padActSet(PADD *pad_pp, PAD_SYSD *sysPad_pp) {
    if (pad_pp) {
        sysPad_pp->act_direct[0] = pad_pp->padvib[0];
        sysPad_pp->act_direct[1] = pad_pp->padvib[1];
    } else {
        sysPad_pp->act_direct[0] = 0;
        sysPad_pp->act_direct[1] = 0;
    }
}

void padActClear(PADD *pad_pp) {
    pad_pp->padvib[0] = 0;
    pad_pp->padvib[1] = 0;
}

INCLUDE_ASM(const s32, "os/syssub", padAnaMixPad);

INCLUDE_ASM(const s32, "os/syssub", GPadRead);

INCLUDE_RODATA(const s32, "os/syssub", D_00391140);

INCLUDE_ASM(const s32, "os/syssub", GetPadbit2PressId);
/*PAD_PRESS_ENUM GetPadbit2PressId(u_short padbit) {
    BIT2PR bit2pr[12];
    PAD_PRESS_ENUM ret;
    u_int i;

    for (i = 1; i < 12; i++) {
        if ((bit2pr[i].bit & padbit) != 0)
            return bit2pr[i].prn;
    }

    return ret;
}*/

u_char GetPadbit2PressPad(PADD *pad_pp, u_short padbit) {
    if (GetPadbit2PressId(padbit) == PAD_PR_None)
        return 0;
    else
        return pad_pp->press[padbit];
}

void SetBackColor(u_char R, u_char G, u_char B) {
    DBufDc.clear0.rgbaq.R = R;
    DBufDc.clear0.rgbaq.G = G;
    DBufDc.clear0.rgbaq.B = B;
    DBufDc.clear1.rgbaq = DBufDc.clear0.rgbaq;
}

void ChangeDrawArea(sceGsDrawEnv1 *env_pp) {
    u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, SCE_GIF_PACKED, 1), SCE_GIF_PACKED_AD };
    sceGifPacket gifpk;
    sceDmaChan *cmnDmaC;

    sceGifPkInit(&gifpk, ChangeDrawAreaPacket);
    sceGifPkReset(&gifpk);
    sceGifPkCnt(&gifpk, 0, 0, 0);

    sceGifPkOpenGifTag(&gifpk, *(u_long128*)giftag);
    sceGifPkAddGsAD(&gifpk, SCE_GS_FRAME_1, *(u_long*)&env_pp->frame1);
    sceGifPkAddGsAD(&gifpk, SCE_GS_FRAME_2, *(u_long*)&env_pp->frame1);
    sceGifPkAddGsAD(&gifpk, SCE_GS_XYOFFSET_1, *(u_long*)&env_pp->xyoffset1);
    sceGifPkAddGsAD(&gifpk, SCE_GS_XYOFFSET_2, *(u_long*)&env_pp->xyoffset1);
    sceGifPkAddGsAD(&gifpk, SCE_GS_SCISSOR_1, *(u_long*)&env_pp->scissor1);
    sceGifPkAddGsAD(&gifpk, SCE_GS_SCISSOR_2, *(u_long*)&env_pp->scissor1);
    sceGifPkCloseGifTag(&gifpk);

    sceGifPkEnd(&gifpk, 0, 0, 0);
    sceGifPkTerminate(&gifpk);

    cmnDmaC = sceDmaGetChan(2);

    FlushCache(0);

    sceDmaSend(cmnDmaC, gifpk.pBase);
    sceGsSyncPath(0,0);
}

void ChangeDrawAreaSetGifTag(sceGsDrawEnv1 *env_pp, sceGifPacket *gifpk_pp) {
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_FRAME_1, *(u_long*)&env_pp->frame1);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_FRAME_2, *(u_long*)&env_pp->frame1);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_XYOFFSET_1, *(u_long*)&env_pp->xyoffset1);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_XYOFFSET_2, *(u_long*)&env_pp->xyoffset1);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_SCISSOR_1, *(u_long*)&env_pp->scissor1);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_SCISSOR_2, *(u_long*)&env_pp->scissor1);
}

void ChangeDrawArea2(sceGsDrawEnv1 *env_pp) {
    u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, SCE_GIF_PACKED, 1), SCE_GIF_PACKED_AD };
    sceGifPacket gifpk;
    sceDmaChan *cmnDmaC;

    sceGifPkInit(&gifpk, ChangeDrawAreaPacket);
    sceGifPkReset(&gifpk);
    sceGifPkCnt(&gifpk, 0, 0, 0);

    sceGifPkOpenGifTag(&gifpk, *(u_long128*)giftag);
    sceGifPkAddGsAD(&gifpk, SCE_GS_FRAME_2, *(u_long*)&env_pp->frame1);
    sceGifPkAddGsAD(&gifpk, SCE_GS_XYOFFSET_2, *(u_long*)&env_pp->xyoffset1);
    sceGifPkAddGsAD(&gifpk, SCE_GS_SCISSOR_2, *(u_long*)&env_pp->scissor1);
    sceGifPkCloseGifTag(&gifpk);

    sceGifPkEnd(&gifpk, 0, 0, 0);
    sceGifPkTerminate(&gifpk);

    cmnDmaC = sceDmaGetChan(2);

    FlushCache(0);

    sceDmaSend(cmnDmaC, gifpk.pBase);
    sceGsSyncPath(0,0);
}

void ClearFrameBufferGifTag(sceGsFrame *draw_pp, sceGifPacket *gifpk_pp, u_char r, u_char g, u_char b, u_char a) {
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_FRAME_1, *(u_long*)draw_pp);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_TEST_1, SCE_GS_SET_TEST(1, 0, 0, 1, 0, 0, 1, 1));
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(r, g, b, a, 0));
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_SCISSOR_1, SCE_GS_SET_SCISSOR(0, 640, 0, 224));
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_PRMODECONT, 1);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_XYOFFSET_1, SCE_GS_SET_XYOFFSET(27648, 30976)); // ???
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_PRIM, SCE_GS_PRIM_SPRITE);
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_XYZ2, SCE_GS_SET_XYZ(27648, 30976, 1)); // ???
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_XYZ2, SCE_GS_SET_XYZ(37888, 34560, 1)); // ???
}

INCLUDE_ASM(const s32, "os/syssub", GGsSetLocalMoveImageort);

void GGsExecLocalMoveImage(sceGsMoveImage *lp) {

    exl_dmatag.qwc = 5;
    exl_dmatag.id = 0;
    exl_dmatag.next = (sceDmaTag*)lp;

    sceDmaSync(sceDmaGetChan(2), 0, 0x7FFFFFFF);
    FlushCache(0);
    sceDmaSend(sceDmaGetChan(2), &exl_dmatag);
}

u_int randMakeMax(u_int max) {
    u_int ret = (rand() & 0x7FFF) * max;
    return ret >> 15;
}

// static char* ByteStringSub(char *s, unsigned int n);
INCLUDE_ASM(const s32, "os/syssub", ByteStringSub);

INCLUDE_ASM(const s32, "os/syssub", ByteString);

INCLUDE_ASM(const s32, "os/syssub", ReportHeapUsage); // makes the whole thing NOT match
#if 0
u_int ReportHeapUsage(void) {
    struct mallinfo malloc_info = mallinfo();

    printf("_________________________________________\n");
    printf("total space allocated from system = %s\n", ByteString(malloc_info.arena));
    printf("total space in mmapped regions = %s\n", ByteString(malloc_info.hblkhd));
    printf("total allocated space = %s\n", ByteString(malloc_info.uordblks));
    printf("total non-inuse space = %s\n", ByteString(malloc_info.fordblks));
    printf("number of non-inuse chunks = %d\n", malloc_info.ordblks);
    printf("number of mmapped regions = %d\n", malloc_info.hblks);
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

    return malloc_info.uordblks;
}
#endif

void usrMallcInit(void) {
    WorkClear(usr_malloc_str, sizeof(usr_malloc_str));
}

void* usrMalloc(u_int size) {
    void *ret;
    int i;

    ret = malloc(size);
    if (ret == 0) 
    {
        printf("malloc is NG\n");
        return 0;
    }
    else
    {
        for (i = 0; i < 256; i++) 
        {
            if (usr_malloc_str[i].adrs == 0) 
            {
                usr_malloc_str[i].adrs = ret;
                usr_malloc_str[i].size = size;
                return ret;
            }
        }

        free(ret);
    }

    printf("malloc is NG\n");
    return 0;
}

void usrFree(void *adrs) {
    int i;

    for (i = 0; i < 256; i++) 
    {
        if ((usr_malloc_str[i].adrs) && (usr_malloc_str[i].adrs == adrs)) 
        { 
            free(adrs);
            usr_malloc_str[i].adrs = 0;
            usr_malloc_str[i].size = 0;
            return;
        }
    }

    printf("free is NG\n\0"); // Either figure out padding, or keep adding zero bytes to strings...
    return;
}

INCLUDE_ASM(const s32, "os/syssub", usrMallcReport);
#if 0
void usrMallcReport(void) {
    int i;
    int cnt = 0;

    printf("--- usr malloc report ---\n");

    for (i = 255; i >= 0; i--) {
        if (usr_malloc_str[i].adrs) {
            printf(" use  adr[%d] size[%d]\n", usr_malloc_str[i].adrs, usr_malloc_str[i].size);
            cnt++;
        }
    }

    printf("--- use cnt[%d] ---\n", cnt);
}
#endif

INCLUDE_ASM(const s32, "os/syssub", DrawGetDrawEnvP);

INCLUDE_ASM(const s32, "os/syssub", DrawGetFrameP);

INCLUDE_ASM(const s32, "os/syssub", DrawGetFbpPos);

int DrawGetTbpPos(DNUM_ENUM dnum) {
    int ret = 0;

    // TODO(poly): figure out what these values mean
    switch (dnum) 
    {
    case DNUM_SHOW:
        ret = 2240;
        if (outbuf_idx == 0) 
            ret = 0;
        break;
    case DNUM_DRAW:
        ret = 2240;
        if (outbuf_idx != 0) 
            ret = 0;
        break;
    case DNUM_VRAM0:
        ret = 0;
        break;
    case DNUM_VRAM1:
        ret = 2240;
        break;
    case DNUM_VRAM2:
        ret = 6720;
        break;
    case DNUM_ZBUFF:
        ret = 4480;
        break;
    case DNUM_END:
        ret = 10240;
        break;
    }

    return ret;
}
