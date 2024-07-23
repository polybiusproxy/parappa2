#ifndef VRAMSAVE_H
#define VRAMSAVE_H

#include "common.h"

#include <eetypes.h>
#include <libgraph.h>
#include <sifdev.h>
#include <stdio.h>
#include <stdlib.h>

#include "os/tim2.h"

#define BMP(x) ((BMP_HEADER*)x)

typedef struct { // 0x38
    /* 0x00 */ u_char pad[2];
    /* 0x02 */ u_char id[2];
    /* 0x04 */ u_int tsize;
    /* 0x08 */ u_short reserved[2];
    /* 0x0c */ u_int ofsbit;
    /* 0x10 */ u_int header;
    /* 0x14 */ u_int imageW;
    /* 0x18 */ u_int imageH;
    /* 0x1c */ u_short planes;
    /* 0x1e */ u_short bitPerPixel;
    /* 0x20 */ u_int compression;
    /* 0x24 */ u_int imageSize;
    /* 0x28 */ u_int XpixcelPerMeter;
    /* 0x2c */ u_int YpixcelPerMeter;
    /* 0x30 */ u_int numberOfColors;
    /* 0x34 */ u_int colorsImportant;
    /* 0x38 */ u_char imageData[0];
} BMP_HEADER;

void VramSave(u_char *fname, int wsize, int hsize, int id);
void VramSaveBMP(u_char *fname, int wsize, int hsize, int id);
void VramSaveBMPDouble(u_char *fname, int wsize, int hsize, int id);
void VramTmpSave(u_char *save_pp, int wsize, int hsize, int id);
void VramTmpSaveOutBMP(u_char *fname, int wsize, int hsize, int id, u_char *dst1_pp, u_char *dst2_pp);

#endif // VRAMSAVE_H