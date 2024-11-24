#ifndef MENUDATA_H
#define MENUDATA_H

#include "common.h"

typedef enum {
    MENU_SPU_CHAN = 0,
    MENU_SPU_CHAN1 = 1,
    MENU_SPU_CHAN2 = 2,
    MENU_SPU_MAX = 3
} MENU_SPU_ENUM;

typedef enum {
    MDISK_00 = 0,
    MDISK_01 = 1,
    MDISK_02 = 2,
    MDISK_03 = 3,
    MDISK_04 = 4,
    MDISK_05 = 5,
    MDISK_06 = 6,
    MDISK_07 = 7,
    MDISK_08 = 8,
    MDISK_09 = 9,
    MDISK_MAX = 10
} MENU_DISKSND_ENUM;

void MenuMsgInit(void);

#endif