#include "common.h"
#include <libgraph.h>
#include "prlib/tim2.h"

PR_EXTERN int Tim2CheckFileHeader()
{
    return 1;
}

PR_EXTERN TIM2_PICTUREHEADER* Tim2GetPictureHeader(void *pTim2, int imgno)
{
    TIM2_FILEHEADER    *pFileHdr = (TIM2_FILEHEADER *)pTim2;
    TIM2_PICTUREHEADER *pPictHdr;

    if (imgno >= pFileHdr->Pictures)
        return NULL;

    if (pFileHdr->FormatId == 0x00)
        pPictHdr = (TIM2_PICTUREHEADER*)((char*)pTim2 + sizeof(TIM2_FILEHEADER));
    else
        pPictHdr = (TIM2_PICTUREHEADER*)((char*)pTim2 + 0x80);

    for (int i = 0; i < imgno; i++)
    {
        pPictHdr = (TIM2_PICTUREHEADER*)((char*)pPictHdr + pPictHdr->TotalSize);
    }

    return pPictHdr;
}

// TODO: fix non-matching jump table
INCLUDE_ASM(const s32, "prlib/tim2", Tim2GetMipMapPictureSize);
/* PR_EXTERN int Tim2GetMipMapPictureSize(TIM2_PICTUREHEADER *ph, int mipmap, int *pWidth, int *pHeight)
{
    int w, h, n;

    w = ph->ImageWidth  >> mipmap;
    h = ph->ImageHeight >> mipmap;

    if (pWidth != NULL)
        *pWidth = w;

    if (pHeight != NULL)
        *pHeight = h;

    n = w * h;

    switch (ph->ImageType)
    {
    case TIM2_RGB16:
        n *= 2;
        break;
    case TIM2_RGB24:
        n *= 3;
        break;
    case TIM2_RGB32:
        n *= 4;
        break;
    case TIM2_IDTEX4:
        n /= 2;
        break;
    case TIM2_IDTEX8:
        break;
    }

    n = (n + 15) & ~15;
    return n;
} */

PR_EXTERN TIM2_MIPMAPHEADER* Tim2GetMipMapHeader(TIM2_PICTUREHEADER *ph, int *pSize)
{
    TIM2_MIPMAPHEADER *pMmHdr;
    extern char mmsize[8];
    
    if (ph->MipMapTextures > 1)
        pMmHdr = (TIM2_MIPMAPHEADER *)((char *)ph + sizeof(TIM2_PICTUREHEADER));
    else
        pMmHdr = NULL;

    if (pSize != NULL)
        *pSize = mmsize[ph->MipMapTextures];

    return pMmHdr;
}

PR_EXTERN void* Tim2GetImage(TIM2_PICTUREHEADER *ph, int mipmap)
{
    void *pImage;

    if (mipmap >= ph->MipMapTextures)
        return NULL;

    pImage = (void*)((char*)ph + ph->HeaderSize);
    if (ph->MipMapTextures == 1)
        return pImage;
    
    TIM2_MIPMAPHEADER *pm = (TIM2_MIPMAPHEADER*)((char*)ph + sizeof(TIM2_PICTUREHEADER));
    for (int i = 0; i < mipmap; i++)
    {
        pImage = (void*)((char*)pImage + pm->MMImageSize[i]); // poly: should be MMImageSize[] instead of Size (with no reserved field after)
    }

    return pImage;
}

PR_EXTERN void* Tim2GetClut(TIM2_PICTUREHEADER *ph)
{
    void *pClut;

    if (ph->ClutColors == 0)
        pClut = NULL;
    else
        pClut = (void*)((char*)ph + ph->HeaderSize + ph->ImageSize);

    return pClut;
}

#if 1
INCLUDE_ASM(const s32, "prlib/tim2", Tim2GetClutColor);
#else
PR_EXTERN u_int Tim2GetClutColor(TIM2_PICTUREHEADER *ph, int clut, int no)
{
    u_char *pClut = (u_char*)Tim2GetClut(ph);
    int n;
    u_char r, g, b, a;

    if (pClut == NULL)
        return 0;

    switch (ph->ImageType)
    {
    case TIM2_IDTEX4:
        n = clut * 16 + no;
        break;
    case TIM2_IDTEX8:
        n = clut * 256 + no;
        break;
    default:
        return 0;
    }

    if (n > ph->ClutColors)
        return 0;

    switch((ph->ClutType << 8) | ph->ImageType)
    {
    case (((TIM2_RGB16 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB24 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB32 | 0x40) << 8) | TIM2_IDTEX4):
    case (( TIM2_RGB16         << 8) | TIM2_IDTEX8):
    case (( TIM2_RGB24         << 8) | TIM2_IDTEX8):
    case (( TIM2_RGB32         << 8) | TIM2_IDTEX8):
        if ((n & 31) >= 8)
        {
            if ((n & 31) < 16)
                n += 8;
            else if ((n & 31) < 24)
                n -= 8;
        }

        break;
    }

    switch (ph->ClutType & 0x3f)
    {
    case TIM2_RGB16:
        r = (u_char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) << 3) & 0xf8);
        g = (u_char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 2) & 0xf8);
        b = (u_char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 7) & 0xf8);
        a = (u_char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 8) & 0x80);
        break;
    case TIM2_RGB24:
        r = pClut[n * 3];
        g = pClut[n * 3 + 1];
        b = pClut[n * 3 + 2];
        a = 128;
        break;
    case TIM2_RGB32:
        r = pClut[n * 4];
        g = pClut[n * 4 + 1];
        b = pClut[n * 4 + 2];
        a = pClut[n * 4 + 3];
        break;
    default:
        r = 0;
        g = 0;
        b = 0;
        a = 0;
        break;
    }
    
    return ((a << 24) | (g << 16) | (b << 8) | r);
}
#endif

PR_EXTERN u_int Tim2SetClutColor(TIM2_PICTUREHEADER *ph, int clut, int no, u_int newcolor)
{
    u_char *pClut = (u_char*)Tim2GetClut(ph);

    if (pClut == NULL)
        return 0; // No CLUT data

    // Calculate the index of the color
    int n;
    switch (ph->ImageType)
    {
    case TIM2_IDTEX4:
        n = clut * 16 + no;
        break;
    case TIM2_IDTEX8:
        n = clut * 256 + no;
        break;
    default:
        return 0; // Illegal pixel color format
    }

    if (n > ph->ClutColors)
        return 0; // If assumed CLUT set, index color doesn't exist
    
    switch((ph->ClutType << 8) | ph->ImageType)
    {
    case (((TIM2_RGB16 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB24 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB32 | 0x40) << 8) | TIM2_IDTEX4):
    case (( TIM2_RGB16         << 8) | TIM2_IDTEX8):
    case (( TIM2_RGB24         << 8) | TIM2_IDTEX8):
    case (( TIM2_RGB32         << 8) | TIM2_IDTEX8):
        if ((n & 31) >= 8)
        {
            if ((n & 31) < 16)
                n += 8;
            else if ((n & 31) < 24)
                n -= 8;
        }

        break;
    }

    // Get color data according to the CLUT pixel format
    u_char r, g, b, a;
    switch (ph->ClutType & 0x3f)
    {
    case TIM2_RGB16: // 16 bits color
        u_char rr, gg, bb, aa;
        r  = (u_char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) << 3) & 0xf8);
        g  = (u_char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 2) & 0xf8);
        b  = (u_char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 7) & 0xf8);
        a  = (u_char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 8) & 0x80);

        rr = (u_char)((newcolor >> 3)   & 0x1f);
        gg = (u_char)((newcolor >> 11)  & 0x1f);
        bb = (u_char)((newcolor >> 19)  & 0x1f);
        aa = (u_char)((newcolor >> 31)  & 1);

        pClut[n * 2]     = (u_char)((((aa << 15) | (bb << 10) | (gg << 5) | rr))      & 0xff);
        pClut[n * 2 + 1] = (u_char)((((aa << 15) | (bb << 10) | (gg << 5) | rr) >> 8) & 0xff);
        break;
    case TIM2_RGB24: // 24 bits color
        r = pClut[n * 3];
        g = pClut[n * 3 + 1];
        b = pClut[n * 3 + 2];
        a = 128;

        pClut[n * 3]     = (u_char)((newcolor)       & 0xff);
        pClut[n * 3 + 1] = (u_char)((newcolor >> 8)  & 0xff);
        pClut[n * 3 + 2] = (u_char)((newcolor >> 16) & 0xff);
        break;
    case TIM2_RGB32: // 32 bits color
        r = pClut[n * 4];
        g = pClut[n * 4 + 1];
        b = pClut[n * 4 + 2];
        a = pClut[n * 4 + 3];

        pClut[n * 4]     = (u_char)((newcolor)       & 0xff);
        pClut[n * 4 + 1] = (u_char)((newcolor >> 8)  & 0xff);
        pClut[n * 4 + 2] = (u_char)((newcolor >> 16) & 0xff);
        pClut[n * 4 + 3] = (u_char)((newcolor >> 24) & 0xff);
        break;
    default: // Illegal pixel format
        r = 0;
        g = 0;
        b = 0;
        a = 0;
        break;
    }

   return ((a << 24) | (b << 16) | (g << 8) | r);
}

INCLUDE_ASM(const s32, "prlib/tim2", Tim2GetTexel);

INCLUDE_ASM(const s32, "prlib/tim2", Tim2SetTexel);

// TODO: fix rodata
#if 1
INCLUDE_ASM(const s32, "prlib/tim2", Tim2GetTextureColor);
#else
PR_EXTERN u_int Tim2GetTextureColor(TIM2_PICTUREHEADER *ph, int mipmap, int clut, int x, int y)
{
    if (Tim2GetImage(ph, mipmap) == NULL)
        return 0; // No assumed level texture data

    u_int t = Tim2GetTexel(ph, mipmap, (x >> mipmap), (y >> mipmap));
    switch (ph->ImageType)
    {
    case TIM2_RGB16:
        u_char r, g, b, a;
        r = (u_char)((t << 3) & 0xf8);
        g = (u_char)((t >> 7) & 0xf8);
        b = (u_char)((t >> 2) & 0xf8);
        a = (u_char)((t >> 8) & 0x80);
        return ((a << 24) | (g << 16) | (b << 8) | r);
    case TIM2_RGB24:
        return ((0x80 << 24) | (t & 0x00ffffff));
    case TIM2_RGB32:
        return t;
    case TIM2_IDTEX4:
    case TIM2_IDTEX8:
        return Tim2GetClutColor(ph, clut, t);
    }

    return 0;
}
#endif

PR_EXTERN void Tim2LoadPicture(TIM2_PICTUREHEADER *ph, u_int tbp, u_int cbp)
{
    Tim2LoadImage(ph);
    Tim2LoadClut(ph);
}

INCLUDE_ASM(const s32, "prlib/tim2", Tim2LoadImage);

INCLUDE_ASM(const s32, "prlib/tim2", Tim2LoadClut);

static void Tim2LoadTexture(int psm, u_int tbp, int tbw, int w, int h, u_long128 *pImage)
{
    int n, l;
    sceGsLoadImage li;
    u_long128 *p;

    switch (psm)
    {
    case SCE_GS_PSMZ32:
    case SCE_GS_PSMCT32:
        n = w * 4;
        break;

    case SCE_GS_PSMZ24:
    case SCE_GS_PSMCT24:
        n = w * 3;
        break;

    case SCE_GS_PSMZ16:
    case SCE_GS_PSMZ16S:
    case SCE_GS_PSMCT16:
    case SCE_GS_PSMCT16S:
        n = w * 2;
        break;
    
    case SCE_GS_PSMT8H:
    case SCE_GS_PSMT8:
        n = w;
        break;

    case SCE_GS_PSMT4HL:
    case SCE_GS_PSMT4HH:
    case SCE_GS_PSMT4:
        n = w / 2;
        break;

    default:
        return;
    }

    l = 32764 * 16 / n; // not to exceed the max. DMA transfer limit of 512KB, split then transfer
    for (int i = 0; i < h; i += l)
    {
        p = (u_long128*)((char*)pImage + n * i);
        if ((i + l) > h)
            l = h - i;

        // set up texture
        sceGsSetDefLoadImage(&li, tbp, tbw, psm, 0, i, w, l);
        FlushCache(0);

        // Transfer to VRAM
        sceGsExecLoadImage(&li, p);
        sceGsSyncPath(0, 0);
    }
}
