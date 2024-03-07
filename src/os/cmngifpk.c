#include "os/cmngifpk.h"

/* static--move to bss! */
CMNGIF_PRI cmngif_pri[64];
int cmngif_pri_cnt;

u_long128 *cmnGifPkBase, *cmnGifPkCurrent, *cmnGifPkEnd;

u_long128 cmnGifTr[65];
sceGifPacket cmnGifPacket;

void CmnGifInit(void *buf_adr, int size)
{
    cmnGifPkEnd     = (buf_adr + size * 0x10);
    cmnGifPkCurrent = buf_adr;
    cmnGifPkBase    = buf_adr;
    
    sceGifPkInit(&cmnGifPacket, cmnGifTr);
    CmnGifClear();
}

void CmnGifClear(void)
{
    int i;

    cmnGifPkCurrent = cmnGifPkBase;

    for (i = 0; i < PR_ARRAYSIZE(cmngif_pri); i++)
    {
        cmngif_pri[i].pBase = 0;
    }

    cmngif_pri_cnt = 0;
    sceGifPkReset(&cmnGifPacket);
}

void CmnGifFlush(void)
{
    sceDmaChan *cmnDmaC;
    int i, j;

    if (cmngif_pri_cnt != 0) 
    {
        for (i = 0; i < PR_ARRAYSIZE(cmngif_pri) - 1 && cmngif_pri[i].pBase; i++) 
        {
            for (j = i + 1; j < PR_ARRAYSIZE(cmngif_pri) && cmngif_pri[j].pBase; j++) 
            {
                if (cmngif_pri[i].pri > cmngif_pri[j].pri)
                {
                    CMNGIF_PRI temp = cmngif_pri[i];
                    cmngif_pri[i] = cmngif_pri[j];
                    cmngif_pri[j] = temp;
                }
            }
        }

        {
            int i;
            for (i = 0; i < PR_ARRAYSIZE(cmngif_pri) && cmngif_pri[i].pBase; i++)
            {
                sceGifPkCall(&cmnGifPacket, cmngif_pri[i].pBase, 0, 0, 0);
            }
        }

        sceGifPkEnd(&cmnGifPacket, 0, 0, 0);
        sceGifPkTerminate(&cmnGifPacket);

        cmnDmaC = sceDmaGetChan(2);

        FlushCache(0);
        sceDmaSend(cmnDmaC, cmnGifPacket.pBase);
        sceGsSyncPath(0, 0);
    }
}

int CmnGifSetData(sceGifPacket *gifpk_pp, int pri)
{
    if (cmngif_pri_cnt >= 64)
    {
        printf("common packet set over\n");
        return 1;
    }
    else
    {
        cmngif_pri[cmngif_pri_cnt].pBase = gifpk_pp->pBase;
        cmngif_pri[cmngif_pri_cnt].pri = pri;
        
        cmngif_pri_cnt++;
        return 0;
    }
}

int CmnGifOpenCmnPk(sceGifPacket *gifpk_pp)
{
    u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, SCE_GIF_PACKED, 1), SCE_GIF_PACKED_AD };

    if (cmnGifPkCurrent > cmnGifPkEnd)
    {
        printf("common packet over\n");
        return -1;
    }
    else
    {
        sceGifPkInit(gifpk_pp, cmnGifPkCurrent);
        sceGifPkReset(gifpk_pp);

        sceGifPkRet(gifpk_pp, 0, 0, 0);

        sceGifPkOpenGifTag(gifpk_pp, *(u_long128*)giftag);
        sceGifPkAddGsAD(gifpk_pp, SCE_GS_TEXFLUSH, 0);
        sceGifPkAddGsAD(gifpk_pp, SCE_GS_TEX1_1, 0);
        sceGifPkAddGsAD(gifpk_pp, SCE_GS_TEST_1, SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, SCE_GS_ZALWAYS));
        sceGifPkAddGsAD(gifpk_pp, SCE_GS_PRMODECONT, SCE_GS_SET_PRMODECONT(1));
        sceGifPkAddGsAD(gifpk_pp, SCE_GS_CLAMP_1, SCE_GS_SET_CLAMP_1(0, 0, 0, 0, 0, 0));
        sceGifPkAddGsAD(gifpk_pp, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, 128, 1));

        return 0;
    }
}

int CmnGifCloseCmnPk(sceGifPacket *gifpk_pp, int pri)
{
    sceGifPkCloseGifTag(gifpk_pp);
    cmnGifPkCurrent = sceGifPkTerminate(gifpk_pp);

    if (cmnGifPkEnd < cmnGifPkCurrent)
    {
        printf("common packet over\n");
        return -1;
    }
    else
    {
        return CmnGifSetData(gifpk_pp, pri);
    }
}

u_long128* CmnGifAdrsGet(void)
{
    return cmnGifPkCurrent;
}

int CmnGifAdrsEnd(u_long128 *adr)
{
    cmnGifPkCurrent = adr;

    if (cmnGifPkEnd < adr)
    {
        printf("common packet over\n");
        return -1;
    }
    else
    {
        return 0;
    }
}

void CmnGifADPacketMake(sceGifPacket *gifP_pp, sceGsFrame *gsframe_pp)
{
    u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, SCE_GIF_PACKED, 1), SCE_GIF_PACKED_AD };

    sceGifPkInit(gifP_pp, CmnGifAdrsGet());
    sceGifPkReset(gifP_pp);
    sceGifPkCnt(gifP_pp, 0, 0, 0);

    sceGifPkOpenGifTag(gifP_pp, *(u_long128*)giftag);

    if (gsframe_pp)
        sceGifPkAddGsAD(gifP_pp, SCE_GS_FRAME_1, *(u_long*)gsframe_pp);

    sceGifPkAddGsAD(gifP_pp, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(gifP_pp, SCE_GS_TEX1_1, 0);
    sceGifPkAddGsAD(gifP_pp, SCE_GS_TEST_1, SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, SCE_GS_ZALWAYS));
    sceGifPkAddGsAD(gifP_pp, SCE_GS_PRMODECONT, SCE_GS_SET_PRMODECONT(1));
    sceGifPkAddGsAD(gifP_pp, SCE_GS_CLAMP_1, SCE_GS_SET_CLAMP_1(0, 0, 0, 0, 0, 0));
    sceGifPkAddGsAD(gifP_pp, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, 128, 1));
}

void CmnGifADPacketMake2(sceGifPacket *gifP_pp, sceGsFrame *gsframe_pp)
{
    u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, SCE_GIF_PACKED, 1), SCE_GIF_PACKED_AD };

    sceGifPkInit(gifP_pp, CmnGifAdrsGet());
    sceGifPkReset(gifP_pp);
    sceGifPkCnt(gifP_pp, 0, 0, 0);

    sceGifPkOpenGifTag(gifP_pp, *(u_long128*)giftag);

    if (gsframe_pp)
        sceGifPkAddGsAD(gifP_pp, SCE_GS_FRAME_2, *(u_long*)gsframe_pp);

    sceGifPkAddGsAD(gifP_pp, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(gifP_pp, SCE_GS_TEX1_2, 0);
    sceGifPkAddGsAD(gifP_pp, SCE_GS_TEST_2, SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, SCE_GS_ZALWAYS));
    sceGifPkAddGsAD(gifP_pp, SCE_GS_PRMODECONT, SCE_GS_SET_PRMODECONT(1));
    sceGifPkAddGsAD(gifP_pp, SCE_GS_CLAMP_2, 0);
    sceGifPkAddGsAD(gifP_pp, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(128, 128, 128, 128, 1));
}

int CmnGifADPacketMakeTrans(sceGifPacket *gifP_pp)
{
    sceDmaChan *cmnDmaC;

    sceGifPkCloseGifTag(gifP_pp);
    sceGifPkEnd(gifP_pp, 0, 0, 0);

    if (cmnGifPkEnd < sceGifPkTerminate(gifP_pp))
    {
        printf("common packet over\n");
        return -1;
    }
    else
    {
        cmnDmaC = sceDmaGetChan(SCE_DMA_GIF);
        FlushCache(0);

        sceDmaSend(cmnDmaC, gifP_pp->pBase);
        sceGsSyncPath(0,0);
        return 0;
    }
}
