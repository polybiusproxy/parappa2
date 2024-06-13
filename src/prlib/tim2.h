#ifndef PRLIB_TIM2_H
#define PRLIB_TIM2_H

#include <eetypes.h>

#define TIM2_NONE   (0)	// For use on ClutType when there is no CLUT data
#define TIM2_RGB16  (1)	// 16bit color (used for both ClutType and ImageType)
#define TIM2_RGB24  (2)	// 24bit color (only used with ImageType)
#define TIM2_RGB32  (3)	// 32bit color (used for both ClutType and ImageType)
#define TIM2_IDTEX4 (4)	// 16 color texture (only used with ImageType)
#define TIM2_IDTEX8 (5)	// 16 color texture (only used with ImageType)

typedef struct { // 0x10
    /* 0x0 */ char FileId[4];
    /* 0x4 */ u_char FormatVersion;
    /* 0x5 */ u_char FormatId;
    /* 0x6 */ u_short Pictures;
    /* 0x8 */ char Reserved[8];
} TIM2_FILEHEADER;

typedef struct { // 0x30
    /* 0x00 */ u_int TotalSize;
    /* 0x04 */ u_int ClutSize;
    /* 0x08 */ u_int ImageSize;
    /* 0x0c */ u_short HeaderSize;
    /* 0x0e */ u_short ClutColors;
    /* 0x10 */ u_char PictFormat;
    /* 0x11 */ u_char MipMapTextures;
    /* 0x12 */ u_char ClutType;
    /* 0x13 */ u_char ImageType;
    /* 0x14 */ u_short ImageWidth;
    /* 0x16 */ u_short ImageHeight;
    /* 0x18 */ u_long GsTex0;
    /* 0x20 */ u_long GsTex1;
    /* 0x28 */ u_int GsRegs;
    /* 0x2c */ u_int GsTexClut;
} TIM2_PICTUREHEADER;

typedef struct { // 0x30
    /* 0x00 */ u_long GsMiptbp1;
    /* 0x08 */ u_long GsMiptbp2;
    /* 0x10 */ u_int MMImageSize[0];
} TIM2_MIPMAPHEADER;

typedef struct { // 0x10
    /* 0x0 */ char ExHeaderId[4];
    /* 0x4 */ u_int UserSpaceSize;
    /* 0x8 */ u_int UserDataSize;
    /* 0xc */ u_int Reserved;
} TIM2_EXHEADER;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

int                 Tim2CheckFileHeader();

TIM2_PICTUREHEADER* Tim2GetPictureHeader(void *pTim2, int imgno);
int                 Tim2GetMipMapPictureSize(TIM2_PICTUREHEADER *ph, int mipmap, int *pWidth, int *pHeight);
TIM2_MIPMAPHEADER*  Tim2GetMipMapHeader(TIM2_PICTUREHEADER *ph, int *pSize);
void*               Tim2GetImage(TIM2_PICTUREHEADER *ph, int mipmap);
void*               Tim2GetClut(TIM2_PICTUREHEADER *ph);
u_int               Tim2GetClutColor(TIM2_PICTUREHEADER *ph, int clut, int no);

u_int               Tim2GetTexel(TIM2_PICTUREHEADER *ph, int mipmap, int x, int y);
u_int               Tim2SetTexel(TIM2_PICTUREHEADER *ph, int mipmap, int x, int y, u_int newtexel);

void                Tim2LoadPicture(TIM2_PICTUREHEADER *ph);
void                Tim2LoadImage(TIM2_PICTUREHEADER *ph);
u_int               Tim2LoadClut(TIM2_PICTUREHEADER *ph);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif