#include "os/tim2.h"

// TIM2 pixel format table
int tim2ColorTypeTbl[] =
{
    SCE_GS_PSMCT32, SCE_GS_PSMCT16, SCE_GS_PSMCT24, SCE_GS_PSMCT32,
    SCE_GS_PSMT4, SCE_GS_PSMT8
};

int SPstrncmp(char *sr1, char *sr2, int num)
{
    while (num) 
    {
        if (*sr1 > *sr2)
            return 1;
        if (*sr1 < *sr2)
            return -1;

        if (*sr1 == '\0')
            return 0;

        num--;

        sr1++;
        sr2++;
	}

    return 0;
}

int GetTim2Info(void *tim2_pp, TIM2INFO *info_pp, int maxinfo)
{
    int   i;
    void *current_pp;
    int   mm_ofs;
    int   k_size;
    int   pictures;

    for (i = 0; i < maxinfo; i++)
    {
        info_pp[i].fileH = NULL;
    }   

    info_pp->fileH = tim2_pp;

    if (SPstrncmp(TIM2(tim2_pp)->FileId, "TIM2", 4) && SPstrncmp(info_pp->fileH->FileId, "CLT2", 4))
    {
        info_pp->fileH    = NULL;
        info_pp->picturH  = NULL;
        info_pp->mipmapH  = NULL;
        info_pp->exH      = NULL;
        info_pp->clut_pp  = NULL;
        info_pp->image_pp = NULL;
        return 0;
    }

    pictures = info_pp->fileH->Pictures;
    if (maxinfo > pictures)
        maxinfo = pictures;

    current_pp = tim2_pp + sizeof(TIM2_FILEHEADER);
    
    for(i = 0; i < maxinfo; i++, info_pp++)
    {
        info_pp->fileH    = tim2_pp;
        info_pp->picturH  = NULL;
        info_pp->mipmapH  = NULL;
        info_pp->exH      = NULL;
        info_pp->clut_pp  = NULL;
        info_pp->image_pp = NULL;

        if (TIM2(tim2_pp)->FormatId != 0)
            info_pp->picturH = (TIM2_PICTUREHEADER*)PR_ALIGNU(current_pp, 128);
        else
            info_pp->picturH = (TIM2_PICTUREHEADER*)PR_ALIGNU(current_pp, sizeof(TIM2_FILEHEADER));

        current_pp = info_pp->picturH + 1;

        mm_ofs = 0;
        
        if (info_pp->picturH->MipMapTextures > 1)
        {
            info_pp->mipmapH = current_pp;

            mm_ofs = 32;
            if (info_pp->picturH->MipMapTextures > 4)
                mm_ofs = 48;

            current_pp += mm_ofs;
        }

        k_size = info_pp->picturH->HeaderSize - 48 - mm_ofs;
        if (k_size != 0)
        {
            info_pp->exH = current_pp;
            current_pp += k_size;
        }

        if (info_pp->picturH->ImageSize != 0)
        {
            if (TIM2(info_pp->fileH)->FormatId != 0)
                info_pp->image_pp = (u_long*)PR_ALIGNU(current_pp, 128);
            else
                info_pp->image_pp = (u_long*)PR_ALIGNU(current_pp, sizeof(TIM2_FILEHEADER));
            
            current_pp = (char*)info_pp->image_pp + info_pp->picturH->ImageSize;
        }

        if (info_pp->picturH->ClutSize != 0)
        {
            if (TIM2(info_pp->fileH)->FormatId != 0)
                info_pp->clut_pp = (u_long*)PR_ALIGNU(current_pp, 128);
            else
                info_pp->clut_pp = (u_long*)PR_ALIGNU(current_pp, sizeof(TIM2_FILEHEADER));

            current_pp = (char*)info_pp->clut_pp + info_pp->picturH->ClutSize;

            {
                int ct_tbl[4] = {0, 2, 1, 0};
                int Ct = info_pp->picturH->ClutType & 3;

                info_pp->picturH->GsTex0 &= SCE_GS_SET_TEX0(0x3fff, 0x3f, 0x3f, 0xf, 0xf, 0x1, 0x3, 0x3fff, 8, 0x1, 0x1f, 0x7);
                info_pp->picturH->GsTex0 |= SCE_GS_SET_TEX0(0, 0, 0, 0, 0, 0, 0, 0, ct_tbl[Ct], 0, 0, 0);
            }
        }
    }

    return pictures;
}

static int HsizeAdj(int w, int h, int mode)
{
    int tr_h = 8;
    
    if (mode >= 0)
    {
        if (mode >= 3)
        {
            if (mode < 21)
            {
                if (mode >= 19)
                    tr_h = 16;
            }
        }
    }

    return ((h + tr_h - 1) / tr_h) * tr_h;
}

int Tim2SetLoadImageI(TIM2INFO *info_pp, int img_pos, sceGsLoadImage *img_pp, int ofsx, int ofsy)
{
    u_long    dbw   = info_pp->picturH->GsTex0;
    short int dpsm;
    short int w;
    short int h;

    dbw  = PR_TEX0(info_pp->picturH).TBW;
    dpsm = tim2ColorTypeTbl[info_pp->picturH->ImageType];

    w = info_pp->picturH->ImageWidth;
    h = info_pp->picturH->ImageHeight;
    
    h = HsizeAdj(w, h, dpsm);
    sceGsSetDefLoadImage(img_pp, img_pos, dbw & 0x3f, dpsm, ofsx, ofsy, w, h);

    return w * h;
}

int Tim2SetLoadImageIX(TIM2INFO *info_pp, int img_pos, sceGsLoadImage *img_pp, TIM2INFO *infoX_pp)
{
    u_long    dbw   = info_pp->picturH->GsTex0;
    short int dpsm;
    short int w;
    short int h;
    int       ofsx;
    int       ofsy;

    dbw  = PR_TEX0(info_pp->picturH).TBW;
    dpsm = tim2ColorTypeTbl[info_pp->picturH->ImageType];

    w = infoX_pp->picturH->ImageWidth;
    h = infoX_pp->picturH->ImageHeight;

    ofsx = PR_REGS(infoX_pp->picturH).ta0;
    ofsy = PR_REGS(infoX_pp->picturH).ta1;
    
    h = HsizeAdj(w, h, dpsm);
    sceGsSetDefLoadImage(img_pp, img_pos, dbw & 0x3f, dpsm, ofsx, ofsy, w, h);

    return w * h;
}

int Tim2SetLoadImageC(TIM2INFO *info_pp, int col_pos, sceGsLoadImage *img_pp, int ofsx, int ofsy)
{
    int       col_type;
    short int dpsm;
    short int w;
    short int h;

    col_type = info_pp->picturH->ClutType & 0x1f;

    if (col_type)
    {
        dpsm = tim2ColorTypeTbl[col_type];
        w    = 8;
        h    = 2;

        if (info_pp->picturH->ImageType == TIM2_IDTEX8)
        {
            w = 16;
            h = 16;
        }

        h = HsizeAdj(w, h, dpsm);
        sceGsSetDefLoadImage(img_pp, col_pos, 1, dpsm, ofsx, ofsy, w, h);
    }
    
    return col_type;
}

int Tim2Load(TIM2INFO *info_pp, int img_pos, int col_pos)
{
    static sceGsLoadImage tp;

    if (Tim2SetLoadImageI(info_pp, img_pos, &tp, 0, 0))
    {
        FlushCache(0);

        sceGsExecLoadImage(&tp, (u_long128*)info_pp->image_pp);
        sceGsSyncPath(0, 0);
    }

    if (Tim2SetLoadImageC(info_pp, col_pos, &tp, 0, 0))
    {
        FlushCache(0);

        sceGsExecLoadImage(&tp, (u_long128*)info_pp->clut_pp);
        sceGsSyncPath(0, 0);
    }

    // Preserve TBP0, CBP, TCC and CLD from TEX0.
    info_pp->picturH->GsTex0 &= SCE_GS_SET_TEX0(0, 0x3f, 0x3f, 0xf, 0xf, 1, 0x3, 0, 0xf, 0x1, 0x1f, 0);
    // Now put our values in.
    info_pp->picturH->GsTex0 |= SCE_GS_SET_TEX0(img_pos, 0, SCE_GS_PSMCT32, 0, 0, 1, 0, col_pos, SCE_GS_PSMCT32, 0, 0, 1);

    return 1;
}

int MODE_TR_P(int mode, int ws, int hs)
{
    int rsize = ws * hs;

    if (mode == 1)
        rsize = ws * hs * 2;
    if (mode == 2)
        rsize *= 3;
    if (mode == 3)
        rsize <<= 2;
    if (mode == 4)
        rsize /= 2;

    return rsize;
}

int Tim2LoadSet(TIM2INFO *info_pp)
{
    int i;
    
    static sceGsLoadImage tp;
    u_long img_pos = info_pp->picturH->GsTex0;
    u_long col_pos = img_pos;
    
    int ws, hs;
    int dpsm, dbw, dtbp;
    int adrs;

    col_pos = PR_TEX0(info_pp->picturH).CBP;
    img_pos = PR_TEX0(info_pp->picturH).TBP0;

    if (Tim2SetLoadImageI(info_pp, img_pos, &tp, 0, 0))
    {
        FlushCache(0);
        sceGsExecLoadImage(&tp, (u_long128*)info_pp->image_pp);
        sceGsSyncPath(0, 0);
    }

    if (Tim2SetLoadImageC(info_pp, col_pos, &tp, 0, 0))
    {
        FlushCache(0);
        sceGsExecLoadImage(&tp, (u_long128*)info_pp->clut_pp);
        sceGsSyncPath(0, 0);
    }

    adrs = (int)info_pp->image_pp;
    dpsm = tim2ColorTypeTbl[info_pp->picturH->ImageType];
    ws   = info_pp->picturH->ImageWidth;
    hs   = info_pp->picturH->ImageHeight;

    for (i = 1; i < info_pp->picturH->MipMapTextures; i++)
    {
        adrs += info_pp->mipmapH->Size[i - 1];
        adrs = PR_ALIGN(adrs, 16);

        ws >>= 1;
        hs >>= 1;

        switch (i - 1)
        {
        case 0:
            dtbp = (*(sceGsMiptbp1*)&info_pp->mipmapH->GsMiptbp1).TBP1;
            dbw  = (*(sceGsMiptbp1*)&info_pp->mipmapH->GsMiptbp1).TBW1;
            break;
        case 1:
            dtbp = (*(sceGsMiptbp1*)&info_pp->mipmapH->GsMiptbp1).TBP2;
            dbw  = (*(sceGsMiptbp1*)&info_pp->mipmapH->GsMiptbp1).TBW2;
            break;
        case 2:
            dtbp = (*(sceGsMiptbp1*)&info_pp->mipmapH->GsMiptbp1).TBP3;
            dbw  = (*(sceGsMiptbp1*)&info_pp->mipmapH->GsMiptbp1).TBW3;
            break;
        case 3:
            dtbp = (*(sceGsMiptbp2*)&info_pp->mipmapH->GsMiptbp2).TBP4;
            dbw  = (*(sceGsMiptbp2*)&info_pp->mipmapH->GsMiptbp2).TBW4;
            break;
        case 4:
            dtbp = (*(sceGsMiptbp2*)&info_pp->mipmapH->GsMiptbp2).TBP5;
            dbw  = (*(sceGsMiptbp2*)&info_pp->mipmapH->GsMiptbp2).TBW5;
            break;
        default:
            dtbp = (*(sceGsMiptbp2*)&info_pp->mipmapH->GsMiptbp2).TBP6;
            dbw  = (*(sceGsMiptbp2*)&info_pp->mipmapH->GsMiptbp2).TBW6;
            break;
        }

        hs = HsizeAdj(ws, hs, dpsm);
        sceGsSetDefLoadImage(&tp, dtbp, dbw, dpsm, 0, 0, ws, hs);

        FlushCache(0);
        sceGsExecLoadImage(&tp, (u_long128*)adrs);
        sceGsSyncPath(0, 0);
    }

    return 1;
}

int Tim2LoadSetX(TIM2INFO *info_pp, TIM2INFO *infoX_pp)
{
    static sceGsLoadImage tp;
    u_long col_pos = info_pp->picturH->GsTex0;
    u_long img_pos = col_pos;

    col_pos = PR_TEX0(info_pp->picturH).CBP;
    img_pos = PR_TEX0(info_pp->picturH).TBP0;

    if (Tim2SetLoadImageIX(info_pp, img_pos, &tp, infoX_pp))
    {
        FlushCache(0);
        sceGsExecLoadImage(&tp, (u_long128*)infoX_pp->image_pp);
        sceGsSyncPath(0,0);
    }

    if (Tim2SetLoadImageC(infoX_pp, col_pos, &tp, 0, 0)) 
    {
        FlushCache(0);
        sceGsExecLoadImage(&tp, (u_long128*)infoX_pp->clut_pp);
        sceGsSyncPath(0,0);
    }

    return 1;
}

void Tim2Trans(void *adrs)
{
    TIM2INFO tim2info;

    if (!GetTim2Info(adrs, &tim2info, 1)) 
        printf("[0x%08x] is not tim2file\n", adrs);
    else
        Tim2LoadSet(&tim2info);
}

int Tim2TransX(void *adrs, int ofs_num)
{
    TIM2INFO info;
    TIM2INFO *info_x;
    int max_page;
    int ret = 0;

    if (GetTim2Info(adrs, &info, 1))
    {
        max_page = info.fileH->Pictures;

        if (ofs_num >= max_page)
        {
            ret = -1;
        }
        else
        {
            info_x = (TIM2INFO*)malloc(max_page * sizeof(TIM2INFO));
            GetTim2Info(adrs, info_x, max_page);

            if (ofs_num != 0)
                Tim2LoadSetX(&info, info_x + ofs_num);
            else
                Tim2LoadSet(&info);

            ret = 1;
            free(info_x);
        }
    }

    return ret;
}

static int GetModeMaxH(int w, int mode, int *trsize_pp)
{
    int line1_size = w;
    int ret;

    mode = (mode & 7);
    mode = 4 - mode;

    if (mode)
        line1_size *= mode;
    else
        line1_size /= 2;
        
    ret = 0x7fff0 / line1_size & 0xfffffff0;

    if (trsize_pp)
        *trsize_pp = ret * line1_size;

    return ret;
}

void Tim2Trans_TBP_MODE(void *adrs, int tbp, int mode)
{
    TIM2INFO tim2info;
    static sceGsLoadImage tp;
    u_long dbw;
    short int w;
    short int h;
    short int maxh;
    short int ofsy = 0;
    u_char *tr_adr;
    int trans_1size;
    short int h_tmp;

    GetTim2Info(adrs, &tim2info, 1);

    w    = tim2info.picturH->ImageWidth;
    h    = tim2info.picturH->ImageHeight;
    dbw  = PR_TEX0(tim2info.picturH).TBW;
    maxh = GetModeMaxH(w, mode, &trans_1size);

    tr_adr = (char*)tim2info.image_pp;

    do
    {
        if (maxh < h)
            h_tmp = maxh;
        else
            h_tmp = h;

        sceGsSetDefLoadImage(&tp, tbp, dbw, mode, 0, ofsy, w, h_tmp);

        FlushCache(0);
        sceGsExecLoadImage(&tp, (u_long128*)tr_adr);
        sceGsSyncPath(0, 0);

        tr_adr += trans_1size;
        ofsy += maxh;
        h -= maxh;
    } while (h > 0);
}

void Tim2TransColor_TBP(void *adrs, int tbp)
{
    TIM2INFO tim2info;
    static sceGsLoadImage tp;
    short int w;
    short int h;
    u_char *tr_adr;
    int col_type;
    int mode;
    
    GetTim2Info(adrs, &tim2info, 1);
    col_type = tim2info.picturH->ClutType & 0x1F;
    mode = tim2ColorTypeTbl[col_type];
    
    w = 8;
    h = 2;
    
    if (tim2info.picturH->ImageType == TIM2_IDTEX8) 
    {
        w = 16;
        h = 16;
    }
    
    tr_adr = (u_char*)tim2info.clut_pp;
    
    sceGsSetDefLoadImage(&tp, tbp, 1, mode, 0, 0, w, h);
    
    FlushCache(0);
    sceGsExecLoadImage(&tp, (u_long128*)tr_adr);
    sceGsSyncPath(0, 0);
}

static char _pad_[2720]; /* bss pad - TODO: remove once RBuff (cdctrl.c) is added */
