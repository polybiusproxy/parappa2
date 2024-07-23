#ifndef DRAWCTRL_H
#define DRAWCTRL_H

#include "common.h"

#include <eetypes.h>

#include "main/etc.h"

typedef enum {
    MEN_CTRL_BtoG = 0,
    MEN_CTRL_AtoAB = 1,
    MEN_CTRL_ABtoB = 2,
    MEN_CTRL_AtoB = 3,
    MEN_CTRL_GtoB = 4,
    MEN_CTRL_BtoBA = 5,
    MEN_CTRL_BAtoA = 6,
    MEN_CTRL_BtoA = 7
} MEN_CTRL_ENUM;

typedef enum {
    OBJBTHROW_TEACHER = 0,
    OBJBTHROW_PARAPPA = 1,
    OBJBTHROW_MAX = 2
} OBJBTHROW_TYPE;

typedef struct { // 0x1c
    /* 0x00 */ u_char use;
    /* 0x04 */ float xp;
    /* 0x08 */ float yp;
    /* 0x0c */ void *mdl_adr;
    /* 0x10 */ void *tim2_dat_pp;
    /* 0x14 */ void *homingpp;
    /* 0x18 */ int endTime;
} BTHROW_STR;

typedef struct { // 0xe14
    /* 0x000 */ int frame;
    /* 0x004 */ BTHROW_STR bthrow_str[128];
    /* 0xe04 */ int bthrow_str_cnt;
    /* 0xe08 */ float targetX;
    /* 0xe0c */ float targetY;
    /* 0xe10 */ void *targ_mdl_adr;
} BTHROW_CTRL;

typedef enum {
    ODAT_SPA = 0,
    ODAT_SPM = 1,
    ODAT_SPC = 2,
    ODAT_SPF = 3,
    ODAT_TM2 = 4,
    ODAT_CL2 = 5,
    ODAT_MAX = 6
} OBJDAT_TYPE;

typedef struct { // 0x14
    /* 0x00 */ u_short objdat_type;
    /* 0x02 */ u_short objdat_num;
    /* 0x04 */ int maxfr;
    /* 0x08 */ void *handle;
    /* 0x0c */ float subdat[2];
} OBJDAT;

typedef struct { // 0x1c
    /* 0x00 */ u_char first_flag;
    /* 0x01 */ u_char job_type;
    /* 0x02 */ u_short status;
    /* 0x04 */ u_short main_num;
    /* 0x06 */ u_short sub_num;
    /* 0x08 */ u_int start_time;
    /* 0x0c */ u_int end_time;
    /* 0x10 */ u_int now_time;
    /* 0x14 */ float focal_lng;
    /* 0x18 */ float defocus_lng;
} OBJACTPRG;

enum
{
    OBJACTPRG_ORG = 0,
    OBJACTPRG_NORMAL = 1,
    OBJACTPRG_TAP = 2,
    OBJACTPRG_MAX = 3
};

typedef struct { // 0x4
    /* 0x0 */ u_int maxFrame;
    /* 0x4 */ float frame[0];
} SPF_STR;

typedef enum {
    OBJSTR_REQ = 1,
    OBJSTR_ON = 2,
    OBJSTR_PRESSON = 4
} OBJSTR_ENUM;

typedef enum {
    OCTRL_NON = 0,
    OCTRL_ANI = 1,
    OCTRL_MDL = 2,
    OCTRL_CAM = 3,
    OCTRL_TM2 = 4,
    OCTRL_TCTRL = 5,
    OCTRL_SUB = 6,
    OCTRL_END = 7,
    OCTRL_LOOP_P = 8,
    OCTRL_NEXT = 9,
    OCTRL_LOOP = 10,
    OCTRL_EXIT = 11,
    OCTRL_BIBU = 12,
    OCTRL_ANIPOS = 13,
    OCTRL_CL2 = 14,
    OCTRL_BTHROW = 15,
    OCTRL_BHIT = 16,
    OCTRL_ANIPOSXX = 17,
    OCTRL_MOZAIKU = 18,
    OCTRL_MAX = 19
} OBJCTRL_TYPE;

typedef struct { // 0x20
    /* 0x00 */ u_int frame;
    /* 0x04 */ u_short objctrl_type;
    /* 0x06 */ u_short status;
    /* 0x08 */ u_short dat[5];
    /* 0x14 */ float subDat;
    /* 0x18 */ u_char PRflag;
    /* 0x1a */ u_short PRdata;
    /* 0x1c */ u_int PRtime;
} OBJCTRL;

typedef struct { // 0x24
    /* 0x00 */ int size;
    /* 0x04 */ OBJCTRL *objctrl_pp;
    /* 0x08 */ u_char PRflag;
    /* 0x0a */ u_short PRdata;
    /* 0x0c */ int PRtime;
    /* 0x10 */ int PRtimeOld;
    /* 0x14 */ u_char *PRpress;
    /* 0x18 */ OBJCTRL *current_pp;
    /* 0x1c */ u_int loop_time;
    /* 0x20 */ OBJCTRL *loop_pp;
} OBJSTR;

typedef struct { // 0x4
    /* 0x0 */ u_short obj_num;
    /* 0x2 */ u_short channel;
} OBJTAP;

typedef struct { // 0xc
    /* 0x0 */ int objtap_size;
    /* 0x4 */ OBJTAP *objtap_pp;
    /* 0x8 */ int ovl_cnt_num;
} OBJTAPSTR;

typedef struct { // 0x10
    /* 0x0 */ int num;
    /* 0x4 */ OBJACTPRG *objactprg[3];
} OBJACTPRG_CTRL;

typedef struct { // 0xc
    /* 0x0 */ short int tap_id;
    /* 0x2 */ short int req_no;
    /* 0x4 */ PLAYER_INDEX player_index;
    /* 0x8 */ u_char *pad_prs_pp;
} DR_TAP_REQ;

typedef struct { // 0x21c
    /* 0x000 */ int objdat_size;
    /* 0x004 */ OBJDAT *objdat_pp;
    /* 0x008 */ int objstr_size;
    /* 0x00c */ OBJSTR *objstr_pp;
    /* 0x010 */ int tapstr_size;
    /* 0x014 */ OBJSTR *tapstr_pp;
    /* 0x018 */ int objtapstr_size;
    /* 0x01c */ OBJTAPSTR *objtapstr_pp;
    /* 0x020 */ char *usrName;
    /* 0x024 */ int tap_id;
    /* 0x028 */ void *handle;
    /* 0x02c */ OBJACTPRG_CTRL objactprg_ctrl;
    /* 0x03c */ DR_TAP_REQ dr_tap_req[40];
} SCENE_OBJDATA;

typedef struct { // 0x20
    /* 0x00 */ u_int start_flame;
    /* 0x04 */ u_int end_flame;
    /* 0x08 */ u_int useDisp;
    /* 0x0c */ u_int drDisp;
    /* 0x10 */ u_char pri;
    /* 0x11 */ u_char use_flag;
    /* 0x14 */ int (*prg_pp)(void *para_pp, int frame, int first_f, int useDisp, int drDisp);
    /* 0x18 */ void *param_pp;
    /* 0x1c */ u_int condition;
} SCENECTRL;

typedef struct { // 0x8
    /* 0x0 */ int scenectrl_num;
    /* 0x4 */ SCENECTRL *scenectrl_pp;
} SCENESTR;

typedef struct { // 0x8
    /* 0x0 */ int scenestr_size;
    /* 0x4 */ SCENESTR *scenestr_pp;
} EVENTREC;

typedef struct { // 0xc
    /* 0x0 */ char *dbgmsg;
    /* 0x4 */ int  (*prg_pp)(int pad);
    /* 0x8 */ void (*msg_pp)(char *buf);
} DRAW_DBG_STR;

int DrawFadeDisp(void *para_pp, int frame, int first_f, int useDisp, int drDisp);
int DrawMoveDispIn(void *para_pp, int frame, int first_f, int useDisp, int drDisp);

int DrawVramClear(void *para_pp, int frame, int first_f, int useDisp, int drDisp);
void DrawCtrlTblChange(int ctrlTbl);
int DrawTapReqTbl(int atap, int pindx, u_char *prs_pp);

void Cl2MixTrans(int now_T, int max_T, u_char *cl2_0_pp, u_char *cl2_1_pp);

void DrawCtrlInit(EVENTREC *ev_pp, int ctrlTbl, void *dat_top);
void DrawCtrlQuit(void);
void DrawCtrlTimeSet(int time);

#endif