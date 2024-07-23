#include "dbug/vramsave.h"

/* data */
extern /* static */ TIM2SETINFO tim2setinfo;
extern sceGsStoreImage gs_simage;

static u_int BekiDat(u_int size)
{
    u_int mot = 2;
    u_int i;
    
    for (i = 1; size > mot; i++)
    {
        mot = mot << 1;
    }

    return i;
}

void VramSave(u_char *fname, int wsize, int hsize, int id)
{
    int         fd;
    u_char      fname_tmp[64];
    u_long128  *dst1_pp;
    int         i;
    
    sprintf(fname_tmp, "host:%s", fname);
    printf("vram save [%s]\n", fname_tmp);

    fd = sceOpen(fname_tmp, SCE_TRUNC | SCE_CREAT | SCE_WRONLY);
    if (fd < 0)
    {
        printf("file open error!![%s]\n", fname_tmp);
        return;
    }

    dst1_pp = malloc(wsize * hsize * 4);

    sceGsSetDefStoreImage(&gs_simage, (id * wsize * hsize) / 64, wsize / 64, 0, 0, 0, wsize, hsize);
    FlushCache(WRITEBACK_DCACHE);
    
    if (sceGsExecStoreImage(&gs_simage, dst1_pp) < 0)
    {
        printf("vramsave Timeout error!!\n");
        
        free(dst1_pp);
        sceClose(fd);
        return;
    }

    sceGsSyncPath(0, 0);
                    
    tim2setinfo.t2p.ImageSize   = wsize * hsize * 8;
    tim2setinfo.t2p.TotalSize   = tim2setinfo.t2p.ImageSize + sizeof(TIM2_PICTUREHEADER);
    tim2setinfo.t2p.ImageWidth  = wsize;
    tim2setinfo.t2p.ImageHeight = hsize * 2;
    tim2setinfo.t2p.GsTex0      = SCE_GS_SET_TEX0((wsize * hsize) / 32, wsize / 64, 0, BekiDat(wsize), BekiDat(hsize * 2), 0, 0, 0, 0, 0, 0, 0);

    sceLseek(fd, 0, 0);
    sceWrite(fd, &tim2setinfo, sizeof(tim2setinfo));
    sceLseek(fd, sizeof(tim2setinfo), 0);

    for (i = 0; i < hsize; i++)
    {
        sceWrite(fd, dst1_pp + (i * wsize) / 4, wsize * 4);
        sceWrite(fd, dst1_pp + (i * wsize) / 4, wsize * 4);
    }

    free(dst1_pp);
    sceClose(fd);
}

void VramSaveBMP(u_char *fname, int wsize, int hsize, int id)
{
    int fd;
    u_char fname_tmp[64];
    
    u_char *dst1_pp;
    u_char *tr_pp;
    u_char *img_pp;
    u_char *moto_pp;
    u_char *imgtr_pp;

    int i;
    int j;
    
    sprintf(fname_tmp, "host:%s", fname);
    printf("vram save BMP [%s]\n", fname_tmp);

    fd = sceOpen(fname_tmp, SCE_TRUNC | SCE_CREAT | SCE_WRONLY);
    if (fd < 0)
    {
        printf("file open error!![%s]\n", fname_tmp);
        return;
    }

    dst1_pp = malloc(wsize * hsize * 4);
    tr_pp   = malloc(wsize * hsize * 3 + sizeof(BMP_HEADER));

    sceGsSetDefStoreImage(&gs_simage, (id * wsize * hsize) / 64, wsize / 64, 0, 0, 0, wsize, hsize);
    FlushCache(WRITEBACK_DCACHE);

    if (sceGsExecStoreImage(&gs_simage, (u_long128*)dst1_pp) < 0)
    {
        printf("vramsave Timeout error!!\n");

        free(tr_pp);
        free(dst1_pp);
        sceClose(fd);
        return;
    }

    sceGsSyncPath(0, 0);
    
    BMP(tr_pp)->id[0] = 'B';
    BMP(tr_pp)->id[1] = 'M';
    BMP(tr_pp)->tsize = (wsize * hsize * 3) + (sizeof(BMP_HEADER) - 2);
    BMP(tr_pp)->reserved[0] = 0;
    BMP(tr_pp)->reserved[1] = 0;
    BMP(tr_pp)->ofsbit = sizeof(BMP_HEADER) - 2;
    BMP(tr_pp)->header = sizeof(BMP_HEADER) - 16;
    BMP(tr_pp)->imageW = wsize;
    BMP(tr_pp)->imageH = hsize;
    BMP(tr_pp)->planes = 1;
    BMP(tr_pp)->bitPerPixel = 24;
    BMP(tr_pp)->compression = 0;
    BMP(tr_pp)->imageSize = wsize * hsize * 3;
    BMP(tr_pp)->XpixcelPerMeter = 3780;
    BMP(tr_pp)->YpixcelPerMeter = 3780;
    BMP(tr_pp)->numberOfColors = 0;
    BMP(tr_pp)->colorsImportant = 0;

    img_pp  = BMP(tr_pp)->imageData;
    moto_pp = dst1_pp;

    for (i = 0; i < hsize; i++)
    {
        imgtr_pp = img_pp + (hsize - i - 1) * 3 * wsize;

        for (j = 0; j < wsize; j++, moto_pp++, imgtr_pp += 3)
        {
            imgtr_pp[2] = *moto_pp++;
            imgtr_pp[1] = *moto_pp++;
            imgtr_pp[0] = *moto_pp++;
        }
    }

    sceLseek(fd, 0, 0);
    sceWrite(fd, tr_pp + 2, wsize * hsize * 3 + (sizeof(BMP_HEADER) - 2));

    free(tr_pp);
    free(dst1_pp);
    sceClose(fd);
}

void VramSaveBMPDouble(u_char *fname, int wsize, int hsize, int id)
{
    int    fd;
    u_char fname_tmp[64];

    u_char *tr_pp;

    u_char *dst1_pp,  *dst2_pp;
    u_char *img_pp,   *img2_pp;
    u_char *moto_pp,  *moto2_pp;
    u_char *imgtr_pp, *imgtr2_pp;

    int i;
    int j;

    sprintf(fname_tmp, "host:%s", fname);
    printf("vram save BMP Double[%s]\n", fname_tmp);

    fd = sceOpen(fname_tmp, SCE_CREAT | SCE_TRUNC | SCE_WRONLY);
    if (fd < 0)
    {
        printf("file open error!![%s]\n", fname_tmp);
        return;
    }

    dst1_pp = malloc(wsize * hsize * 4);
    dst2_pp = malloc(wsize * hsize * 4);
    tr_pp   = malloc(wsize * hsize * 6 + sizeof(BMP_HEADER));

    sceGsSetDefStoreImage(&gs_simage, (id * wsize * hsize) / 64, wsize / 64, 0, 0, 0, wsize, hsize);
    FlushCache(0);

    if (sceGsExecStoreImage(&gs_simage, (u_long128*)dst1_pp) < 0)
    {
        printf("vramsave Timeout error!!\n");
        free(tr_pp);
        free(dst1_pp);
        free(dst2_pp);
        sceClose(fd);
        return;
    }

    sceGsSyncPath(0, 0);

    sceGsSetDefStoreImage(&gs_simage, ((id ^ 1) * wsize * hsize) / 64, wsize / 64, 0, 0, 0, wsize, hsize);
    FlushCache(0);

    if (sceGsExecStoreImage(&gs_simage, (u_long128*)dst2_pp) < 0)
    {
        printf("vramsave Timeout error!!\n");
        free(tr_pp);
        free(dst1_pp);
        free(dst2_pp);
        sceClose(fd);
        return;
    }

    sceGsSyncPath(0, 0);

    BMP(tr_pp)->id[0] = 'B';
    BMP(tr_pp)->id[1] = 'M';
    BMP(tr_pp)->tsize = (wsize * hsize * 6) + (sizeof(BMP_HEADER) - 2);
    BMP(tr_pp)->reserved[0] = 0;
    BMP(tr_pp)->reserved[1] = 0;
    BMP(tr_pp)->ofsbit = sizeof(BMP_HEADER) - 2;
    BMP(tr_pp)->header = sizeof(BMP_HEADER) - 16;
    BMP(tr_pp)->imageW = wsize;
    BMP(tr_pp)->imageH = hsize * 2;
    BMP(tr_pp)->planes = 1;
    BMP(tr_pp)->bitPerPixel = 24;
    BMP(tr_pp)->compression = 0;
    BMP(tr_pp)->imageSize = wsize * hsize * 6;
    BMP(tr_pp)->XpixcelPerMeter = 3780;
    BMP(tr_pp)->YpixcelPerMeter = 3780;
    BMP(tr_pp)->numberOfColors = 0;
    BMP(tr_pp)->colorsImportant = 0;

    img_pp   = img2_pp = BMP(tr_pp)->imageData;
    moto_pp  = dst1_pp;

    img2_pp += wsize * 3;
    moto2_pp = dst2_pp;

    for (i = 0; i < hsize; i++)
    {
        imgtr_pp  = img_pp  + (hsize - i - 1) * (wsize * 3) * 2;
        imgtr2_pp = img2_pp + (hsize - i - 1) * (wsize * 3) * 2;

        for (j = 0; j < wsize; j++, moto_pp++, moto2_pp++, imgtr_pp += 3, imgtr2_pp += 3)
        {
            imgtr_pp[2] = *moto_pp++;
            imgtr_pp[1] = *moto_pp++;
            imgtr_pp[0] = *moto_pp++;

            imgtr2_pp[2] = *moto2_pp++;
            imgtr2_pp[1] = *moto2_pp++;
            imgtr2_pp[0] = *moto2_pp++;
        }
    }

    sceLseek(fd, 0, 0);
    sceWrite(fd, tr_pp + 2, wsize * hsize * 6 + (sizeof(BMP_HEADER) - 2));
    
    free(tr_pp);
    free(dst2_pp);
    free(dst1_pp);
    sceClose(fd);
}

void VramTmpSave(u_char *save_pp, int wsize, int hsize, int id)
{
    sceGsSetDefStoreImage(&gs_simage, (id * wsize * hsize) / 64, wsize / 64, 0, 0, 0, wsize, hsize);
    FlushCache(0);

    if (sceGsExecStoreImage(&gs_simage, (u_long128*)save_pp) < 0)
    {
        printf("vramsave Timeout error!!\n");
        return;
    }

    sceGsSyncPath(0, 0);
}

void VramTmpSaveOutBMP(u_char *fname, int wsize, int hsize, int id, u_char *dst1_pp, u_char *dst2_pp)
{
    int    fd;
    u_char fname_tmp[64];

    u_char *tr_pp;

    u_char *img_pp,   *img2_pp;
    u_char *moto_pp,  *moto2_pp;
    u_char *imgtr_pp, *imgtr2_pp;

    int i;
    int j;

    sprintf(fname_tmp, "host:%s", fname);
    printf("vram save BMP Double[%s]\n", fname_tmp);

    fd = sceOpen(fname_tmp, SCE_TRUNC | SCE_CREAT | SCE_WRONLY);
    if (fd < 0)
    {
        printf("file open error!![%s]\n", fname_tmp);
        return;
    }

    tr_pp = malloc(wsize * hsize * 6 + sizeof(BMP_HEADER));
    BMP(tr_pp)->id[0] = 'B';
    BMP(tr_pp)->id[1] = 'M';
    BMP(tr_pp)->tsize = (wsize * hsize * 6) + (sizeof(BMP_HEADER) - 2);
    BMP(tr_pp)->reserved[0] = 0;
    BMP(tr_pp)->reserved[1] = 0;
    BMP(tr_pp)->ofsbit = sizeof(BMP_HEADER) - 2;
    BMP(tr_pp)->header = sizeof(BMP_HEADER) - 16;
    BMP(tr_pp)->imageW = wsize;
    BMP(tr_pp)->imageH = hsize * 2;
    BMP(tr_pp)->planes = 1;
    BMP(tr_pp)->bitPerPixel = 24;
    BMP(tr_pp)->compression = 0;
    BMP(tr_pp)->imageSize = wsize * hsize * 6;
    BMP(tr_pp)->XpixcelPerMeter = 3780;
    BMP(tr_pp)->YpixcelPerMeter = 3780;
    BMP(tr_pp)->numberOfColors = 0;
    BMP(tr_pp)->colorsImportant = 0;

    img_pp   = BMP(tr_pp)->imageData;

    img2_pp  = BMP(tr_pp)->imageData;
    img2_pp  = &img_pp[wsize * 3];

    moto_pp  = dst1_pp;
    moto2_pp = dst2_pp;
    
    for (i = 0; i < hsize; i++)
    {
        imgtr_pp  = img_pp  + (hsize - i - 1) * (wsize * 3) * 2;
        imgtr2_pp = img2_pp + (hsize - i - 1) * (wsize * 3) * 2;

        for (j = 0; j < wsize; j++, moto_pp++, moto2_pp++, imgtr_pp += 3, imgtr2_pp += 3)
        {
            imgtr_pp[2] = *moto_pp++;
            imgtr_pp[1] = *moto_pp++;
            imgtr_pp[0] = *moto_pp++;

            imgtr2_pp[2] = *moto2_pp++;
            imgtr2_pp[1] = *moto2_pp++;
            imgtr2_pp[0] = *moto2_pp++;
        }
    }

    sceLseek(fd, 0, 0);
    sceWrite(fd, tr_pp + 2, wsize * hsize * 6 + (sizeof(BMP_HEADER) - 2));
    
    free(tr_pp);
    sceClose(fd);
}
