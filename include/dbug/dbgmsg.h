#include "common.h"

#include "libgifpk.h"
#include "libdma.h"

//
// !! THIS SHOULD NOT GO IN HERE !!
// TODO(poly): move this to tim2.c when splitted
//

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
    /* 0x10 */ u_int Size[7];
    /* 0x2c */ char Reserved[4];
} TIM2_MIPMAPHEADER;

typedef struct { // 0x10
    /* 0x0 */ char ExHeaderId[4];
    /* 0x4 */ u_int UserSpaceSize;
    /* 0x8 */ u_int UserDataSize;
    /* 0xc */ u_int Reserved;
} TIM2_EXHEADER;

typedef struct { // 0x18
    /* 0x00 */ TIM2_FILEHEADER* fileH;
    /* 0x04 */ TIM2_PICTUREHEADER* picturH;
    /* 0x08 */ TIM2_MIPMAPHEADER* mipmapH;
    /* 0x0c */ TIM2_EXHEADER* exH;
    /* 0x10 */ u_long* clut_pp;
    /* 0x14 */ u_long* image_pp;
} TIM2INFO;

int GetTim2Info(/* s2 18 */ void* tim2_pp, /* s0 16 */ TIM2INFO* info_pp, /* s1 17 */ int maxinfo);
int Tim2Load(TIM2INFO* info_pp, int img_pos, int col_pos);