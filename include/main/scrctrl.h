#ifndef SCRCTRL_H
#define SCRCTRL_H

#include "common.h"

#include "os/syssub.h"
#include "os/system.h"
#include "main/subt.h"
#include "main/cdctrl.h"
#include "main/stdat.h"
#include "main/etc.h"

typedef struct { // 0x6
    /* 0x0 */ u_short prg;
    /* 0x2 */ u_short key;
    /* 0x4 */ u_short volume;
} SNDTAP;

typedef struct { // 0x10
    /* 0x0 */ int hd_num;
    /* 0x4 */ int bd_num;
    /* 0x8 */ int sndtap_num;
    /* 0xc */ SNDTAP *sndtap_pp;
} SNDREC;

typedef enum {
    TAPSCODE_NORMAL,
    TAPSCODE_QUESTION,
    TAPSCODE_ANSWER_F,
    TAPSCODE_ANSWER,
    TAPSCODE_MAX
} TAPSCODE_ENUM;

typedef struct { // 0xc
    /* 0x0 */ int frame;
    /* 0x4 */ int actor;
    /* 0x8 */ short int sound;
} TAPCT;

typedef struct { // 0x38
    /* 0x00 */ int time;
    /* 0x04 */ short int KeyIndex;
    /* 0x08 */ TAPCT tapct[4];
} TAPDAT;

typedef struct { // 0x34
    /* 0x00 */ PLAYER_CODE player_code;
    /* 0x04 */ int tapdat_size;
    /* 0x08 */ TAPDAT *tapdat_pp;
    /* 0x0c */ int tapdatNG_size;
    /* 0x10 */ TAPDAT *tapdatNG_pp;
    /* 0x14 */ int taptimeStart;
    /* 0x18 */ int taptimeEnd;
    /* 0x1c */ int coolup;
    /* 0x20 */ int chan[2];
    /* 0x28 */ u_char *tapsubt[2];
    /* 0x30 */ TAPSCODE_ENUM tapscode;
} TAPSET;

typedef struct { // 0x8
    /* 0x0 */ int tapset_size;
    /* 0x4 */ TAPSET *tapset_pp;
} TAPSTR;

typedef enum {
    KiNO = 0,
    KiTR = 1,
    KiCI = 2,
    KiXX = 3,
    KiSQ = 4,
    KiL1 = 5,
    KiR1 = 6,
    KiMAX = 7,
    _N = 0,
    _A = 1,
    _O = 2,
    _X = 3,
    _H = 4,
    _L = 5,
    _R = 6
} KEY_INDEX_ENUM;

typedef struct { // 0x10
    /* 0x0 */ short int job;
    /* 0x2 */ short int sub;
    /* 0x4 */ int jobd1;
    /* 0x8 */ int jobd2;
    /* 0xc */ int data;
} SCRREC;

typedef struct { // 0x8
    /* 0x0 */ int time;
    /* 0x4 */ short int chan[2];
} SCR_CHAN_AUTO;

typedef enum {
    GTIME_CD0 = 0,
    GTIME_CD1 = 1,
    GTIME_CD2 = 2,
    GTIME_VSYNC = 3
} GET_TIME_TYPE;

typedef struct { // 0x2c
    /* 0x00 */ char gtime_type;
    /* 0x01 */ char cdChan[2];
    /* 0x04 */ int scrrec_num;
    /* 0x08 */ SCRREC *scrrec_pp;
    /* 0x0c */ int lineTime;
    /* 0x10 */ int lineTimeFrame;
    /* 0x14 */ int subtLine;
    /* 0x18 */ int drawLine;
    /* 0x1c */ int scr_chan_auto_size;
    /* 0x20 */ SCR_CHAN_AUTO *scr_chan_auto_pp;
    /* 0x24 */ int ofsCdtime;
    /* 0x28 */ float tempo;
} SCR_CTRL;

typedef struct SCR_MAIN { // 0x10
    /* 0x0 */ int sndrec_num;
    /* 0x4 */ SNDREC *sndrec_pp;
    /* 0x8 */ int scr_ctrl_num;
    /* 0xc */ SCR_CTRL *scr_ctrl_pp;
} SCR_MAIN;

typedef struct { // 0xc
    /* 0x0 */ int ofs_frame;
    /* 0x4 */ short int onKey;
    /* 0x6 */ char othOn;
    /* 0x7 */ u_char othNum;
    /* 0x8 */ KEY_INDEX_ENUM key;
} SCR_TAP_MEMORY;

typedef enum {
    SCRLINE_NODATA = -1,
    SCRLINE_COOL = 0,
    SCRLINE_GOOD = 1,
    SCRLINE_BAD = 2,
    SCRLINE_AWFUL = 3,
    SCRLINE_toCOOL = 4,
    SCRLINE_fromCOOL = 5,
    SCRLINE_toUP = 6,
    SCRLINE_toDOWN = 7,
    SCRLINE_GAMEOVER = 8,
    SCRLINE_COOL2 = 9,
    SCRLINE_GOOD2 = 10,
    SCRLINE_BAD2 = 11,
    SCRLINE_AWFUL2 = 12,
    SCRLINE_toCOOL2 = 13,
    SCRLINE_fromCOOL2 = 14,
    SCRLINE_toUP2 = 15,
    SCRLINE_toDOWN2 = 16,
    SCRLINE_GAMEOVER2 = 17,
    SCRLINE_COOL3 = 18,
    SCRLINE_GOOD3 = 19,
    SCRLINE_BAD3 = 20,
    SCRLINE_AWFUL3 = 21,
    SCRLINE_toCOOL3 = 22,
    SCRLINE_fromCOOL3 = 23,
    SCRLINE_toUP3 = 24,
    SCRLINE_toDOWN3 = 25,
    SCRLINE_GAMEOVER3 = 26,
    SCRLINE_MAX = 27,
    SCRLINE_EXAMG1 = 9,
    SCRLINE_EXAMG2 = 10,
    SCRLINE_EXAMG3 = 11,
    SCRLINE_EXAMG4 = 12,
    SCRLINE_EXAMG5 = 13,
    SCRLINE_EXAMB1 = 14,
    SCRLINE_EXAMB2 = 15,
    SCRLINE_EXAMB3 = 16,
    SCRLINE_EXAMB4 = 17,
    SCRLINE_EXAMB5 = 18,
    SCRLINE_EXAMA1 = 19,
    SCRLINE_EXAMA2 = 20,
    SCRLINE_EXAMA3 = 21,
    SCRLINE_EXAMA4 = 22,
    SCRLINE_EXAMA5 = 23,
    SCRLINE_HOOK_COOL = 0,
    SCRLINE_HOOK_GOOD = 1,
    SCRLINE_HOOK_BAD1 = 2,
    SCRLINE_HOOK_BAD2 = 3,
    SCRLINE_HOOK_BAD3 = 4,
    SCRLINE_HOOK_BAD4 = 5,
    SCRLINE_HOOK_toCOOL = 6,
    SCRLINE_HOOK_fromCOOL = 7,
    SCRLINE_HOOK_GAMEOVER = 8,
    SCRLINE_HOOK_MAX = 9,
    SCRLINE_SER_MAIN = 0
} SCRLINE_ENUM;

typedef enum
{
	EXAM_NONE,
	EXAM_COOL,
	EXAM_GOOD,
	EXAM_BAD,
	EXAM_AWFUL,
	EXAM_HOOK,
	EXAM_VS,
	EXAM_CANCEL,
	EXAM_BONUS,
	EXAM_MAX
} EXAM_ENUM;

typedef enum
{
    EXAM_DO_NON,
    EXAM_DO_END,
    EXAM_DO_END_GO,
    EXAM_DO_END_GO_RET
} EXAM_DO_ENUM;

typedef struct { // 0xd4
    /* 0x00 */ int sndrec_num;
    /* 0x04 */ int score_lng;
    /* 0x08 */ TAPSTR tapstr[17];
    /* 0x90 */ int drawofs[17];
} SCRDAT;

typedef struct { // 0x10
    /* 0x0 */ int goto_time;
    /* 0x4 */ SCRLINE_ENUM goto_line;
    /* 0x8 */ int goto_job_time;
    /* 0xc */ SCRLINE_ENUM goto_job;
} SCR_EXAM_JOB;

typedef struct { // 0x13c
    /* 0x000 */ EXAM_ENUM exam_enum;
    /* 0x004 */ int exam_start;
    /* 0x008 */ PLAYER_CODE vsPlayer;
    /* 0x00c */ SCR_EXAM_JOB scr_exam_job[18];
    /* 0x12c */ EXAM_DO_ENUM exam_do;
    /* 0x130 */ SCR_EXAM_JOB *scr_exam_job_pp;
    /* 0x134 */ int exam_coolP;
    /* 0x138 */ int exam_point;
} SCR_EXAM_STR;

typedef enum
{
    GUI_CUR_NONE,
    GUI_CUR_PARAPPA,
    GUI_CUR_BOXY,
    GUI_CUR_ST1,
    GUI_CUR_ST2,
    GUI_CUR_ST3,
    GUI_CUR_ST4,
    GUI_CUR_ST5,
    GUI_CUR_ST6,
    GUI_CUR_ST7,
    GUI_CUR_ST8,
    GUI_CUR_MAX
} GUI_CURSOR_ENUM;

typedef enum
{
    TAP_FOLLOW_NONE,
    TAP_FOLLOW_SAVE,
    TAP_FOLLOW_LOAD,
    TAP_FOLLOW_MAX
} TAP_FOLLOW_ENUM;

typedef enum {
    CK_TH_NOCK = -1,
    CK_TH_NORMAL = 0,
    CK_TH_LATE = 1,
    CK_TH_HANE = 2,
    CK_TH_MAX = 3
} CK_TH_ENUM;

typedef enum {
    DLVL_COOL = 0,
    DLVL_GOOD = 1,
    DLVL_BAD = 2,
    DLVL_AWFUL = 3,
    DLVL_MAX = 4,
    DLVL_HK_COOL = 0,
    DLVL_HK_GOOD = 1,
    DLVL_HK_BAD1 = 2,
    DLVL_HK_BAD2 = 3,
    DLVL_HK_BAD3 = 4,
    DLVL_HK_MAX = 5
} DISP_LEVEL;

typedef struct { // 0x8
	/* 0x0 */ int time;
	/* 0x4 */ short int KeyIndex;
} COMMAKE_STR;

typedef struct { // 0x18
	/* 0x00 */ int bank[2];
	/* 0x08 */ SNDREC *sndrec_pp[2];
	/* 0x10 */ void *data_top;
	/* 0x14 */ u_int next_index;
} SCR_SND_DBUFF;

typedef struct { // 0x14
	/* 0x00 */ STDAT_DAT *stdat_dat_pp;
	/* 0x04 */ void *int_top;
	/* 0x08 */ int ready_flag;
	/* 0x0c */ int go_loop_flag;
	/* 0x10 */ int mbar_flag;
} SCORE_STR;

typedef struct { // 0xed4
    /* 0x000 */ int status;
    /* 0x004 */ PLAYER_CODE plycode;
    /* 0x008 */ SCR_CTRL *top_scr_ctrlpp;
    /* 0x00c */ SCRREC *current_scrrec_pp;
    /* 0x010 */ GLOBAL_PLY *global_ply;
    /* 0x014 */ int keyCnt[7];
    /* 0x030 */ int keyCntCom;
    /* 0x034 */ int scr_tap_vib_on;
    /* 0x038 */ int scr_tap_memory_cnt;
    /* 0x03c */ SCR_TAP_MEMORY scr_tap_memory[256];
    /* 0xc3c */ int sndId;
    /* 0xc40 */ int current_time;
    /* 0xc44 */ SCRDAT *scrdat_pp;
    /* 0xc48 */ int tapset_pos;
    /* 0xc4c */ SCR_EXAM_STR scr_exam_str;
    /* 0xd88 */ int sjob[24];
    /* 0xde8 */ int sjob_data[24][2];
    /* 0xea8 */ int wakeUpTime;
    /* 0xeac */ int wakeUpGoTime;
    /* 0xeb0 */ int useLine;
    /* 0xeb4 */ int wakeUpWaitLine;
    /* 0xeb8 */ int retStartLine;
    /* 0xebc */ int refStartTime;
    /* 0xec0 */ int refTartegLine;
    /* 0xec4 */ int refTargetTime;
    /* 0xec8 */ GUI_CURSOR_ENUM cursor_num;
    /* 0xecc */ TAP_FOLLOW_ENUM tap_follow_enum;
    /* 0xed0 */ int cansel_flag;
} SCORE_INDV_STR;

typedef enum {
    BNGAPE_NONE = -1,
    BNGAPE_A = 0,
    BNGAPE_O = 1,
    BNGAPE_X = 2,
    BNGAPE_H = 3,
    BNGAPE_A_NG = 4,
    BNGAPE_O_NG = 5,
    BNGAPE_X_NG = 6,
    BNGAPE_H_NG = 7
} BNG_ACT_P_ENUM;


typedef enum {
    BNGAKE_NONE = -1,
    BNGAKE_1_TOP = 0,
    BNGAKE_A11 = 0,
    BNGAKE_O11 = 1,
    BNGAKE_X11 = 2,
    BNGAKE_H11 = 3,
    BNGAKE_1_TOP_2 = 4,
    BNGAKE_A12 = 4,
    BNGAKE_O12 = 5,
    BNGAKE_X12 = 6,
    BNGAKE_H12 = 7,
    BNGAKE_1_TOP_3 = 8,
    BNGAKE_A13 = 8,
    BNGAKE_O13 = 9,
    BNGAKE_X13 = 10,
    BNGAKE_H13 = 11,
    BNGAKE_2_TOP = 12,
    BNGAKE_A2 = 12,
    BNGAKE_O2 = 13,
    BNGAKE_X2 = 14,
    BNGAKE_H2 = 15,
    BNGAKE_3_TOP = 16,
    BNGAKE_A31 = 16,
    BNGAKE_O31 = 17,
    BNGAKE_X31 = 18,
    BNGAKE_H31 = 19,
    BNGAKE_3_TOP_2 = 20,
    BNGAKE_A32 = 20,
    BNGAKE_O32 = 21,
    BNGAKE_X32 = 22,
    BNGAKE_H32 = 23,
    BNGAKE_3_TOP_3 = 24,
    BNGAKE_A33 = 24,
    BNGAKE_O33 = 25,
    BNGAKE_X33 = 26,
    BNGAKE_H33 = 27,
    BNGAKE_4_TOP = 28,
    BNGAKE_A4 = 28,
    BNGAKE_O4 = 29,
    BNGAKE_X4 = 30,
    BNGAKE_H4 = 31,
    BNGAKE_5_TOP = 32,
    BNGAKE_A5 = 32,
    BNGAKE_O5 = 33,
    BNGAKE_X5 = 34,
    BNGAKE_H5 = 35
} BNG_ACT_K_ENUM;

int GetCurrentTblNumber(void);

void tapReqGroupTapClear(PLAYER_INDEX pindex);

#endif