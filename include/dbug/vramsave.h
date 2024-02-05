#ifndef VRAMSAVE_H
#define VRAMSAVE_H

#include <eetypes.h>

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
    /* 0x38 */ u_char imageData[];
} BMP_HEADER;

#endif // VRAMSAVE_H