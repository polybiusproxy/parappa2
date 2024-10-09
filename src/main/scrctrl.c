#include "main/scrctrl.h"

#include "main/mbar.h"

/* data */
extern u_int thnum_tbl[4];
extern TCL_CTRL tcl_ctrl[4][33];
extern LERO_TIM2_PT lero_tim2_pt[7];
extern LERO_POS_STR lero_pos_str[10][2];
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

/* bss - static */
/* not static */ extern SCORE_INDV_STR score_indv_str[5];
extern SNDTAP *scr_sndtap_pp[4];
extern SCR_TAP_MEMORY follow_scr_tap_memory[256];
extern TAP_GROUPE_STR tap_groupe_str[5];

/* sbss - static */
int follow_scr_tap_memory_cnt;
int follow_scr_tap_memory_cnt_load;
int commake_str_cnt;

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

INCLUDE_ASM("main/scrctrl", ScrTapDbuffSet);

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
INCLUDE_ASM("main/scrctrl", vsTapdatSetMemoryLoad);

INCLUDE_ASM("main/scrctrl", vsTapdatSet);

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
            vs_tapdat_tmp[vs_tapdat_tmp_cnt].tapct[0].sound = -1;

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

INCLUDE_ASM("main/scrctrl", followTapLoad);
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

INCLUDE_ASM("main/scrctrl", ScrCtrlCurrentSearch);
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
            sindv_pp->status = SCS_USE;
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

    sexam_pp->exam_enum  = EXAM_NONE;
    sexam_pp->exam_start = -1;
    sexam_pp->exam_do    = EXAM_DO_NON;

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

INCLUDE_ASM("main/scrctrl", ScrCtrlIndvNextTime);
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

INCLUDE_ASM("main/scrctrl", ScrCtrlIndvNextReadLine);
int ScrCtrlIndvNextReadLine(/* t0 8 */ SCORE_INDV_STR *sindv_pp, /* t2 10 */ int ckf);

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

INCLUDE_ASM("main/scrctrl", tapLevelChangeSub);
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

INCLUDE_ASM("main/scrctrl", ScrCtrlIndvNextRead);
#if 0
void ScrCtrlIndvNextRead(/* s0 16 */ SCORE_INDV_STR *sindv_pp, /* a1 5 */ int tap_res_f)
{
    int i;
    /* s2 18 */ int endf;
    /* v1 3 */ int j;
    /* a3 7 */ short Rsub;
    /* t0 8 */ int Rdata;
    /* t1 9 */ int sj_data1;
    /* t2 10 */ int sj_data2;
    /* a0 4 */ int xx;
    /* t4 12 */ int yari_ff;

    SCRREC *scrrec_pp;

    sindv_pp->keyCntCom = 0;
    sindv_pp->scr_tap_memory_cnt = 0;
    sindv_pp->scr_tap_vib_on = 0;
    sindv_pp->cansel_flag = 0;

    if (tap_res_f)
    {
        KeyCntClear(sindv_pp->keyCnt);
        tapReqGroupTapClear(Pcode2Pindex(sindv_pp->plycode));
    }

    ScrCtrlExamClearIndv(&sindv_pp->scr_exam_str);

    if ((sindv_pp->status & 1) == 0)
    {
        sindv_pp->scrdat_pp = NULL;
        sindv_pp->sndId = -1;

        for (i = 0; i < 24; i++)
        {
            sindv_pp->sjob[i] = -1;
        }

        ScrCtrlExamClear(&sindv_pp->scr_exam_str);
        return;
    }

    if (sindv_pp->tapset_pos == -1 || ScrCtrlIndvNextReadLine(sindv_pp, 0))
    {
        ScrCtrlExamClear(&sindv_pp->scr_exam_str);

        scrrec_pp = sindv_pp->current_scrrec_pp;
        sindv_pp->scrdat_pp = NULL;
        sindv_pp->sndId = -1;

        for (i = 0; i < 24; i++)
        {
            sindv_pp->sjob[i] = -1;
        }

        while (1)
        {
            if (scrrec_pp->job == 7)
                break;

            if (scrrec_pp->job != 0)
            {
                if (scrrec_pp->sub & sindv_pp->plycode)
                {
                    sindv_pp->current_time = scrrec_pp->data;
                    sindv_pp->cursor_num = scrrec_pp->jobd1;
                    sindv_pp->tap_follow_enum = TAP_FOLLOW_NONE;

                    endf = 0;
                    
                    scrrec_pp++;

                    while (1)
                    {
                        Rsub = scrrec_pp->sub;
                        Rdata = scrrec_pp->data;
                        sj_data1 = scrrec_pp->jobd1;
                        sj_data2 = scrrec_pp->jobd2;

                        switch (scrrec_pp->job)
                        {
                        case 0:
                        case 7:
                        case 8:
                            endf = 1;
                            break;
                        case 2:
                            sindv_pp->scr_exam_str.vsPlayer = Rdata;
                            sindv_pp->scr_exam_str.exam_enum = Rsub;
                            sindv_pp->scr_exam_str.exam_start = 0;
                            break;
                        case 3:
                            sindv_pp->scr_exam_str.scr_exam_job[0].goto_time = Rdata;
                            sindv_pp->scr_exam_str.scr_exam_job[0].goto_line = Rsub;
                            break;
                        case 4:
                            sindv_pp->scr_exam_str.scr_exam_job[0].goto_job_time = Rdata;
                            sindv_pp->scr_exam_str.scr_exam_job[0].goto_job = Rsub;
                            break;
                        case 5:
                            sindv_pp->scr_exam_str.scr_exam_job[1].goto_time = Rdata;
                            sindv_pp->scr_exam_str.scr_exam_job[1].goto_line = Rsub;
                            break;
                        case 6:
                            sindv_pp->scr_exam_str.scr_exam_job[1].goto_job_time = Rdata;
                            sindv_pp->scr_exam_str.scr_exam_job[1].goto_job = Rsub;
                            break;
                        case 9:
                            if (Rsub == 9)
                            {
                                yari_ff = 0;

                                for (j = 0; j < 4; i++)
                                {
                                    if (sindv_pp->sjob[j] != -1)
                                    {
                                        sindv_pp->sjob_data[j][0] = sj_data1;
                                        sindv_pp->sjob_data[j][1] = sj_data2;
                                        yari_ff = 1;
                                    }
                                }

                                if (!yari_ff)
                                    printf("ERROR! SUBJOB_NO_ID OVER!\n");
                            }
                            else
                            {
                                sindv_pp->sjob[Rsub] = Rdata;
                                sindv_pp->sjob_data[Rsub][0] = sj_data1;
                                sindv_pp->sjob_data[Rsub][1] = sj_data2;
                            }
                            break;
                        case 0xB:
                            sindv_pp->scr_exam_str.scr_exam_job[2].goto_line = sj_data1;
                            sindv_pp->scr_exam_str.scr_exam_job[2].goto_job_time = Rdata;
                            sindv_pp->scr_exam_str.scr_exam_job[2].goto_job = Rsub;
                            break;
                        case 0xC:
                            sindv_pp->scr_exam_str.scr_exam_job[3].goto_line = sj_data1;
                            sindv_pp->scr_exam_str.scr_exam_job[3].goto_job_time = Rdata;
                            sindv_pp->scr_exam_str.scr_exam_job[3].goto_job = Rsub;
                            break;
                        case 0xD:
                            sindv_pp->scr_exam_str.scr_exam_job[sj_data2].goto_line = Rsub;
                            sindv_pp->scr_exam_str.scr_exam_job[sj_data2].goto_time = Rdata;
                            break;
                        case 0xE:
                            sindv_pp->scr_exam_str.scr_exam_job[sj_data2].goto_job = Rsub;
                            sindv_pp->scr_exam_str.scr_exam_job[sj_data2].goto_job_time = Rdata;
                            break;
                        default:
                            break;
                        }

                        if (endf)
                        {
                            if (sindv_pp->global_ply == NULL)
                                printf("global_play is NULL [%d]\n", sindv_pp->plycode);
                            else
                                tapLevelChange(sindv_pp);

                            ScrCtrlIndvNextReadLine(sindv_pp, 0);
                            return;
                        }

                        scrrec_pp++;
                    }
                }

                scrrec_pp++;
            }
        }

        sindv_pp->status |= 8;
        sindv_pp->current_time = scrrec_pp->data;
    }
}
#endif

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
        intIndvStatusSet(sindv_pp, SCS_PAUSE, SCS_PAUSE_END, SCS_PAUSE);
    }
}

void allIndvGoContinue(void)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        intIndvStatusSet(sindv_pp, SCS_PAUSE_END, 0, SCS_PAUSE_END);
    }
}

void otherIndvPause(int num)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (i != num)
            intIndvStatusSet(sindv_pp, SCS_USE, SCS_PAUSE, 0);
    }
}

void otherIndvTapReset(int num)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (i != num)
        {
            if (sindv_pp->status & SCS_USE)
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

void selectIndvTapResetPlay(int num)
{
    SCORE_INDV_STR *sindv_pp = &score_indv_str[num];
    TAPSET         *tapset_pp;

    if (sindv_pp->status & SCS_USE)
    {
        tapset_pp = IndvGetTapSetAdrs(sindv_pp);
        if (tapset_pp != NULL && tapset_pp->coolup != -1)
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

void IndivMoveChange(SCORE_INDV_STR *sindv_pp, int goto_time, SCRLINE_ENUM goto_line)
{
    int j;

    KeyCntClear(sindv_pp->keyCnt);
    tapReqGroupTapClear(Pcode2Pindex(sindv_pp->plycode));

    sindv_pp->top_scr_ctrlpp = score_str.stdat_dat_pp->scr_pp->scr_ctrl_pp;
    sindv_pp->current_scrrec_pp = ScrCtrlCurrentSearch(sindv_pp, goto_line, goto_time);

    for (j = 0; j < 24; j++)
    {
        sindv_pp->sjob[j] = -1;
    }

    sindv_pp->useLine = goto_line;
    sindv_pp->tapset_pos = -1;

    ScrCtrlIndvNextRead(sindv_pp, 1);
}

void useIndevAllMove(int goto_time, SCRLINE_ENUM goto_line)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (sindv_pp->status & SCS_USE)
        {
            sindv_pp->plycode = PR_BIT(i);
            sindv_pp->status  = SCS_USE;

            IndivMoveChange(sindv_pp, goto_time, goto_line);
        }
    }
}

static int useIndevCodeGet(void)
{
    int             i;
    int             ret      = 0;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (sindv_pp->status & SCS_USE)
            ret |= PR_BIT(i);
    }

    return ret;
}

INCLUDE_ASM("main/scrctrl", targetTimeGet);
int targetTimeGet(/* a0 4 */ int line, /* a1 5 */ int time, /* a2 6 */ int codeAll);

void useIndevSndKill(void)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (sindv_pp->status & SCS_USE)
            TapCt(0xe0, i, 0);
    }
}

void useAllClearKeySnd(void)
{
    int i;

    for (i = 0; i < 12; i++)
    {
        TapCt(0xe0, i, 0);
    }
}

int useIndevSndKillOther(int num)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (i != num)
        {
            if (sindv_pp->status & SCS_USE)
                TapCt(0xe0, i, 0);
        }
    }

    return 0;
}

int TapKeyCheckNum(TAPSET *tapset_pp, int keyId, int ng_f)
{
    int     i;
    int     ret;

    int     tmp_size;
    TAPDAT *tmp_tapdat_pp;

    ret = 0;

    if (ng_f)
    {
        tmp_size      = tapset_pp->tapdatNG_size;
        tmp_tapdat_pp = tapset_pp->tapdatNG_pp;
    }
    else
    {
        tmp_size      = tapset_pp->tapdat_size;
        tmp_tapdat_pp = tapset_pp->tapdat_pp;
    }

    for (i = 0; i < tmp_size; i++)
    {
        if (tmp_tapdat_pp[i].KeyIndex == keyId)
            ret++;
    }

    return ret;
}

TAPDAT* TapKeyGetDatPP(TAPSET *tapset_pp, int keyId, int keyCnt, int ng_f, u_char *keyNumSave)
{
    int     i;
    int     cnt;
    int     tmp_size;
    TAPDAT *tmp_tapdat_pp;

    cnt = 0;
    *keyNumSave = 0;

    if (ng_f)
    {
        tmp_size      = tapset_pp->tapdatNG_size;
        tmp_tapdat_pp = tapset_pp->tapdatNG_pp;
    }
    else
    {
        tmp_size      = tapset_pp->tapdat_size;
        tmp_tapdat_pp = tapset_pp->tapdat_pp;
    }

    for (i = 0; i < tmp_size; i++)
    {
        if (tmp_tapdat_pp[i].KeyIndex == keyId)
        {
            if (cnt == keyCnt)
            {
                *keyNumSave = i;
                return &tmp_tapdat_pp[i];
            }

            cnt++;
        }
    }

    return NULL;
}

void tapReqGroupInit(void)
{
    WorkClear(tap_groupe_str, sizeof(tap_groupe_str));
}

void tapReqGroupTapClear(PLAYER_INDEX pindex)
{
    WorkClear(&tap_groupe_str[pindex], sizeof(TAP_GROUPE_STR));
}

void tapReqGroup(TAPCT *tapct_pp, PLAYER_INDEX pindex, int sndId, u_char *tappress_pp)
{
    if (pindex >= PINDEX_MAX)
        printf("tap group PINDEX over[%d]\n", pindex);

    tap_groupe_str[pindex].timer       = 0;
    tap_groupe_str[pindex].sndId       = sndId;
    tap_groupe_str[pindex].tapct_pp    = tapct_pp;
    tap_groupe_str[pindex].tappress_pp = tappress_pp;
}

INCLUDE_ASM("main/scrctrl", tapReqGroupPoll);
#if 0
void tapReqGroupPoll(void)
{
    /* s4 20 */ int i;
    /* s3 19 */ int j;
    /* s5 21 */ int end_frameT;
    /* s1 17 */ TAP_GROUPE_STR *tgs_pp = tap_groupe_str;

    for (i = 0; i < 5; i++, tgs_pp++)
    {
        end_frameT = 0;

        if (tgs_pp->tapct_pp != NULL)
        {
            for (j = 0; i < 5; j++)
            {
                if (tgs_pp->tapct_pp[j].frame != -1)
                {
                    if (tgs_pp->tapct_pp[j].frame == tgs_pp->timer)
                    {
                        if (tgs_pp->tapct_pp[j].actor != -1)
                            DrawTapReqTbl(tgs_pp->tapct_pp[j].actor, i, tgs_pp->tappress_pp);
                        if (tgs_pp->tapct_pp[j].sound != -1)
                            ScrTapReq(tgs_pp->sndId, i, tgs_pp->tapct_pp[j].sound);
                    }
                }
                else if (tgs_pp->tapct_pp[j].frame > tgs_pp->timer)
                {
                    end_frameT = 1;
                }
            }

            if (end_frameT)
                tgs_pp->timer++;
            else
                WorkClear(tgs_pp, sizeof(TAP_GROUPE_STR));
        }
    }
}
#endif

INCLUDE_ASM("main/scrctrl", tapEventCheck);

static int otehon_all_make(EXAM_CHECK *ec_pp)
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

static int thnum_get(int p96_num, CK_TH_ENUM ckth);

INCLUDE_ASM("main/scrctrl", treateTimeChange);
#if 0
static int treateTimeChange(/* s0 16 */ int time)
{
    /* s1 17 */ int thnum_ofs;
    /* a0 4 */ int thnum_now;
    int ret;

    thnum_ofs = thnum_get(24, CK_TH_NORMAL);
    thnum_now = thnum_get((time + 96) / 4, CK_TH_NORMAL);

    if (thnum_now & 1)
        return -1;

    if (thnum_now >= thnum_ofs)
        return ((thnum_now - thnum_ofs) / 2) * 24;
    
    return -1;
}
#endif

static int thnum_get(int p96_num, CK_TH_ENUM ckth)
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

static int MapNormalNumGet(int time)
{
    return thnum_get(time / 4, CK_TH_NORMAL);
}

INCLUDE_ASM("main/scrctrl", on_th_make);

/* TODO(poly): Make these static once .data is migrated */
/* static */ int exh_normal_add(EXAM_CHECK *ec_pp)
{
    int i;
    int ret = 0;

    for (i = 0; i < ec_pp->ted_num; i++)
    {
        if ((ec_pp->ted[i].th_num & 1) == 0)
        {
            if (GetIndex2KeyCode(ec_pp->ted[i].key) & ec_pp->otehon_all)
                ret++;
        }
    }

    return ret;
}

/* static */ int exh_normal_sub(EXAM_CHECK *ec_pp)
{
    int i;
    int ret = 0;

    for (i = 0; i < ec_pp->ted_num; i++)
    {
        if (GetIndex2KeyCode(ec_pp->ted[i].key) & ec_pp->otehon_all)
            ret -= ec_pp->ted[i].th_num & 1;
    }

    return ret;
}

INCLUDE_ASM("main/scrctrl", exh_nombar_sub);
#if 0
static int exh_nombar_sub(/* s2 18 */ EXAM_CHECK *ec_pp)
{
    /* s1 17 */ int i;
    /* s0 16 */ int ret = 0;
    /* a0 4 */  int bai;

    for (i = 0; i < ec_pp->ted_num; i++)
    {
        if ((GetIndex2KeyCode(ec_pp->ted[i].key) & ec_pp->otehon_all) == 0)
            ret--;
    }

    bai = 0;

    for (i = 0; i < 4U; i++)
    {
        bai += (ec_pp->otehon_all >> i) & 1;
    }

    if (bai == 1)
        ret *= 3;
    if (bai == 2)
        ret *= 2;
    if (bai == 5)
        ret /= 2;
    if (bai == 6)
        ret /= 2;

    return ret;
}
#endif

INCLUDE_ASM("main/scrctrl", exh_mbar_key_out);

INCLUDE_ASM("main/scrctrl", exh_mbar_time_out);

INCLUDE_ASM("main/scrctrl", exh_mbar_num_out);

INCLUDE_ASM("main/scrctrl", exh_yaku);

INCLUDE_ASM("main/scrctrl", exh_yaku_original);

INCLUDE_ASM("main/scrctrl", exh_yaku_hane);

INCLUDE_ASM("main/scrctrl", exh_allkey_out);

INCLUDE_ASM("main/scrctrl", exh_allkey_out_nh);

INCLUDE_ASM("main/scrctrl", exh_command);

INCLUDE_ASM("main/scrctrl", exh_renda_out);

INCLUDE_ASM("main/scrctrl", manemane_check_sub);

INCLUDE_ASM("main/scrctrl", manemane_check);

INCLUDE_ASM("main/scrctrl", exh_mane);

INCLUDE_ASM("main/scrctrl", exh_all_add);

INCLUDE_ASM("main/scrctrl", IndvGetTapSetAdrs);
TAPSET* IndvGetTapSetAdrs(/* a0 4 */ SCORE_INDV_STR *sindv_pp);

INCLUDE_ASM("main/scrctrl", nextExamTime);

INCLUDE_ASM("main/scrctrl", GetSindvPcodeLine);
SCORE_INDV_STR* GetSindvPcodeLine(/* a0 4 */ PLAYER_CODE pcode);

INCLUDE_ASM("main/scrctrl", ExamScoreCheck);

INCLUDE_ASM("main/scrctrl", ExamScoreCheckSame);

INCLUDE_ASM("main/scrctrl", levelChangeCheck);

INCLUDE_ASM("main/scrctrl", levelUpRank);

INCLUDE_ASM("main/scrctrl", levelDownRank);

INCLUDE_RODATA("main/scrctrl", D_00392D30);

INCLUDE_RODATA("main/scrctrl", D_00392D40);

INCLUDE_RODATA("main/scrctrl", D_00392D88);

INCLUDE_ASM("main/scrctrl", ScrMoveSetSub);
#if 0
void ScrMoveSetSub(SCORE_INDV_STR *sindv_pp, /* s0 16 */ int Pnum, /* s4 20 */ int sub_job, /* s1 17 */ int sub_time, /* s6 22 */ int goto_job, /* s7 23 */ int goto_time, /* fp 30 */ int start_move_line, /* s3 19 */ int start_move_time)
{
    /* s1 17 */ int ttype;
    /* s0 16 */ int tmp_cdsample;
    /* s2 18 */ int target_move_time;
    /* s0 16 */ SCORE_INDV_STR *sub_in_pp;
    /* -0xb0(sp) */ u_char chantmp[2];

    target_move_time = targetTimeGet(goto_job, goto_time, useIndevCodeGet());

    useIndevAllMove(goto_time, goto_job);
    useIndevSndKill();

    sindv_pp->wakeUpTime = sub_time;
    sindv_pp->wakeUpGoTime = goto_time;
    sindv_pp->wakeUpWaitLine = sub_job;
    sindv_pp->status |= SCS_WAIT;
    otherIndvPause(Pnum);

    sub_in_pp = score_indv_str + 4;
    sub_in_pp->status = 1;
    sub_in_pp->plycode = PCODE_MOVE;
    sub_in_pp->global_ply = NULL;

    IndivMoveChange(sub_in_pp, 0, sub_job);

    sub_in_pp->retStartLine = start_move_line;
    sub_in_pp->refStartTime = start_move_time;
    sub_in_pp->refTartegLine = goto_job;
    sub_in_pp->refTargetTime = target_move_time;

    if (GetTimeType(sub_job) != GTIME_VSYNC)
        GlobalTimeJobChange(FGF_CD);
    else
        GlobalTimeJobChange(FGF_VSYNC);

    TimeCallbackTimeSetChanTempo(sub_job, 0, GetLineTempo(sub_job));

    sub_in_pp->top_scr_ctrlpp[sub_job].lineTime = 0;
    sub_in_pp->top_scr_ctrlpp[sub_job].lineTimeFrame = 0;

    ScrLincChangTbl(sub_job);
    tapEventCheck(sub_in_pp, 0, 0, 4);

    ttype = GetTimeType(sub_job);
    if (ttype == GTIME_VSYNC)
    {
        if (GetTimeType(start_move_line) != GTIME_VSYNC)
            CdctrlWp2Stop();
    }
    else
    {
        printf("file seek\n");

        tmp_cdsample = CdctrlSndTime2WP2sample(GetLineTempo(sub_job), goto_time);
        tmp_cdsample -= (GetTimeOfset(goto_job) * 48) / 256;

        CheckIndvCdChannel(sindv_pp, chantmp);
        CdctrlWP2SetFileSeekChan(&score_str.stdat_dat_pp->sndfile[ttype], tmp_cdsample, chantmp[0], chantmp[1]);
    }
}
#endif

/* Big function! Decompiler discretion advised */
/*      https://decomp.me/scratch/woLno        */
INCLUDE_ASM("main/scrctrl", ScrExamSetCheck);

INCLUDE_ASM("main/scrctrl", subjobEvent);
#if 0
void subjobEvent(/* s1 17 */ SCORE_INDV_STR *sindv_pp, /* s6 22 */ int ctime_next)
{
    int i;
    /* s3 19 */ int j;
    /* s4 20 */ int cont_job;
    /* a0 4 */ int drline;
    /* v0 2 */ int time_tmp;
    /* s0 16 */ int next_time;
    /* s0 16 */ //int next_time;
    /* s0 16 */ //int next_time;
    /* v1 3 */ SCRDAT *scrdat_pp;
    /* s0 16 */ //int next_time;
    /* v0 2 */ SCORE_INDV_STR *cngSindv_pp;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        for (j = 0; j < 24; i++)
        {
            cont_job = 0;

            if (sindv_pp->sjob[j] != -1)
            {
                if (sindv_pp->sjob[j] < ctime_next)
                {
                    switch (j)
                    {
                        case SCRSUBJ_CDSND_ON:
                            CdctrlWP2SetVolume(120);
                            break;
                        case SCRSUBJ_CDSND_OFF:
                            CdctrlWP2SetVolume(0);
                            break;
                        case SCRSUBJ_DRAW_CHANGE:
                            ScrLincChangTbl(sindv_pp->useLine);
                            break;
                        case SCRSUBJ_EFFECT:
                            TapCt(0xB0, sindv_pp->sjob_data[j][0], sindv_pp->sjob_data[j][1]);
                            break;
                        case SCRSUBJ_REVERS:
                            drline = sindv_pp->retStartLine;
                            if (sindv_pp->sjob_data[j][0] / 2 < ctime_next)
                                drline = sindv_pp->refTartegLine;
                            
                            time_tmp = 1;
                            cont_job = 1;

                            if ((sindv_pp->refTargetTime - sindv_pp->refStartTime) * (ctime_next / sindv_pp->sjob_data[j][0]))
                                time_tmp = (sindv_pp->refTargetTime - sindv_pp->refStartTime) * (ctime_next / sindv_pp->sjob_data[j][0]);

                            ScrLincChangTblRef(drline, time_tmp + sindv_pp->refStartTime);
                            break;

                        case SCRSUBJ_SPU_ON:
                        case SCRSUBJ_SPU_ON2:
                        case SCRSUBJ_SPU_ON3:
                        case SCRSUBJ_SPU_ON4:
                            ScrTapReq(-1, sindv_pp->sjob_data[j][0], sindv_pp->sjob_data[j][1]);
                            break;

                        case SCRSUBJ_TITLE:
                            // TODO
                            break;
                        case SCRSUBJ_LOOP:
                            cont_job = 1;
                            TimeCallbackTimeSetChanTempo(sindv_pp->useLine, GetLineTempo(sindv_pp->useLine), sindv_pp->sjob_data[j][0]);
                            break;
                        case SCRSUBJ_FADEOUT:
                            cont_job = 1;
                            fadeoutStartKey = 1;
                            break;
                        case SCRSUBJ_ENDLOOP:
                            gameEndWaitLoop = 1;
                            break;
                        case SCRSUBJ_SPUTRANS:
                            if (sindv_pp->sjob_data[j][0])
                                ScrTapDbuffSetSp(&score_str.stdat_dat_pp->scr_pp->sndrec_pp[sindv_pp->sjob_data[j][0]], sindv_pp->sndId);
                            break;
                        case SCRSUBJ_STOP_MENDERER:
                        {
                            int next_time = ScrCtrlIndvNextTime(sindv_pp, 1) - sindv_pp->current_time;

                            PrDecelerateMenderer((((next_time * 3600.0f) + (GetLineTempo(sindv_pp->useLine) * 96.0f * 0.5f)) / (GetLineTempo(sindv_pp->useLine) * 96.0f)));
                            printf("SCRSUBJ_STOP_MENDERER req\n");
                            break;
                        }
                        case SCRSUBJ_BONUS_GAME:
                            cont_job = 1LL;
                            bonusGameCtrl(ctime_next);
                            break;
                        case SCRSUBJ_BONUS_GAME_END:
                            bonusPointSave();
                            bonusScoreDraw();
                            break;

                        case SCRSUBJ_LESSON:
                            LessonRoundDisp(sindv_pp->sjob_data[j][0]);
                            cont_job = (sindv_pp->sjob_data[j][1] < 180);
                            break;
                        case SCRSUBJ_VS_RESET:
                            cngSindv_pp = GetSindvPcodeLine(PCODE_TEACHER);
                            if (cngSindv_pp)
                                cngSindv_pp->global_ply->score = 500;
                            
                            vsAnimationReset(1, 500);

                            cngSindv_pp = GetSindvPcodeLine(PCODE_PARA);
                            if (cngSindv_pp)
                                cngSindv_pp->global_ply->score = 500;
                            
                            vsAnimationReset(0, 500);
                            break;
                        case SCRSUBJ_CDSND_READY:
                            CdctrlWP2Set(&score_str.stdat_dat_pp->sndfile[sindv_pp->sjob_data[j][0]]);
                            break;
                        case SCRSUBJ_CDSND_REQ:
                            CdctrlWP2Play();
                            CdctrlWP2SetVolume(sindv_pp->sjob_data[j][0]);
                            break;
                        case SCRSUBJ_SPU_OFF:
                            ScrTapReqStop(sindv_pp->sjob_data[j][0]);
                            break;
                        case SCRSUBJ_JIMAKU_OFF:
                            jimakuWakuOff = 1;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}
#endif

INCLUDE_ASM("main/scrctrl", ScrCtrlIndvJob);

static void ScrTimeRenew(SCR_MAIN *scr_main_pp)
{
    int   i;
    int   samplecnt;
    float tempo;

    for (i = 0; i < scr_main_pp->scr_ctrl_num; i++)
    {
        if (scr_main_pp->scr_ctrl_pp[i].gtime_type == GTIME_VSYNC)
        {
            scr_main_pp->scr_ctrl_pp[i].lineTime =  ((TimeCallbackTimeGetChan(i) * 96.0f * GetLineTempo(i) + 1800.0f)  / 3600.0f);
            scr_main_pp->scr_ctrl_pp[i].lineTime += ((GetLineTempo(i) * 96.0f * scr_main_pp->scr_ctrl_pp[i].ofsCdtime) / 60000.0f);

            if (scr_main_pp->scr_ctrl_pp[i].lineTime < 0)
                scr_main_pp->scr_ctrl_pp[i].lineTime = 0;

            scr_main_pp->scr_ctrl_pp[i].lineTimeFrame = TimeCallbackTimeGetChan(i);
        }
        else
        {
            samplecnt = GlobalSndSampleGet() + ((scr_main_pp->scr_ctrl_pp[i].ofsCdtime * 48) / 256);

            if (global_data.play_step == PSTEP_XTR)
                samplecnt = CdctrlWp2GetSampleTmp() - getTopSeekPos();

            if (samplecnt < 0)
                samplecnt = 0;

            tempo = GetLineTempo(i);

            scr_main_pp->scr_ctrl_pp[i].lineTime = CdctrlWp2CdSample2SndTime(samplecnt, tempo);
            scr_main_pp->scr_ctrl_pp[i].lineTimeFrame = CdctrlWp2CdSample2Frame(samplecnt);
        }
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("main/scrctrl", ScrMbarReq);
#else
void ScrMbarReq(/* s5 21 */ int mbarTime)
{
    /* s6 22 */ int i;
    /* s2 18 */ SCORE_INDV_STR *sindv_pp;
    /* s1 17 */ TAPSET *tapset_pp;
    /* s0 16 */ int dare;
    /* s4 20 */ int tapdat_size;
    /* s3 19 */ TAPDAT *tapdat_pp;

    MbarSetCtrlTime(mbarTime);

    sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (sindv_pp->status & 1)
        {
            if ((sindv_pp->status & 158) == 0)
            {
                tapset_pp = IndvGetTapSetAdrs(sindv_pp);
                if (tapset_pp != NULL)
                {
                    tapdat_size = tapset_pp->tapdat_size;
                    tapdat_pp = tapset_pp->tapdat_pp;

                    if (global_data.play_step == PSTEP_VS)
                    {
                        dare = MBAR_PARAPPA_VS;

                        if (sindv_pp->plycode == PCODE_TEACHER)
                            dare = MBAR_TEACHER_VS;
                        if (sindv_pp->plycode == PCODE_BOXY)
                        {
                            dare = MBAR_BOXY_VS;
                            if (mbarTime < sindv_pp->current_time + tapset_pp->taptimeEnd)
                                MbarReset();
                        }
                    }
                    else
                    {
                        if (global_data.play_step == PSTEP_HOOK)
                            dare = MBAR_PARAPPA_HOOK;
                        else
                            dare = MBAR_PARAPPA;
                        
                        if (sindv_pp->plycode != PCODE_PARA)
                            dare = MBAR_TEACHER;
                    }

                    if (tapset_pp->tapscode == TAPSCODE_ANSWER)
                    {
                        tapdat_pp = vs_tapdat_tmp;
                        tapdat_size = vs_tapdat_tmp_cnt;
                    }

                    if ((dare == MBAR_PARAPPA) || (dare == MBAR_PARAPPA_HOOK))
                    {
                        if (mbarTime < sindv_pp->current_time)
                            dare = MBAR_NONE;
                    }

                    MbarReq(dare, tapset_pp, sindv_pp->current_time, sindv_pp->scr_tap_memory, sindv_pp->scr_tap_memory_cnt, game_status.language_type, tapdat_size, tapdat_pp, sindv_pp->cursor_num);
                }
            }
        }
    }
}
#endif

void allTimeCallbackTimeSetChanTempo(int time)
{
    int       i;
    SCR_MAIN *scr_main_pp = score_str.stdat_dat_pp->scr_pp;

    for (i = 0; i < scr_main_pp->scr_ctrl_num; i++)
    {
        if (scr_main_pp->scr_ctrl_pp[i].gtime_type == GTIME_VSYNC)
        {
            scr_main_pp->scr_ctrl_pp[i].lineTime = time;
            TimeCallbackTimeSetChanTempo(i, time, GetLineTempo(i));

            scr_main_pp->scr_ctrl_pp[i].lineTimeFrame = (time * 3600.0f + GetLineTempo(i) * 96.0f * 0.5f) / (GetLineTempo(i) * 96.0f);
        }
    }
}

INCLUDE_ASM("main/scrctrl", SetIndvDrawTblLine);
#if 0
int SetIndvDrawTblLine(/* s0 16 */ SCORE_INDV_STR *sindv_pp)
{
    /* a1 5 */ TAPSET *tapset_pp;
    /* a2 6 */ int ctime;

    if ((sindv_pp->status & SCS_USE) && (sindv_pp->status & 158))
    {
        {
            if (sindv_pp->top_scr_ctrlpp[sindv_pp->useLine].gtime_type != GTIME_VSYNC)
            {
                tapset_pp = IndvGetTapSetAdrs(sindv_pp);
                ctime     = sindv_pp->top_scr_ctrlpp[sindv_pp->useLine].lineTime;

                if (tapset_pp != NULL)
                {
                    if (global_data.play_step != PSTEP_VS)
                    {
                        if (ctime < sindv_pp->current_time + tapset_pp->taptimeStart)
                            return 0;
                    }

                    if (ctime < sindv_pp->current_time + tapset_pp->taptimeEnd)
                        currentTblNumber = sindv_pp->scrdat_pp->drawofs[global_data.tapLevel];
                }
            }
        }
    }

    return 0;
}
#endif

INCLUDE_ASM("main/scrctrl", otehonSetCheck);
#if 0
static int otehonSetCheck(void)
{
    /* s0 16 */ SCORE_INDV_STR *sindv_pp = score_indv_str;
    /* v0 2 */ TAPSET *tapset_pp;
    /* s1 17 */ int i;

    for (i = 0; i < 5; i++)
    {
        if (sindv_pp[i].status & 1)
            return 0;

        tapset_pp = IndvGetTapSetAdrs(sindv_pp);

        if (tapset_pp != NULL)
        {
            if (sindv_pp[i].top_scr_ctrlpp[sindv_pp->useLine].lineTime >= sindv_pp[i].current_time)
                return 1;
        }
    }
}
#endif

// INCLUDE_RODATA("main/scrctrl", D_00392F70);
INCLUDE_ASM("main/scrctrl", ScrCtrlMainLoop);

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

INCLUDE_ASM("main/scrctrl", SetIndvCdChannel);

INCLUDE_ASM("main/scrctrl", CheckIndvCdChannel);
int CheckIndvCdChannel(/* s1 17 */ SCORE_INDV_STR *sindv_pp, /* s0 16 */ u_char *chantmp);

// INCLUDE_RODATA("main/scrctrl", dbg_tbl_msg);

INCLUDE_ASM("main/scrctrl", ScrCtrlInit);

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

int ScrEndCheckScore(void)
{
    int             i;
    SCORE_INDV_STR *sindv_pp = score_indv_str;

    for (i = 0; i < 5; i++, sindv_pp++)
    {
        if (sindv_pp->status & SCS_USE)
        {
            if (sindv_pp->status & SCS_END)
            {
                printf("end end end[%d] line time[%d]\n", i, sindv_pp->top_scr_ctrlpp[i].lineTime);
                return 1;
            }
        }
    }

    return 0;
}

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

INCLUDE_ASM("main/scrctrl", bonusGameInit);

INCLUDE_ASM("main/scrctrl", bonusGameCntPls);

INCLUDE_ASM("main/scrctrl", bonusPointSave);

INCLUDE_ASM("main/scrctrl", bngTapEventCheck);
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
    else if (wtime < 13)
        return 13;
    else if (wtime < 20)
        return 18;
    else
        return 24;
}

static int bonus_pls_point_sub(int wtime)
{
    if (wtime < 3)
        return 26;
    else if (wtime < 6)
        return 15;
    else if (wtime < 9)
        return 9;
    else
        return 5;
}

INCLUDE_ASM("main/scrctrl", bonusGameCtrl);

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

INCLUDE_ASM("main/scrctrl", bnNumberDisp);
void bnNumberDisp(sceGifPacket *gif_pp, long score, short x, short y, int keta, int tate, int type);

static void bonusScoreDraw(void)
{
    long         scr_stg;
    long         scr_bn;
    long         scr_add;

    sceGifPacket bn_gif;
    VCLR_PARA    vclr_para = {};

    DrawVramClear(&vclr_para, 0, 0, 0, DNUM_VRAM2);
    ChangeDrawArea(DrawGetDrawEnvP(DNUM_VRAM2));

    CmnGifADPacketMake(&bn_gif, NULL);

    sceGifPkAddGsAD(&bn_gif, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(&bn_gif, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(0, 0, 0, 0, 0, 0, 1, 1));
    sceGifPkAddGsAD(&bn_gif, SCE_GS_TEXA, 0x8000008000);
    sceGifPkAddGsAD(&bn_gif, SCE_GS_CLAMP_1, 5);
    sceGifPkAddGsAD(&bn_gif, SCE_GS_PABE, 0);
    sceGifPkAddGsAD(&bn_gif, SCE_GS_TEXA, 0x8000008000);

    scr_bn  = ingame_common_str.BonusScore;
    scr_stg = ingame_common_str.SingleScore;
    scr_add = scr_stg + scr_bn;

    bnNumberDisp(&bn_gif, scr_stg,  0,  0, 5, 1, 0);
    bnNumberDisp(&bn_gif, scr_stg, 96, 32, 5, 0, 1);

    bnNumberDisp(&bn_gif, scr_bn,  32,  0, 5, 1, 0);
    bnNumberDisp(&bn_gif, scr_bn,  96, 56, 5, 0, 1);

    bnNumberDisp(&bn_gif, scr_add, 64,  0, 5, 1, 0);
    bnNumberDisp(&bn_gif, scr_add, 96, 80, 5, 0, 1);

    bnNumberDisp(&bn_gif, ingame_common_str.BonusStage, 96,  0, 1, 0, 0);
    bnNumberDisp(&bn_gif, ingame_common_str.BonusStage, 128, 0, 1, 0, 1);

    CmnGifADPacketMakeTrans(&bn_gif);
}

static void set_lero_gifset(sceGifPacket *gifpk_pp, LERO_TIM2_PT *let2_pp, short xp, short yp)
{
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_UV,   SCE_GS_SET_UV(let2_pp->u0 << 4, let2_pp->v0 << 4));
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((xp + 2048) << 4,
                                                           (yp + 2048) << 4, 1));

    sceGifPkAddGsAD(gifpk_pp, SCE_GS_UV,   SCE_GS_SET_UV(let2_pp->u1 << 4, let2_pp->v1 << 4));
    sceGifPkAddGsAD(gifpk_pp, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((xp + 2048 + let2_pp->w) << 4, 
                                                           (yp + 2048 + let2_pp->h) << 4, 1));
}

static void LessonRoundDisp(SCRRJ_LESSON_ROUND_ENUM type)
{
    sceGifPacket gifpk;
    TIM2_DAT    *tim2_dat_pp;
    int          i;

    if (type < SCRRJ_LR_MAX)
    {
        tim2_dat_pp = lessonTim2InfoGet();

        // Preserve CBP from our TEX0
        (*(sceGsTex0*)&tim2_dat_pp->GsTex0).CBP = (*(sceGsTex0*)&lessonCl2InfoGet(type)->GsTex0).CBP;

        CmnGifOpenCmnPk(&gifpk);
        ChangeDrawAreaSetGifTag(DrawGetDrawEnvP(DNUM_DRAW), &gifpk);

        sceGifPkAddGsAD(&gifpk, SCE_GS_TEXFLUSH, 0);
        sceGifPkAddGsAD(&gifpk, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(1, 6, 0, 0, 0, 0, 1, 1));
        sceGifPkAddGsAD(&gifpk, SCE_GS_TEXA, 0x8000008000);
        sceGifPkAddGsAD(&gifpk, SCE_GS_CLAMP_1, 5);
        sceGifPkAddGsAD(&gifpk, SCE_GS_PABE, 0);
        sceGifPkAddGsAD(&gifpk, SCE_GS_TEXA, 0x8000008000);
        sceGifPkAddGsAD(&gifpk, SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA_1(0, 1, 0, 1, 0));
        sceGifPkAddGsAD(&gifpk, SCE_GS_TEX0_1, tim2_dat_pp->GsTex0);
        sceGifPkAddGsAD(&gifpk, SCE_GS_TEX1_1, tim2_dat_pp->GsTex1);
        sceGifPkAddGsAD(&gifpk, SCE_GS_PRIM, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 0, 0));

        for (i = 0; i < 2; i++)
        {
            set_lero_gifset(&gifpk, &lero_tim2_pt[lero_pos_str[type][i].tim2_num], lero_pos_str[type][i].posx, lero_pos_str[type][i].posy);
        }

        CmnGifCloseCmnPk(&gifpk, 2);
    }
}
