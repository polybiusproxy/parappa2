#ifndef PACK_H
#define PACK_H

#include "common.h"

#include <eetypes.h>

#define PACK(x) ((PACK_STR*)x)

typedef struct { // 0x10
    /* 0x0 */ u_int id;
    /* 0x4 */ char version[8];
    /* 0xc */ u_int fcnt;
} PACK_HEADER;

typedef struct { // 0x30
    /* 0x00 */ char fname[40];
    /* 0x28 */ u_int ofs;
    /* 0x2c */ u_int size;
} PACK_LIST;

typedef struct { // 0x10
    /* 0x0 */ PACK_HEADER pack_header;
    /* 0x10 */ PACK_LIST pack_list[0];
} PACK_STR;

#endif