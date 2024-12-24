#include "main/etc.h"

#include "os/syssub.h"
#include "os/system.h"

#include "main/cdctrl.h"

#include "iop_mdl/tapctrl_rpc.h"

#include <prlib/prlib.h>

/* sdata */
extern PR_SCENEHANDLE usrSceneHandle;

/* data */
extern SCR_SND_AREA scr_snd_area[4];

extern u_char hkl_pknum_01[];
extern u_char hkl_pknum_02[];
extern u_char hkl_pknum_03[];
extern u_char hkl_pknum_04[];
extern u_char hkl_pknum_05[];
extern u_char hkl_pknum_06[];
extern u_char hkl_pknum_07[];
extern HKL_PKSTR hkl_pkstr[];

/* bss */
extern u_int vsync_time[51];

void GlobalInit(void)
{
    WorkClear(&game_status, sizeof(game_status));
    WorkClear(&global_data, sizeof(global_data));

    game_status.play_modeG       = PLAY_MODE_SINGLE;
    game_status.play_typeG       = PLAY_TYPE_NORMAL;
    game_status.roundG           = TRND_R1;

    game_status.play_table_modeG = PLAY_TABLE_NORMAL;
    game_status.play_stageG      = 1;
    game_status.demo_flagG       = DEMOF_OFF;

    game_status.language_type    = LANG_JAPANESE;
    game_status.vibration        = VIBRATION_ON;
    game_status.subtitle         = SUBTITLE_ON;

    game_status.level_vs_enumG   = LVS_1;
}

INCLUDE_ASM("main/etc", clearStageCheck);

void GlobalTimeInit(GLOBAL_DATA *gl_pp)
{
    gl_pp->currentTime     = 0;
    gl_pp->cdTime          = 0;
    gl_pp->vsyncTime       = 0;
    gl_pp->Snd_currentTime = 0;
    gl_pp->Snd_cdTime      = 0;
    gl_pp->Snd_vsyncTime   = 0;
}

void GlobalSetTempo(GLOBAL_DATA *gl_pp, float tempo)
{
    gl_pp->tempo = tempo;
}

void GlobalTimeJobChange(TIME_GET_FLAG tfg)
{
    global_data.TimeType = tfg;
    printf("time job change!!![%x]\n", tfg);
}

#ifndef NON_MATCHING
INCLUDE_ASM("main/etc", GlobalTimeJob);
#else
void GlobalTimeJob(void)
{
    /* Not on STABS, but makes things shorter */
    GLOBAL_DATA *gl_pp = &global_data;

    if (gl_pp->TimeType == FGF_VSYNC)
    {
        gl_pp->currentTime = TimeCallbackTimeGet();
        gl_pp->vsyncTime = gl_pp->currentTime;
        
        gl_pp->Snd_currentTime =
            ((gl_pp->currentTime * 96.0f * gl_pp->tempo + 1800.0f) / 3600.0f);

        gl_pp->Snd_vsyncTime = gl_pp->Snd_currentTime;
        gl_pp->Snd_cdSampleCnt = CdctrlSndTime2WP2sample(gl_pp->tempo, gl_pp->Snd_currentTime);
    }
    else if (gl_pp->TimeType == FGF_CD)
    {
        CdctrlWp2GetSampleTmpBuf();
        gl_pp->Snd_currentTime = CdctrlWp2GetSndTimeTmp(gl_pp->tempo);

        gl_pp->currentTime =
            ((gl_pp->Snd_currentTime * 3600.0f + gl_pp->tempo * 96.0f * 0.5f)
            / (gl_pp->tempo * 96.0f));
        gl_pp->cdTime = gl_pp->currentTime;

        gl_pp->Snd_cdTime = gl_pp->Snd_currentTime;
        gl_pp->Snd_cdSampleCnt = CdctrlWp2GetSampleTmp();
    }
}
#endif

int GlobalTimeGet(void)
{
    return global_data.currentTime;
}

int GlobalSndTimeGet(void)
{
    return global_data.Snd_currentTime;
}

int GlobalSndSampleGet(void)
{
    return global_data.Snd_cdSampleCnt;
}

TAP_ROUND_ENUM GetHatRound(void)
{
    TAP_ROUND_ENUM ret = global_data.roundL;

    if (hat_change_enum != HCNG_AUTO)
        return hat_change_enum;

    return ret;
}

int GlobalMendererUseCheck(void)
{
    int ret = 0;

    if (global_data.play_step == PSTEP_GAME || global_data.play_step == PSTEP_SERIAL)
        ret = 1;
    
    return ret;
}

INCLUDE_ASM("main/etc", GlobalLobcalCopy);
#if 0
void GlobalLobcalCopy(void)
{
    /* a1 5 */ int demo_rnd;

    if (game_status.demo_flagG == DEMOF_OFF)
    {
        global_data.play_modeL = game_status.play_modeG;
        global_data.play_typeL = game_status.play_typeG;
        global_data.roundL = game_status.roundG;
        global_data.play_stageL = game_status.play_stageG;
        global_data.demo_flagL = DEMOF_OFF;

        global_data.play_table_modeL = game_status.play_table_modeG;
        global_data.level_vs_enumL = game_status.level_vs_enumG;
        
    }
    else if (game_status.demo_flagG == DEMOF_DEMO)
    {
        #if 0
        PVar1 = clearStageCheck();
        PVar3 = P3_STAGE_5;
        if (4 < (int)PVar1)
        {
            PVar3 = PVar1;
        }
        PVar1 = P3_STAGE_8;
        if ((int)PVar3 < 9)
        {
            PVar1 = PVar3;
        }
        #endif
        demo_rnd = clearStageCheck();
        if (demo_rnd > 4)


        global_data.play_modeL = PLAY_MODE_SINGLE;
        global_data.play_typeL = game_status.play_typeG;
        global_data.roundL = game_status.roundG;
        global_data.play_stageL = randMakeMax(demo_rnd) + 1;
        global_data.demo_flagL = DEMOF_DEMO;

        global_data.play_table_modeL = PLAY_TABLE_NORMAL;
    }
    else
    {
        global_data.play_modeL = mc_rep_str.play_modeS;
        global_data.play_typeL = mc_rep_str.play_typeS;
        global_data.roundL = mc_rep_str.roundS;
        global_data.play_stageL = mc_rep_str.play_stageS;
        global_data.demo_flagL = DEMOF_REPLAY;

        global_data.play_table_modeL = mc_rep_str.play_table_modeS;
        global_data.level_vs_enumL   = mc_rep_str.level_vs_enumS;
    }

    global_data.tapLevelCtrl = LM_AUTO;
}
#endif

INCLUDE_ASM("main/etc", GlobalPlySet);

PAD_TYPE GetPcode2PadType(PLAYER_CODE player_code)
{
    int         i;
    PAD_TYPE    ret;
    GLOBAL_PLY* gl_pp = global_data.global_ply;

    ret = PAD_UNUSE;

    for (i = 0; i < PR_ARRAYSIZE(global_data.global_ply); i++)
    {
        gl_pp = &global_data.global_ply[i];

        if (gl_pp->player_code & player_code)
        {
            ret = gl_pp->pad_type;
            break;
        }
    }

    return ret;
}

static int TimeCallback(int x)
{
    int    i;
    u_int* time = vsync_time;

    for (i = 0; i < PR_ARRAYSIZE(vsync_time); i++, time++)
    {
        (*time)++;
    }

    return 1;
}

void TimeCallbackSet(void)
{
    int i;

    for (i = 0; i < PR_ARRAYSIZE(vsync_time); i++)
    {
        vsync_time[i] = 0;
    }

    sceGsSyncVCallback(TimeCallback);
}

u_int TimeCallbackTimeGetChan(int chan)
{
    if (chan >= 51)
    {
        printf("Time Callback Channel ERROR!!\n");
        return 0;
    }

    return vsync_time[chan];
}

void TimeCallbackTimeSetChan(TCBK_CHANNEL_ENUM chan, u_int time)
{
    if ((int)chan >= 51)
    {
        printf("Time Callback Channel ERROR!!\n");
        return;
    }

    vsync_time[chan] = time;
}

void TimeCallbackTimeSetChanTempo(int chan, u_int time, float tempo)
{
    if (chan >= 51)
    {
        printf("Time Callback Channel ERROR!!\n");
        return;
    }

    /* This was actually written in a single line, why? */
    vsync_time[chan] = (int)
    (
        (((time < 0) ? (time & 1 | time >> 1) : time) * 3600.0f + tempo * 96.0f * 0.5f) 
            / (tempo * 96.0f)
    );
}

u_int TimeCallbackTimeGet(void)
{
    return TimeCallbackTimeGetChan(0);
}

void TimeCallbackTimeSet(u_int time)
{
    TimeCallbackTimeSetChan(0, time);
}

INCLUDE_ASM("main/etc", Pcode2Pindex);

int GetKeyCode2Index(int code)
{
    if (code & 0x10)
        return 1;
    if (code & 0x20)
        return 2;
    if (code & 0x40)
        return 3;
    if (code & 0x80)
        return 4;

    if (code & 4)
        return 5;
    if (code & 8)
        return 6;

    if (code & 1)
        return 5;
    if (code & 2)
        return 6;

    return 0;
}

INCLUDE_ASM("main/etc", GetIndex2KeyCode);

INCLUDE_ASM("main/etc", GetIndex2PressId);

int GetKeyCode2PressId(int code)
{
    if (code & 0x10)
        return 4;
    if (code & 0x20)
        return 5;
    if (code & 0x40)
        return 6;
    if (code & 0x80)
        return 7;

    if (code & 4)
        return 8;
    if (code & 8)
        return 9;

    return -1;
}

TAP_LINE_LEVEL_ENUM ChangeTapLevel(TAP_LINE_LEVEL_ENUM now_lvl)
{
    return now_lvl;
}

void UsrPrInitScene(void)
{
    // Empty
}

void UsrPrQuitScene(void)
{
    // Empty
}

INCLUDE_ASM("main/etc", UsrPrSetScene);

void SpuBankSet(void)
{
    int i;

    for (i = 0; i < PR_ARRAYSIZEU(scr_snd_area) - 1; i++)
    {
        TapCt(0x8010 | i, scr_snd_area[i].spu_adrs, 0);
        TapCt(0x8020 | i, scr_snd_area[i].iop_size, 0);
    }
}

void SpuBankSetAll(void)
{
    int i;

    for (i = 0; i < PR_ARRAYSIZEU(scr_snd_area); i++)
    {
        TapCt(0x8010 | i, scr_snd_area[i].spu_adrs, 0);
        TapCt(0x8020 | i, scr_snd_area[i].iop_size, 0);
    }
}

void inCmnInit(int stg)
{
    WorkClear(&ingame_common_str, sizeof(ingame_common_str));
    ingame_common_str.BonusStage = stg;
}

INCLUDE_ASM("main/etc", inCmnHookMaxLineCnt);

INCLUDE_ASM("main/etc", inCmnHookMaxLinePknum);

INCLUDE_ASM("main/etc", inCmnHookSet);

INCLUDE_ASM("main/etc", inCmnHook2GameCheck);

INCLUDE_ASM("main/etc", inCmnHook2GameSave);
