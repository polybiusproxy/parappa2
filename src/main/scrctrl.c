#include "main/scrctrl.h"
#include "main/etc.h"
#include "os/system.h"

/* data */
extern u_int thnum_tbl[4];
extern TCL_CTRL tcl_ctrl[4][33];
extern SCR_SND_DBUFF scr_snd_dbuff;
extern SCORE_STR score_str;
extern TAPDAT vs_tapdat_tmp[64];

/* sdata */
/* static */ int titleStartKey;
/* static */ int fadeoutStartKey;
/* static */ int gameEndWaitLoop;
/* static */ int replayGuiOffFlag;
/* static */ int jimakuWakuOff;
int currentTblNumber;
int vs_tapdat_tmp_cnt;
int scrJimakuLine;
int scrDrawLine;
int scrMbarLine;
int scrRefLineTime;

/* sdata - static */
/* implement on ScrCtrlMainLoop */
int dbgmsg_on_off;

/* bss */
extern SCORE_INDV_STR score_indv_str[5];
/* static */ extern SNDTAP *scr_sndtap_pp[4];
extern SCR_TAP_MEMORY follow_scr_tap_memory[256];

/* sbss - static */
int follow_scr_tap_memory_cnt;
int follow_scr_tap_memory_cnt_load;
int commake_str_cnt;

/* .lit4 */
float FLT_00398F10;

int GetCurrentTblNumber(void)
{
    return currentTblNumber;
}

DISP_LEVEL RANK_LEVEL2DISP_LEVEL(RANK_LEVEL lvl)
{
    DISP_LEVEL lvl_tbl[17] =
    {
        DLVL_COOL,  DLVL_COOL,
        DLVL_GOOD,  DLVL_GOOD,  DLVL_GOOD,
        DLVL_BAD,   DLVL_BAD,   DLVL_BAD,
        DLVL_AWFUL, DLVL_AWFUL, DLVL_AWFUL,
        DLVL_MAX,   DLVL_MAX,   DLVL_MAX,
        DLVL_MAX,   DLVL_MAX,   DLVL_MAX
    };

    return lvl_tbl[lvl];
}

DISP_LEVEL RANK_LEVEL2DISP_LEVEL_HK(RANK_LEVEL lvl)
{
    DISP_LEVEL lvl_tbl[15] =
    {
        DLVL_COOL,  DLVL_COOL,
        DLVL_GOOD,  DLVL_GOOD,  DLVL_GOOD,
        DLVL_BAD,   DLVL_BAD,   DLVL_BAD,
        DLVL_AWFUL, DLVL_AWFUL, DLVL_AWFUL,
        DLVL_MAX,   DLVL_MAX,   DLVL_MAX, 5
    };

    return lvl_tbl[lvl];
}

void ScrTapDbuffCtrlInit(void *data_top, int bk0, int bk1)
{
    scr_snd_dbuff.bank[0]      = bk0;
    scr_snd_dbuff.bank[1]      = bk1;

    scr_snd_dbuff.data_top     = data_top;
    scr_snd_dbuff.next_index   = 0;

    scr_snd_dbuff.sndrec_pp[0] = NULL;
    scr_snd_dbuff.sndrec_pp[1] = NULL;
}

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDbuffSet);

void ScrTapDbuffSetSp(SNDREC *sndrec_pp, int id)
{
    u_int ret;

    if (id > -1)
    {
        scr_snd_dbuff.next_index = id;
        ScrTapDataTrans(sndrec_pp, scr_snd_dbuff.bank[id & 1 ^ 1], scr_snd_dbuff.data_top);
        scr_snd_dbuff.sndrec_pp[id & 1 ^ 1] = sndrec_pp;
    }
}

void ScrTapDbuffClear(void)
{
    scr_snd_dbuff.next_index = 0;
    scr_snd_dbuff.sndrec_pp[0] = NULL;
    scr_snd_dbuff.sndrec_pp[1] = NULL;
}

void ScrTapCtrlInit(void *data_top)
{
    ScrTapDbuffCtrlInit(data_top, 1, 2);
}

void ScrTapDataTrans(SNDREC *sndrec_pp, int bank, void *data_top)
{
    if (sndrec_pp->bd_num > -1)
    {
        TapCt(0x8030 | bank, (int)GetIntAdrsCurrent(sndrec_pp->bd_num), GetIntSizeCurrent(sndrec_pp->bd_num));
        TapCt(0x8040 | bank, (int)GetIntAdrsCurrent(sndrec_pp->hd_num), GetIntSizeCurrent(sndrec_pp->hd_num));
    }

    scr_sndtap_pp[bank] = sndrec_pp->sndtap_pp;

    TapCt(0x90, PR_CONCAT(0x3fff, 0x3fff), 0);
    TapCt(0xa0, PR_CONCAT(0x3fff, 0x3fff), 0);
}

int ScrTapDataTransCheck(void)
{
    return TapCt(0x8070, 0, 0);
}

void ScrTapReq(int id, int box, int num)
{
    int     use_chan;
    SNDTAP *tp_pp;

    if (id == -1)
        use_chan = 0;
    else
        use_chan = scr_snd_dbuff.bank[id & 1];

    tp_pp = scr_sndtap_pp[use_chan] + num;

    TapCt(0xf0 | use_chan, box, tp_pp->volume);
    TapCt(0xd0 | use_chan, box, tp_pp->prg + tp_pp->key * 256);
}

void ScrTapReqStop(int box)
{
    TapCt(0xe0, box, 0);
}


static void exam_tbl_updownInit(SCORE_INDV_STR *sindv_pp)
{
    int i;

    if (sindv_pp->global_ply != NULL)
    {
        for (i = 0; i < 25; i++)
        {
            sindv_pp->global_ply->exam_tbl_updown[i] = 0;
        }
    }
}

static void exam_tbl_updownSet(SCORE_INDV_STR *sindv_pp, int now, int sikiichi /* Threshold */, int oth)
{
    int hikaku; /* Comparison variable */

    if (sindv_pp->global_ply != NULL)
    {
        if (oth >= sikiichi / 2)
            sindv_pp->global_ply->exam_tbl_updown[0]++;
        else
            sindv_pp->global_ply->exam_tbl_updown[0]--;

        hikaku = sikiichi / 2;

        if (now > hikaku)
            sindv_pp->global_ply->exam_tbl_updown[1]++;
        if (now >= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[2]++;
        if (now <= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[3]++;
        if (now < hikaku)
            sindv_pp->global_ply->exam_tbl_updown[4]++;

        hikaku = oth;

        if (now > hikaku)
            sindv_pp->global_ply->exam_tbl_updown[5]++;
        if (now >= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[6]++;
        if (now <= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[7]++;
        if (now < hikaku)
            sindv_pp->global_ply->exam_tbl_updown[8]++;

        hikaku = 0;

        if (now > hikaku)
            sindv_pp->global_ply->exam_tbl_updown[9]++;
        if (now >= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[10]++;
        if (now <= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[11]++;
        if (now < hikaku)
            sindv_pp->global_ply->exam_tbl_updown[12]++;

        hikaku = sikiichi;

        if (now > hikaku)
            sindv_pp->global_ply->exam_tbl_updown[13]++;
        if (now >= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[14]++;
        if (now <= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[15]++;
        if (now < hikaku)
            sindv_pp->global_ply->exam_tbl_updown[16]++;

        hikaku = (sikiichi - oth) / 2 + oth;

        if (now > hikaku)
            sindv_pp->global_ply->exam_tbl_updown[17]++;
        if (now >= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[18]++;
        if (now <= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[19]++;
        if (now < hikaku)
            sindv_pp->global_ply->exam_tbl_updown[20]++;

        hikaku = oth / 2;

        if (now > hikaku)
            sindv_pp->global_ply->exam_tbl_updown[21]++;
        if (now >= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[22]++;
        if (now <= hikaku)
            sindv_pp->global_ply->exam_tbl_updown[23]++;
        if (now < hikaku)
            sindv_pp->global_ply->exam_tbl_updown[24]++;
    }
}

static int exam_tbl_updownChange(SCORE_INDV_STR *sindv_pp, TAP_CTRL_LEVEL_ENUM clv, TAP_ROUND_ENUM round, int coolf)
{
    TCL_CTRL *tcl_ctrl_pp;
    int       ret = 0;
    int       udc;

    if (sindv_pp->global_ply == NULL)
        return 0;

    if (coolf)
    {
        tcl_ctrl_pp = &tcl_ctrl[round][32];
    }
    else
    {
        if (sindv_pp->global_ply->exam_tbl_updown[0] < 0)
            tcl_ctrl_pp = &tcl_ctrl[round][clv];
        else
            tcl_ctrl_pp = &tcl_ctrl[round][clv + TCT_MAX];
    }

    udc = 0;

    if (tcl_ctrl_pp->tcl_do_enum_up != TCL_DO_NONE)
        udc = sindv_pp->global_ply->exam_tbl_updown[tcl_ctrl_pp->tcl_do_enum_up];
    if (tcl_ctrl_pp->tcl_do_enum_down != TCL_DO_NONE)
        udc -= sindv_pp->global_ply->exam_tbl_updown[tcl_ctrl_pp->tcl_do_enum_down];

    if (udc < 0)
    {
        if (tcl_ctrl_pp->min < 0)
        {
            ret = tcl_ctrl_pp->min;
        }
        else
        {
            if (udc < -tcl_ctrl_pp->min)
                ret = -tcl_ctrl_pp->min;
            else
                ret = udc;
        }
    }
    else if (udc > 0)
    {
        if (tcl_ctrl_pp->max < 0)
        {
            ret = -tcl_ctrl_pp->max;
        }
        else
        {
            if (udc > tcl_ctrl_pp->max)
                ret = tcl_ctrl_pp->max;
            else
                ret = udc;
        }
    }

    return ret;
}

void vsTapdatSetMemorySave(void)
{
    int        i;
    VSOTHSAVE  vsothsave_tmp;
    TAPDAT    *tapdat_pp;

    tapdat_pp = vs_tapdat_tmp;

    WorkClear(vsothsave_tmp, sizeof(vsothsave_tmp));

    for (i = 0; i < vs_tapdat_tmp_cnt; i++, tapdat_pp++)
    {
        int time = tapdat_pp->time / 24;

        if (time >= 32)
            printf("OTH SAVE OVER\n");
        else
            vsothsave_tmp[time] = *(u_char*)&tapdat_pp->KeyIndex;
    }

    mccReqVSOTHSAVEset(&vsothsave_tmp);
}

/* https://decomp.me/scratch/ZjsvF */
INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSetMemoryLoad);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSet);

void vsTapdatSetMoto(SCORE_INDV_STR *sindv_pp)
{
    int     i;
    TAPSET *tapset_pp;
    TAPDAT *tapdat_pp;

    vs_tapdat_tmp_cnt = 0;
    printf("reset moto!!\n");

    tapset_pp = IndvGetTapSetAdrs(sindv_pp);
    tapdat_pp = tapset_pp->tapdat_pp;

    for (i = 0; i < tapset_pp->tapdat_size; i++, tapdat_pp++)
    {
        if (tapdat_pp->KeyIndex != 0)
        {
            if (global_data.play_typeL == PLAY_TYPE_ONE)
                vs_tapdat_tmp[vs_tapdat_tmp_cnt].KeyIndex = 1;
            else
                vs_tapdat_tmp[vs_tapdat_tmp_cnt].KeyIndex = tapdat_pp->KeyIndex;

            vs_tapdat_tmp[vs_tapdat_tmp_cnt].time = tapdat_pp->time;
            vs_tapdat_tmp[vs_tapdat_tmp_cnt].tapct[0].actor = -1;
            vs_tapdat_tmp[vs_tapdat_tmp_cnt].tapct[0].sound = 0xffff;

            vs_tapdat_tmp_cnt++;
        }
    }
}

static void followTapInit(void)
{
    follow_scr_tap_memory_cnt = 0;
    follow_scr_tap_memory_cnt_load = 0;
    WorkClear(follow_scr_tap_memory, sizeof(follow_scr_tap_memory));
}

static void followTapSave(SCORE_INDV_STR *sindv_pp)
{
    int i;

    follow_scr_tap_memory_cnt = 0;
    follow_scr_tap_memory_cnt_load = 0;

    for (i = 0; i < sindv_pp->scr_tap_memory_cnt; i++)
    {
        if (sindv_pp->scr_tap_memory[i].onKey)
        {
            follow_scr_tap_memory[follow_scr_tap_memory_cnt] = sindv_pp->scr_tap_memory[i];
            follow_scr_tap_memory_cnt++;
        }
    }
}

INCLUDE_ASM(const s32, "main/scrctrl", followTapLoad);
#if 0
static SCR_TAP_MEMORY* followTapLoad(int pos, int time)
{
    if (follow_scr_tap_memory_cnt > pos)
    {
        if (follow_scr_tap_memory[pos].ofs_frame > time)
            return NULL;

        if (follow_scr_tap_memory_cnt_load == pos)
        {
            follow_scr_tap_memory_cnt_load = pos + 1;
            return &follow_scr_tap_memory[pos];
        }
    }
}
#endif

static void ScrLincChangTbl(int line)
{
    scrJimakuLine = line;
    scrDrawLine   = line;
    scrMbarLine   = line;

    DrawCtrlTblChange(GetDrawLine(line));
}

static void ScrLincChangTblRef(int line, int ck_time)
{
    // TODO(poly): Flags?
    scrDrawLine |= 0x8000;
    scrMbarLine |= 0x8000;
    scrRefLineTime = ck_time;

    DrawCtrlTblChange(GetDrawLine(line));
}

void ScrLineSafeRefMode(void)
{
    if (scrDrawLine & 0x8000)
    {
        scrDrawLine &= 0xffff7fff;
        DrawCtrlTblChange(GetDrawLine(scrDrawLine));
    }

    if (scrMbarLine & 0x8000)
    {
        scrMbarLine &= 0xffff7fff;
    }
}

int ScrDrawTimeGet(int line)
{
    if (line & 0x8000)
        return scrRefLineTime;

    return score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp[line].lineTime;
}

int ScrDrawTimeGetFrame(int line)
{
    if (line & 0x8000)
        return (scrRefLineTime * 3600.0f + GetLineTempo(line) * 96.0f * 0.5f) / (GetLineTempo(line) * 96.0f);
    
    return score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp[line].lineTimeFrame;
}

void KeyCntClear(int *key_pp)
{
    int i;

    for (i = 0; i <= 6; i++, key_pp++)
    {
        *key_pp = -1;
    }
}

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlCurrentSearch);
SCRREC* ScrCtrlCurrentSearch(/* a0 4 */ SCORE_INDV_STR *sindv_pp, /* a3 7 */ int index, /* a2 6 */ int frame);
#if 0
{
    SCRREC *scrrec_pp;

    if (index >= score_str.stdat_dat_pp->scr_pp->scr_ctrl_num)
    {
        printf("ScrCtrlCurrentSearch index[%d] is over!!\n", index);
        return NULL;
    }
    else
    {
        scrrec_pp = sindv_pp->top_scr_ctrlpp[index].scrrec_pp;

        while (1)
        {
            if (scrrec_pp->job - 7 < 2)
                break;
            if (scrrec_pp->data >= frame)
                break;
            
            scrrec_pp++;
        }

        return scrrec_pp;
    }

    printf("ScrCtrlCurrentSearch index[%d] is NULL line!!\n", index);
    return NULL;
}
#endif

void ScrCtrlIndvInit(STDAT_DAT *sdat_pp)
{
	int             i, j;
	int             dare;

	SCORE_INDV_STR *sindv_pp;
    GLOBAL_PLY     *gply_pp;

    WorkClear(score_indv_str, sizeof(score_indv_str));
    sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        dare = -1;
        gply_pp = global_data.global_ply;
        
        for (j = 0; j < 4; j++)
        {
            if (gply_pp[j].player_code == PR_BIT(i))
            {
                dare = j;
                break;
            }
        }

        if (dare >= 0)
        {
            KeyCntClear(sindv_pp->keyCnt);
            tapReqGroupTapClear(Pcode2Pindex(sindv_pp->plycode));

            sindv_pp->keyCntCom = 0;
            sindv_pp->status = 1;
            sindv_pp->plycode = PR_BIT(i);
            sindv_pp->global_ply = &gply_pp[dare];
            sindv_pp->top_scr_ctrlpp = sdat_pp->scr_pp->scr_ctrl_pp;
            
            sindv_pp->current_scrrec_pp = ScrCtrlCurrentSearch(sindv_pp, global_data.draw_tbl_top, 0);
            sindv_pp->useLine = global_data.draw_tbl_top;
            
            sindv_pp->global_ply->exam_tbl_up = 0;
            sindv_pp->global_ply->exam_tbl_dw = 0;

            exam_tbl_updownInit(sindv_pp);

            for (j = 0; j < 24; j++)
            {
                sindv_pp->sjob[j] = -1;
            }

            sindv_pp->tapset_pos = -1;
        }
    }
}

void ScrCtrlExamClear(SCR_EXAM_STR *sexam_pp)
{
    int i;

    sexam_pp->exam_enum = EXAM_NONE;
    sexam_pp->exam_start = -1;
    sexam_pp->exam_do = EXAM_DO_NON;

    for (i = 0; i < 18; i++)
    {
        sexam_pp->scr_exam_job[i].goto_time = -1;
        sexam_pp->scr_exam_job[i].goto_line = SCRLINE_NODATA;
        sexam_pp->scr_exam_job[i].goto_job_time = -1;
        sexam_pp->scr_exam_job[i].goto_job = -1;
    }
}

void ScrCtrlExamClearIndv(SCR_EXAM_STR *sexam_pp)
{
    sexam_pp->exam_do = EXAM_DO_NON;
    sexam_pp->exam_start = -1;
}

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvNextTime);
int ScrCtrlIndvNextTime(/* a0 4 */ SCORE_INDV_STR *sindv_pp, /* a1 5 */ int Ncnt);
#if 0
{
    SCRREC *cur_pp = sindv_pp->current_scrrec_pp;

    while (1)
    {
        if (cur_pp->job == 7 || cur_pp->job == 8)
            break;

        if (cur_pp->job == 0)
        {
            Ncnt--;
            if (Ncnt == 0)
                break;
        }

        cur_pp++;
    }

    return cur_pp->data;
}
#endif

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvNextReadLine);

int getLvlTblRand(TAPLVL_DAT *taplvl_dat_pp)
{
    int rand_tmp;
    int ret;
    int i;
    int check;

    ret = 0;
    rand_tmp = randMakeMax(100);
    check = 0;

    for (i = 0; i < 17; i++, ret++)
    {
        check += taplvl_dat_pp->per[i];

        if (check > rand_tmp)
            break;
    }

    return ret;
}

INCLUDE_ASM(const s32, "main/scrctrl", tapLevelChangeSub);
int tapLevelChangeSub(void);
#if 0
{
    /* a3 7 */ int add_move;
    /* a0 4 */ TAPLVL_STR *taplvl_str_pp;
    /* a0 4 */ int lvl_num;

    if (global_data.tapLevelCtrl != LM_AUTO)
        return global_data.tapLevel;
    
    if (score_str.stdat_dat_pp->taplvl_str_pp == NULL)
        return global_data.tapLevel;

    taplvl_str_pp = score_str.stdat_dat_pp->taplvl_str_pp;
    
    lvl_num = global_data.roundL;

    if ( global_data.play_table_modeL == PLAY_TABLE_EASY )
        lvl_num = global_data.roundL + TRND_MAX;

    add_move = global_data.tap_ctrl_level;
    global_data.tapLevel = getLvlTblRand(&taplvl_str_pp[lvl_num].taplvl_dat[add_move]);

    return global_data.tapLevel;
}
#endif

void tapLevelChange(SCORE_INDV_STR *sindv_pp)
{
    int add_move;
    int old_num;

    int tmp_lv;
    int tmp_hklv;

    if ((sindv_pp->global_ply->flags & GPLAY_TBLCNG_REQ) == 0)
        return;

    tmp_lv = exam_tbl_updownChange(sindv_pp, global_data.tap_ctrl_level, global_data.roundL, (RANK_LEVEL2DISP_LEVEL(sindv_pp->global_ply->rank_level) == DLVL_COOL));

    sindv_pp->global_ply->exam_tbl_up = 0;
    sindv_pp->global_ply->exam_tbl_dw = 0;

    exam_tbl_updownInit(sindv_pp);

    printf("----- LEVEL CHANGE ----\n");
    printf(" CTRL LEVEL before[%d]\n", global_data.tap_ctrl_level);

    add_move = tmp_lv + global_data.tap_ctrl_level;

    if (add_move < 0)
        add_move = TCT_LV00;

    if (add_move > 15)
        add_move = TCT_LV15;

    global_data.tap_ctrl_level = add_move;

    printf("            after [%d]\n\n", add_move);
    printf(" PLAY LEVEL before[%d]\n", global_data.tapLevel);

    old_num  = global_data.tapLevel;
    add_move = tapLevelChangeSub();

    if (global_data.demo_flagL != DEMOF_REPLAY)
    {
        if (global_data.tapLevelCtrl == LM_AUTO)
        {
            if (sindv_pp->scrdat_pp != NULL)
            {
                tmp_hklv = inCmnHook2GameCheck(sindv_pp->scrdat_pp->sndrec_num);
                if (tmp_hklv >= 0)
                {
                    add_move = tmp_hklv;
                    global_data.tapLevel = tmp_hklv;
                }
            }
        }

        /* Stage 8 specific logic */
        if (global_data.play_step == PSTEP_GAME && global_data.play_stageL == 8)
        {
            if (sindv_pp->scrdat_pp != NULL)
            {
                if ((u_int)(sindv_pp->scrdat_pp->sndrec_num - 24) < 3)
                {
                    add_move = old_num;
                    printf("st8 sp same num:%d sndLine:%d\n", old_num, sindv_pp->scrdat_pp->sndrec_num);
                }
            }
            else
            {
                add_move = old_num;
                printf("st8 sp same num:%d\n", old_num);
            }
        }

        mccReqLvlSet(add_move);
    }
    else
    {
        add_move = mccReqLvlGet();
    }

    global_data.tapLevel = add_move;

    if (old_num != add_move)
        selectIndvTapResetPlay(1);

    printf("            after [%d]\n\n", add_move);
}

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvNextRead);

void intIndvStatusSet(SCORE_INDV_STR *sindv_pp, u_int CKF, u_int STF, u_int UNF)
{
    if (sindv_pp->status & CKF)
        sindv_pp->status = (sindv_pp->status | STF) & ~UNF;
}

void allIndvNextContinue(void)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        intIndvStatusSet(sindv_pp, 128, 32, 128);
    }
}

void allIndvGoContinue(void)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        intIndvStatusSet(sindv_pp, 32, 0, 32);
    }
}

void otherIndvPause(int num)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (i != num)
            intIndvStatusSet(sindv_pp, 1, 128, 0);
    }
}

INCLUDE_ASM(const s32, "main/scrctrl", otherIndvTapReset);
#if 0 /* Matching */
void otherIndvTapReset(int num)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (i != num)
        {
            if (sindv_pp->status & 1)
            {
                KeyCntClear(sindv_pp->keyCnt);
                
                sindv_pp->keyCntCom = 0;
                sindv_pp->scr_tap_memory_cnt = 0;
                sindv_pp->scr_tap_vib_on = 0;
                sindv_pp->cansel_flag = 0;
                
                tapReqGroupTapClear(Pcode2Pindex(sindv_pp->plycode));
            }
        }
    }
}
#endif

INCLUDE_ASM(const s32, "main/scrctrl", selectIndvTapResetPlay);

INCLUDE_ASM(const s32, "main/scrctrl", IndivMoveChange);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevAllMove);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevCodeGet);

INCLUDE_ASM(const s32, "main/scrctrl", targetTimeGet);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevSndKill);

void useAllClearKeySnd(void)
{
    int i;

    for (i = 0; i < 12; i++)
    {
        TapCt(0xe0, i, 0);
    }
}

INCLUDE_ASM(const s32, "main/scrctrl", useIndevSndKillOther);

INCLUDE_ASM(const s32, "main/scrctrl", TapKeyCheckNum);

INCLUDE_ASM(const s32, "main/scrctrl", TapKeyGetDatPP);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupInit);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupTapClear);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroup);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupPoll);

INCLUDE_ASM(const s32, "main/scrctrl", tapEventCheck);

int otehon_all_make(EXAM_CHECK *ec_pp)
{
    int i;
    int ret = 0;

    if (!ec_pp->tapset_pp)
        return 0;

    for (i = 0; i < ec_pp->tapset_pp->tapdat_size; i++)
    {
        ret |= GetIndex2KeyCode(ec_pp->tapset_pp->tapdat_pp[i].KeyIndex);
    }

    return ret;
}

INCLUDE_ASM(const s32, "main/scrctrl", treateTimeChange);

int thnum_get(int p96_num, CK_TH_ENUM ckth)
{
    u_int thnum_data;
    int   ck_bit;
    int   ck_dat;
    int   ret_cnt;
    int   i;

    ck_bit = -1;
    ret_cnt = 0;
    thnum_data = thnum_tbl[ckth];

    for (i = 0; i <= p96_num; i++)
    {
        ck_dat = (thnum_data >> (23 - i % 24)) & 1;
        
        if (ck_bit < 0)
        {
            if (ck_dat == 0)
                ret_cnt = 1;
        }
        else if (ck_dat != ck_bit)
        {
            ret_cnt++;
        }
        
        ck_bit = ck_dat;
    }

    return ret_cnt;
}

INCLUDE_ASM(const s32, "main/scrctrl", MapNormalNumGet);

INCLUDE_ASM(const s32, "main/scrctrl", on_th_make);

INCLUDE_ASM(const s32, "main/scrctrl", exh_normal_add);

INCLUDE_ASM(const s32, "main/scrctrl", exh_normal_sub);

INCLUDE_ASM(const s32, "main/scrctrl", exh_nombar_sub);

INCLUDE_ASM(const s32, "main/scrctrl", exh_mbar_key_out);

INCLUDE_ASM(const s32, "main/scrctrl", exh_mbar_time_out);

INCLUDE_ASM(const s32, "main/scrctrl", exh_mbar_num_out);

INCLUDE_ASM(const s32, "main/scrctrl", exh_yaku);

INCLUDE_ASM(const s32, "main/scrctrl", exh_yaku_original);

INCLUDE_ASM(const s32, "main/scrctrl", exh_yaku_hane);

INCLUDE_ASM(const s32, "main/scrctrl", exh_allkey_out);

INCLUDE_ASM(const s32, "main/scrctrl", exh_allkey_out_nh);

INCLUDE_ASM(const s32, "main/scrctrl", exh_command);

INCLUDE_ASM(const s32, "main/scrctrl", exh_renda_out);

INCLUDE_ASM(const s32, "main/scrctrl", manemane_check_sub);

INCLUDE_ASM(const s32, "main/scrctrl", manemane_check);

INCLUDE_ASM(const s32, "main/scrctrl", exh_mane);

INCLUDE_ASM(const s32, "main/scrctrl", exh_all_add);

INCLUDE_ASM(const s32, "main/scrctrl", IndvGetTapSetAdrs);
TAPSET* IndvGetTapSetAdrs(/* a0 4 */ SCORE_INDV_STR *sindv_pp);

INCLUDE_ASM(const s32, "main/scrctrl", nextExamTime);

INCLUDE_ASM(const s32, "main/scrctrl", GetSindvPcodeLine);

INCLUDE_ASM(const s32, "main/scrctrl", ExamScoreCheck);

INCLUDE_ASM(const s32, "main/scrctrl", ExamScoreCheckSame);

INCLUDE_ASM(const s32, "main/scrctrl", levelChangeCheck);

INCLUDE_ASM(const s32, "main/scrctrl", levelUpRank);

INCLUDE_ASM(const s32, "main/scrctrl", levelDownRank);

INCLUDE_RODATA(const s32, "main/scrctrl", D_00392D30);

INCLUDE_RODATA(const s32, "main/scrctrl", D_00392D40);

INCLUDE_RODATA(const s32, "main/scrctrl", D_00392D88);

INCLUDE_ASM(const s32, "main/scrctrl", ScrMoveSetSub);

/* Big function! Decompiler discretion advised */
/*      https://decomp.me/scratch/woLno        */
INCLUDE_ASM(const s32, "main/scrctrl", ScrExamSetCheck);

INCLUDE_ASM(const s32, "main/scrctrl", subjobEvent);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvJob);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTimeRenew);

INCLUDE_ASM(const s32, "main/scrctrl", ScrMbarReq);

INCLUDE_ASM(const s32, "main/scrctrl", allTimeCallbackTimeSetChanTempo);

INCLUDE_ASM(const s32, "main/scrctrl", SetIndvDrawTblLine);

INCLUDE_ASM(const s32, "main/scrctrl", otehonSetCheck);

// INCLUDE_RODATA(const s32, "main/scrctrl", D_00392F70);
INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlMainLoop);

GET_TIME_TYPE GetTimeType(int scr_line)
{
    return score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp[scr_line].gtime_type;
}

int GetTimeOfset(int scr_line)
{
    return score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp[scr_line].ofsCdtime;
}

int GetSubtLine(int scr_line)
{
    return score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp[scr_line].subtLine;
}

int GetDrawLine(int scr_line)
{
    return score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp[scr_line].drawLine;
}

float GetLineTempo(int scr_line)
{
    return score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp[scr_line].tempo;
}

void SetLineChannel(int scr_line)
{
    SCR_CTRL *scr_ctrl_pp = score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp + scr_line;

    if (scr_ctrl_pp->gtime_type != GTIME_VSYNC)
    {
        CdctrlWP2SetChannel(scr_ctrl_pp->cdChan[0], scr_ctrl_pp->cdChan[1]);
    }
}

INCLUDE_ASM(const s32, "main/scrctrl", SetIndvCdChannel);

INCLUDE_ASM(const s32, "main/scrctrl", CheckIndvCdChannel);

// INCLUDE_RODATA(const s32, "main/scrctrl", dbg_tbl_msg);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlInit);

void ScrCtrlQuit(void)
{
    useAllClearKeySnd();
    TapCt(0x120, 0, 0);

    MtcKill(5);
}

int ScrCtrlInitCheck(void)
{
    return score_str.ready_flag;
}

void ScrCtrlGoLoop(void)
{
    score_str.go_loop_flag = 1;
}

INCLUDE_ASM(const s32, "main/scrctrl", ScrEndCheckScore);

int ScrEndCheckTitle(void)
{
    return titleStartKey;
}

int ScrEndCheckFadeOut(void)
{
    return fadeoutStartKey;
}

int ScrEndWaitLoop(void)
{
    return gameEndWaitLoop;
}

INCLUDE_ASM(const s32, "main/scrctrl", bonusGameInit);

INCLUDE_ASM(const s32, "main/scrctrl", bonusGameCntPls);

INCLUDE_ASM(const s32, "main/scrctrl", bonusPointSave);

INCLUDE_ASM(const s32, "main/scrctrl", bngTapEventCheck);
void bngTapEventCheck(/* s1 17 */ SCORE_INDV_STR *sindv_pp, /* t0 8 */ int num, /* s3 19 */ int id);

void bonusGameParaReq(BNG_ACT_P_ENUM actnum)
{
    bngTapEventCheck(score_indv_str + 2, actnum, 0);
}

static void bonusGameKoamaReq(int kotamaNum, BNG_ACT_K_ENUM actnum)
{
    bngTapEventCheck(score_indv_str + 1, actnum + kotamaNum, kotamaNum + 1);
}

static int bonus_minus_point_sub(int wtime)
{
    if (wtime < 8)
        return 9;

    if (wtime < 13)
        return 13;

    if (wtime < 20)
        return 18;

    return 24;
}

static int bonus_pls_point_sub(int wtime)
{
    if (wtime < 3)
        return 26;

    if (wtime < 6)
        return 15;

    if (wtime < 9)
        return 9;

    return 5;
}

INCLUDE_ASM(const s32, "main/scrctrl", bonusGameCtrl);

static u_long hex2dec(u_long data)
{
    u_long ret = 0;
    u_int  i;

    for (i = 0; data != 0;)
    {
        ret |= (data % 10) << (i * 4);
        data /= 10;

        i++;
        
        if (i >= 16)
            return ret;
    }

    return ret;
}

INCLUDE_ASM(const s32, "main/scrctrl", bnNumberDisp);

INCLUDE_ASM(const s32, "main/scrctrl", bonusScoreDraw);

INCLUDE_ASM(const s32, "main/scrctrl", set_lero_gifset);

INCLUDE_ASM(const s32, "main/scrctrl", LessonRoundDisp);
