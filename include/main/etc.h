#ifndef ETC_H
#define ETC_H

// Typedef structs that I'm too lazy to define here
// -----------------------------------------------
typedef int EVENTREC;
typedef int TAPLVL_STR;
// -----------------------------------------------

typedef enum
{
    PSTEP_SERIAL,
    PSTEP_HOOK,
    PSTEP_GAME,
    PSTEP_BONUS,
    PSTEP_VS,
    PSTEP_XTR,
    PSTEP_MAX
} PLAY_STEP;

typedef enum
{
    PCODE_NONE = 1,
    PCODE_TEACHER = 2,
    PCODE_PARA = 4,
    PCODE_BOXY = 8,
    PCODE_MOVE = 16,
    PCODE_ALL = 31
} PLAYER_CODE;

typedef enum {
    PINDEX_NONE,
    PINDEX_TEACHER,
    PINDEX_PARA,
    PINDEX_BOXY,
    PINDEX_MOVE,
    PINDEX_MAX
} PLAYER_INDEX;

typedef enum
{
    DTBL_ENUM_SERIAL_MAIN = 0,
    DTBL_ENUM_SERIAL_TOP = 0,
    DTBL_ENUM_HOOK_COOL = 0,
    DTBL_ENUM_HOOK_GOOD = 1,
    DTBL_ENUM_HOOK_BAD1 = 2,
    DTBL_ENUM_HOOK_BAD2 = 3,
    DTBL_ENUM_HOOK_BAD3 = 4,
    DTBL_ENUM_HOOK_BAD4 = 5,
    DTBL_ENUM_HOOK_toCOOL = 6,
    DTBL_ENUM_HOOK_fromCOOL = 7,
    DTBL_ENUM_HOOK_GAMEOVER = 8,
    DTBL_ENUM_HOOK_TOP = 0,
    DTBL_ENUM_GAME_COOL = 0,
    DTBL_ENUM_GAME_GOOD = 1,
    DTBL_ENUM_GAME_BAD = 2,
    DTBL_ENUM_GAME_AWFUL = 3,
    DTBL_ENUM_GAME_toCOOL = 4,
    DTBL_ENUM_GAME_fromCOOL = 5,
    DTBL_ENUM_GAME_toUP = 6,
    DTBL_ENUM_GAME_toDOWN = 7,
    DTBL_ENUM_GAME_GAMEOVER = 8,
    DTBL_ENUM_GAME_COOL2 = 9,
    DTBL_ENUM_GAME_GOOD2 = 10,
    DTBL_ENUM_GAME_BAD2 = 11,
    DTBL_ENUM_GAME_AWFUL2 = 12,
    DTBL_ENUM_GAME_toCOOL2 = 13,
    DTBL_ENUM_GAME_fromCOOL2 = 14,
    DTBL_ENUM_GAME_toUP2 = 15,
    DTBL_ENUM_GAME_toDOWN2 = 16,
    DTBL_ENUM_GAME_GAMEOVER2 = 17,
    DTBL_ENUM_GAME_COOL3 = 18,
    DTBL_ENUM_GAME_GOOD3 = 19,
    DTBL_ENUM_GAME_BAD3 = 20,
    DTBL_ENUM_GAME_AWFUL3 = 21,
    DTBL_ENUM_GAME_toCOOL3 = 22,
    DTBL_ENUM_GAME_fromCOOL3 = 23,
    DTBL_ENUM_GAME_toUP3 = 24,
    DTBL_ENUM_GAME_toDOWN3 = 25,
    DTBL_ENUM_GAME_GAMEOVER3 = 26,
    DTBL_ENUM_GAME_TOP = 1,
    DTBL_ENUM_BN_FIRST = 0,
    DTBL_ENUM_BN_JUMP = 1,
    DTBL_ENUM_BN_LOOP = 2,
    DTBL_ENUM_BN_ENDING = 3,
    DTBL_ENUM_BN_TOP = 0,
    DTBL_ENUM_VS_FIRST = 0,
    DTBL_ENUM_VS_JUMP = 1,
    DTBL_ENUM_VS_L1 = 2,
    DTBL_ENUM_VS_L2 = 3,
    DTBL_ENUM_VS_L3 = 4,
    DTBL_ENUM_VS_L4 = 5,
    DTBL_ENUM_VS_L5 = 6,
    DTBL_ENUM_VS_ABATTLE0 = 7,
    DTBL_ENUM_VS_ABATTLE1_0 = 8,
    DTBL_ENUM_VS_ABATTLE1_1 = 9,
    DTBL_ENUM_VS_ABATTLE1_2 = 10,
    DTBL_ENUM_VS_ABATTLE2_0 = 11,
    DTBL_ENUM_VS_ABATTLE2_1 = 12,
    DTBL_ENUM_VS_ABATTLE2_2 = 13,
    DTBL_ENUM_VS_AROUND0 = 14,
    DTBL_ENUM_VS_AROUND1_0 = 15,
    DTBL_ENUM_VS_AROUND1_1 = 16,
    DTBL_ENUM_VS_AROUND1_2 = 17,
    DTBL_ENUM_VS_AROUND2_0 = 18,
    DTBL_ENUM_VS_AROUND2_1 = 19,
    DTBL_ENUM_VS_AROUND2_2 = 20,
    DTBL_ENUM_VS_L1_AROUND0 = 14,
    DTBL_ENUM_VS_L1_AROUND1_0 = 15,
    DTBL_ENUM_VS_L1_AROUND1_1 = 16,
    DTBL_ENUM_VS_L1_AROUND1_2 = 17,
    DTBL_ENUM_VS_L1_AROUND2_0 = 18,
    DTBL_ENUM_VS_L1_AROUND2_1 = 19,
    DTBL_ENUM_VS_L1_AROUND2_2 = 20,
    DTBL_ENUM_VS_L2_AROUND0 = 21,
    DTBL_ENUM_VS_L2_AROUND1_0 = 22,
    DTBL_ENUM_VS_L2_AROUND1_1 = 23,
    DTBL_ENUM_VS_L2_AROUND1_2 = 24,
    DTBL_ENUM_VS_L2_AROUND2_0 = 25,
    DTBL_ENUM_VS_L2_AROUND2_1 = 26,
    DTBL_ENUM_VS_L2_AROUND2_2 = 27,
    DTBL_ENUM_VS_L3_AROUND0 = 28,
    DTBL_ENUM_VS_L3_AROUND1_0 = 29,
    DTBL_ENUM_VS_L3_AROUND1_1 = 30,
    DTBL_ENUM_VS_L3_AROUND1_2 = 31,
    DTBL_ENUM_VS_L3_AROUND2_0 = 32,
    DTBL_ENUM_VS_L3_AROUND2_1 = 33,
    DTBL_ENUM_VS_L3_AROUND2_2 = 34,
    DTBL_ENUM_VS_L4_AROUND0 = 35,
    DTBL_ENUM_VS_L4_AROUND1_0 = 36,
    DTBL_ENUM_VS_L4_AROUND1_1 = 37,
    DTBL_ENUM_VS_L4_AROUND1_2 = 38,
    DTBL_ENUM_VS_L4_AROUND2_0 = 39,
    DTBL_ENUM_VS_L4_AROUND2_1 = 40,
    DTBL_ENUM_VS_L4_AROUND2_2 = 41,
    DTBL_ENUM_VS_2_ABATTLE0 = 42,
    DTBL_ENUM_VS_2_ABATTLE1_0 = 43,
    DTBL_ENUM_VS_2_ABATTLE1_1 = 44,
    DTBL_ENUM_VS_2_ABATTLE1_2 = 45,
    DTBL_ENUM_VS_2_ABATTLE2_0 = 46,
    DTBL_ENUM_VS_2_ABATTLE2_1 = 47,
    DTBL_ENUM_VS_2_ABATTLE2_2 = 48,
    DTBL_ENUM_MAX_CNT = 49,
    DTBL_ENUM_VS_TOP = 0
} DRAW_TBL_ENUM;

typedef enum
{
    PLAY_MODE_SINGLE,
    PLAY_MODE_VS_MAN,
    PLAY_MODE_VS_COM,
    PLAY_MODE_MAX
} PLAY_MODE;

typedef enum
{
    PLAY_TYPE_NORMAL,
    PLAY_TYPE_ONE,
    PLAY_TYPE_MAX
} PLAY_TYPE;

typedef enum
{
    PLAY_TABLE_NORMAL,
    PLAY_TABLE_EASY,
    PLAY_TABLE_MAX
} PLAY_TABLE_MODE;

typedef enum
{
    FGF_VSYNC,
    FGF_CD
} TIME_GET_FLAG;

typedef enum 
{
    LM_FIX,
    LM_AUTO,
    LM_MAX
} LEVEL_MOVE_ENUM;

typedef enum
{
    LVS_1,
    LVS_2,
    LVS_3,
    LVS_4,
    LVS_MAX
} LEVEL_VS_ENUM;

typedef enum
{
    DEMOF_OFF,
    DEMOF_DEMO,
    DEMOF_REPLAY,
    DEMOF_MAX
} DEMO_FLAG_ENUM;

typedef enum
{
    TLL_NORMAL,
    TLL_LV01,
    TLL_LV02,
    TLL_LV03,
    TLL_LV04,
    TLL_LV05,
    TLL_LV06,
    TLL_LV07,
    TLL_LV08,
    TLL_LV09,
    TLL_LV10,
    TLL_LV11,
    TLL_LV12,
    TLL_LV13,
    TLL_LV14,
    TLL_LV15,
    TLL_LV16,
    TLL_MAX
} TAP_LINE_LEVEL_ENUM;

typedef enum
{
    TCT_LV00,
    TCT_LV01,
    TCT_LV02,
    TCT_LV03,
    TCT_LV04,
    TCT_LV05,
    TCT_LV06,
    TCT_LV07,
    TCT_LV08,
    TCT_LV09,
    TCT_LV10,
    TCT_LV11,
    TCT_LV12,
    TCT_LV13,
    TCT_LV14,
    TCT_LV15,
    TCT_MAX,

    TCT_START = 7
} TAP_CTRL_LEVEL_ENUM;

typedef enum
{
    TRND_R1,
    TRND_R2,
    TRND_R3,
    TRND_R4,
    TRND_MAX
} TAP_ROUND_ENUM;

typedef enum
{
    RLVL_COOL = 0,
    RLVL_COOL_GOOD = 1,
    RLVL_GOOD_COOL = 2,
    RLVL_GOOD = 3,
    RLVL_GOOD_BAD = 4,
    RLVL_BAD_GOOD = 5,
    RLVL_BAD = 6,
    RLVL_BAD_AWFUL = 7,
    RLVL_AWFUL_BAD = 8,
    RLVL_AWFUL = 9,
    RLVL_AWFUL_END = 10,
    RLVL_END0 = 11,
    RLVL_END1 = 12,
    RLVL_END2 = 13,
    RLVL_MAX = 14,
    RLVL_HK_COOL = 0,
    RLVL_HK_COOL_GOOD = 1,
    RLVL_HK_GOOD_COOL = 2,
    RLVL_HK_GOOD = 3,
    RLVL_HK_GOOD_BAD1 = 4,
    RLVL_HK_BAD1_GOOD = 5,
    RLVL_HK_BAD1 = 6,
    RLVL_HK_BAD1_BAD2 = 7,
    RLVL_HK_BAD2_BAD1 = 8,
    RLVL_HK_BAD2 = 9,
    RLVL_HK_BAD2_BAD3 = 10,
    RLVL_HK_BAD3_BAD2 = 11,
    RLVL_HK_BAD3 = 12,
    RLVL_HK_BAD3_END = 13,
    RLVL_HK_END0 = 14,
    RLVL_HK_END1 = 15,
    RLVL_HK_END2 = 16,
    RLVL_HK_MAX = 17
} RANK_LEVEL;

typedef enum
{
    PAD_1CON,
    PAD_2CON,
    PAD_NOPLAYER,
    PAD_COM,
    PAD_DEMO,
    PAD_REPLAY,
    PAD_UNUSE,
    PAD_TYPE_MAX
} PAD_TYPE;

typedef struct { // 0xb8
    /* 0x00 */ PLAYER_CODE player_code;
    /* 0x04 */ RANK_LEVEL rank_level;
    /* 0x08 */ PAD_TYPE pad_type;
    /* 0x0c */ int exam_score[3];
    /* 0x18 */ int now_score;
    /* 0x20 */ long int score;
    /* 0x28 */ long int bonus;
    /* 0x30 */ int exam_tbl_up;
    /* 0x34 */ int exam_tbl_dw;
    /* 0x38 */ int exam_tbl_updown[25];
    /* 0x9c */ int tap_lvl_tmp;
    /* 0xa0 */ u_int flags;
    /* 0xa4 */ u_int vsDraw;
    /* 0xa8 */ u_int vsWin;
    /* 0xac */ u_int vsLost;
    /* 0xb0 */ long int vsScore;
} GLOBAL_PLY;

typedef struct { // 0x338
    /* 0x000 */ PLAY_MODE play_modeL;
    /* 0x004 */ PLAY_TYPE play_typeL;
    /* 0x008 */ TAP_ROUND_ENUM roundL;
    /* 0x00c */ PLAY_TABLE_MODE play_table_modeL;
    /* 0x010 */ int play_stageL;
    /* 0x018 */ GLOBAL_PLY global_ply[4];
    /* 0x2f8 */ float tempo;
    /* 0x2fc */ TIME_GET_FLAG TimeType;
    /* 0x300 */ int currentTime;
    /* 0x304 */ int cdTime;
    /* 0x308 */ int vsyncTime;
    /* 0x30c */ int Snd_currentTime;
    /* 0x310 */ int Snd_cdTime;
    /* 0x314 */ int Snd_vsyncTime;
    /* 0x318 */ int Snd_cdSampleCnt;
    /* 0x31c */ DRAW_TBL_ENUM draw_tbl_top;
    /* 0x320 */ LEVEL_MOVE_ENUM tapLevelCtrl;
    /* 0x324 */ TAP_LINE_LEVEL_ENUM tapLevel;
    /* 0x328 */ DEMO_FLAG_ENUM demo_flagL;
    /* 0x32c */ LEVEL_VS_ENUM level_vs_enumL;
    /* 0x330 */ TAP_CTRL_LEVEL_ENUM tap_ctrl_level;
    /* 0x334 */ PLAY_STEP play_step;
} GLOBAL_DATA;

TAP_ROUND_ENUM GetHatRound(void);
PLAYER_INDEX Pcode2Pindex(PLAYER_CODE pc);

#endif