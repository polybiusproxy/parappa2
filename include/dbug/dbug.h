#ifndef DBUG_H
#define DBUG_H

#include <eetypes.h>

typedef struct { // 0x10
    /* 0x0 */ int debug_on;
    /* 0x4 */ int use_line;
    /* 0x8 */ int score_updown;
    /* 0xc */ int non_play;
} DBG_SELECT_STR;

typedef struct { // 0x14
    /* 0x00 */ u_char *msg_pp;
    /* 0x04 */ int *set_pp;
    /* 0x08 */ int min;
    /* 0x0c */ int max;
    /* 0x10 */ u_char **selmsg_pp;
} DBG_MODE_STR;

#endif