#ifndef STDAT_H
#define STDAT_H

#include "common.h"

#include "main/etc.h"
#include "main/subt.h"
#include "main/cdctrl.h"
#include "main/drawctrl.h"

struct SCR_MAIN;

typedef struct { // 0xd0
    /* 0x00 */ PLAY_STEP play_step;
    /* 0x04 */ char *ply_name;
    /* 0x08 */ float tempo;
    /* 0x0c */ int stage;
    /* 0x10 */ EVENTREC *ev_pp;
    /* 0x14 */ struct SCR_MAIN *scr_pp;
    /* 0x18 */ JIMAKU_STR *jimaku_str_pp;
    /* 0x1c */ FILE_STR intfile;
    /* 0x48 */ FILE_STR sndfile[3];
    /* 0xcc */ TAPLVL_STR *taplvl_str_pp;
} STDAT_DAT;

typedef struct { // 0x38
    /* 0x00 */ FILE_STR ovlfile;
    /* 0x2c */ int stdat_dat_num;
    /* 0x30 */ STDAT_DAT *stdat_dat_pp;
    /* 0x34 */ char *strec_name;
} STDAT_REC;

FILE_STR file_str_logo_file;
FILE_STR file_str_menu_file;
FILE_STR file_str_extra_file[10];

STDAT_REC stdat_rec[19];

void stDatFirstFileSearch(void);

#endif