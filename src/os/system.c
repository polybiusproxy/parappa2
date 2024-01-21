#include "os/system.h"

/*char *iop_module[11] = {
        "cdrom0:\\IRX\\SIO2MAN.IRX;1",
        "cdrom0:\\IRX\\PADMAN.IRX;1",
        "cdrom0:\\IRX\\LIBSD.IRX;1",
        "cdrom0:\\IRX\\SDRDRV.IRX;1",
        "cdrom0:\\IRX\\MODMIDI.IRX;1",
        "cdrom0:\\IRX\\MODHSYN.IRX;1",
        "cdrom0:\\IRX\\MODMSIN.IRX;1",
        "cdrom0:\\IRX\\MCMAN.IRX;1",
        "cdrom0:\\IRX\\MCSERV.IRX;1",
        "cdrom0:\\IRX\\WAVE2PS2.IRX;1",
        "cdrom0:\\IRX\\TAPCTRL.IRX;1",
};*/

extern char* iop_module[11];

int _end_addr;
int _stack_size_addr;

int oddeven_idx;

sceGsDrawEnv1* drawEnvP[5];

u_long128 GifPkCommon[8192];

void (*OsFuncAddr)();

sceGsDrawEnv1 drawEnvSp;
sceGsDrawEnv1 drawEnvZbuff;
sceGsDrawEnv1 drawEnvEnd;

int main() {
    mallocInit();

    while (1) {
        MtcInit();
        initSystem();

        MtcStart(systemCtrlMain);

        MtcQuit();
        exitSystem();
    }
}

INCLUDE_RODATA(const s32, "os/system", D_00391760);

INCLUDE_RODATA(const s32, "os/system", D_00391780);

INCLUDE_RODATA(const s32, "os/system", D_003917A0);

INCLUDE_RODATA(const s32, "os/system", D_003917C0);

INCLUDE_RODATA(const s32, "os/system", D_003917D8);

INCLUDE_RODATA(const s32, "os/system", D_003917F8);

INCLUDE_RODATA(const s32, "os/system", D_00391818);

INCLUDE_RODATA(const s32, "os/system", D_00391838);

INCLUDE_RODATA(const s32, "os/system", D_00391858);

INCLUDE_RODATA(const s32, "os/system", D_00391870);

INCLUDE_RODATA(const s32, "os/system", D_00391890);

int SetIopModule(void) {
    u_int i;

    sceSifInitRpc(0);
    sceCdInit(0);

    while (sceSifRebootIop("cdrom0:\\IRX\\IOPRP23.IMG;1") == 0);
    while (sceSifSyncIop() == 0);

    sceSifInitRpc(0);
    sceSifLoadFileReset();
    sceFsReset();

    sceCdInit(0);
    sceCdMmode(SCECdDVD);

    for (i = 0; i < PR_COUNTOF(iop_module); i++) {
        if (sceSifLoadModule(iop_module[i], 0, NULL) < 0) {
            printf("Can't load module [%s]\n", iop_module[i]);
            return 1;
        }
    }

    return 0;
}

static void firstClrFrameBuffer(void) {
    CLEAR_VRAM_DMA vclr_dma;

    vclr_dma.dmatag.qwc = 15;
    vclr_dma.dmatag.id = 0;
    vclr_dma.dmatag.next = (sceDmaTag*)&vclr_dma.giftag;

    vclr_dma.giftag.NLOOP = 14;
    vclr_dma.giftag.EOP = 1;
    vclr_dma.giftag.FLG = 0;
    vclr_dma.giftag.NREG = 1;
    vclr_dma.giftag.REGS0 = SCE_GIF_PACKED_AD;

    vclr_dma.clear.rgbaq.Q = 1.0f;

    sceGsSetDefDrawEnv(&vclr_dma.drEnv, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT, SCE_GS_ZNOUSE, SCE_GS_ZNOUSE);
    sceGsSetDefClear(
        &vclr_dma.clear, SCE_GS_ZNOUSE, 2048 - (SCREEN_WIDTH >> 1), 2048 - (SCREEN_HEIGHT >> 1), SCREEN_WIDTH,
        SCREEN_HEIGHT, 0, 0, 0, 0, 0
    );

    sceDmaSync(sceDmaGetChan(2), 0, 0x7fffffff);
    FlushCache(0);

    sceDmaSend(sceDmaGetChan(2), &vclr_dma);
    sceGsSyncPath(0, 0);
}

void initSystem(void) {
    SetIopModule();

    sceDevVif0Reset();
    sceDevVu0Reset();
    sceGsResetPath();
    sceDmaReset(SCE_DMA_VIF1);

    GPadInit();

    firstClrFrameBuffer();
    sceGsSyncV(0);
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

    sceGsSetDefDBuffDc(
        &DBufDc, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT / 2, SCE_GS_ZGEQUAL, SCE_GS_PSMZ32, SCE_GS_CLEAR
    );
    SetBackColor(0, 0, 0);

    *T0_MODE = T_MODE_CLKS_M | T_MODE_CUE_M;

    outbuf_idx = 0;

    FlushCache(0);
    sceGsSyncPath(0, 0);
    sceGsSwapDBuffDc(&DBufDc, outbuf_idx);

    drawEnvP[0] = &DBufDc.draw11;
    drawEnvP[1] = &DBufDc.draw01;

    drawEnvSp = DBufDc.draw01;
    drawEnvSp.frame1.FBP = ((SCREEN_WIDTH * 224 * 3) / 64) / 32;
    drawEnvP[2] = &drawEnvSp;
    sceGsSetHalfOffset(&drawEnvSp, 2048, 2048, 0);

    drawEnvZbuff = DBufDc.draw01;
    drawEnvZbuff.frame1.FBP = (SCREEN_WIDTH * SCREEN_HEIGHT) / 2048;
    drawEnvP[3] = &drawEnvZbuff;
    sceGsSetHalfOffset(&drawEnvZbuff, 2048, 2048, 0);

    drawEnvEnd = DBufDc.draw01;
    drawEnvEnd.frame1.FBP = (SCREEN_WIDTH * (512 * 2)) / 2048;
    drawEnvP[4] = &drawEnvEnd;
    sceGsSetHalfOffset(&drawEnvEnd, 2048, 2048, 0);

    CmnGifInit(GifPkCommon, PR_COUNTOF(GifPkCommon));
    CmnGifClear();

    SyoriLineInit(256);
    while (!sceGsSyncV(0));

    usrMallcInit();
}

void exitSystem(void) {
    sceGsSyncPath(0, 0);
    GPadExit();
    sceCdInit(SCECdEXIT);
    sceSifExitCmd();
}

void SetOsFuncAddr(void* func_pp) {
    OsFuncAddr = func_pp;
}

void osFunc(void) {
    int total_h_cnt;

    rand();

    SyoriLineCnt(16);

    CmnGifFlush();
    sceGsSyncPath(0, 0);

    SyoriLineCnt(16);
    SyoriLineDisp(132, 2);

    total_h_cnt = *T0_COUNT;
    oddeven_idx = sceGsSyncV(0);
    *T0_COUNT = 0;

    SyoriLineReset();
    SyoriLineCnt(0);

    GPadSysRead();
    GPadRead(pad);

    outbuf_idx ^= 1;
    CmnGifClear();

    if (outbuf_idx) {
        sceGsSetHalfOffset(&DBufDc.draw11, 2048, 2048, oddeven_idx ^ 1);
        sceGsSetHalfOffset2(&DBufDc.draw12, 2048, 2048, oddeven_idx ^ 1);
    } else {
        sceGsSetHalfOffset(&DBufDc.draw01, 2048, 2048, oddeven_idx ^ 1);
        sceGsSetHalfOffset2(&DBufDc.draw02, 2048, 2048, oddeven_idx ^ 1);
    }

    FlushCache(0);

    // wait for the GIF channel to be available
    while ( *D2_CHCR & D_CHCR_STR_M );

    if (sceGsSyncPath(0, 0))
        printf("SyncPath timeout \n");

    if (sceGsSwapDBuffDc(&DBufDc, outbuf_idx))
        printf("swap dma error\n");
}

void systemCtrlMain(void* xx) {
    MtcExec(mainStart, 1);
    SetOsFuncAddr(osFunc);

    while (1) {
        (OsFuncAddr)();
        MtcWait(1);
    }
}

static int FullAllocAndFree(void) {
    int stack_sizeX = _stack_size_addr;
    int endX = _end_addr;

    int heap_size = 0x01FFEFE0 - ((endX + 0x1000) + stack_sizeX);

    free(malloc(heap_size));
    return heap_size;
}

void mallocInit(void) {
    int size = FullAllocAndFree();
    scePrintf("HEAP SIZE[%08x]\n", size);
}
