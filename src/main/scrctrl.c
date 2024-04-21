#include "main/scrctrl.h"

/* data */
extern u_int thnum_tbl[4];
extern SCR_SND_DBUFF scr_snd_dbuff;
extern SCORE_STR score_str;

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

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDataTrans);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapDataTransCheck);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapReq);

INCLUDE_ASM(const s32, "main/scrctrl", ScrTapReqStop);

INCLUDE_ASM(const s32, "main/scrctrl", exam_tbl_updownInit);
void exam_tbl_updownInit(SCORE_INDV_STR *sindv_pp);

INCLUDE_ASM(const s32, "main/scrctrl", exam_tbl_updownSet);

INCLUDE_ASM(const s32, "main/scrctrl", exam_tbl_updownChange);
int exam_tbl_updownChange(/* a0 4 */ SCORE_INDV_STR *sindv_pp, /* t1 9 */ TAP_CTRL_LEVEL_ENUM clv, /* a2 6 */ TAP_ROUND_ENUM round, /* a3 7 */ int coolf);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSetMemorySave);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSetMemoryLoad);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSet);

INCLUDE_ASM(const s32, "main/scrctrl", vsTapdatSetMoto);

void followTapInit(void)
{
    follow_scr_tap_memory_cnt = 0;
    follow_scr_tap_memory_cnt_load = 0;
    WorkClear(follow_scr_tap_memory, sizeof(follow_scr_tap_memory));
}

INCLUDE_ASM(const s32, "main/scrctrl", followTapSave);

INCLUDE_ASM(const s32, "main/scrctrl", followTapLoad);
#if 0
static SCR_TAP_MEMORY* followTapLoad(int pos, int time)
{
    if (follow_scr_tap_memory_cnt > pos)
    {
        if (follow_scr_tap_memory[pos].ofs_frame > time)
        {
            return NULL;
        }
    }

    if (follow_scr_tap_memory_cnt_load == pos)
    {
        follow_scr_tap_memory_cnt_load = pos + 1;
        return follow_scr_tap_memory + pos;
    }

    return NULL;
}
#endif

INCLUDE_ASM(const s32, "main/scrctrl", ScrLincChangTbl);

INCLUDE_ASM(const s32, "main/scrctrl", ScrLincChangTblRef);

INCLUDE_ASM(const s32, "main/scrctrl", ScrLineSafeRefMode);

INCLUDE_ASM(const s32, "main/scrctrl", ScrDrawTimeGet);

INCLUDE_ASM(const s32, "main/scrctrl", ScrDrawTimeGetFrame);

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
    SCRREC *scrrec_pp = sindv_pp->top_scr_ctrlpp[index].scrrec_pp;

    if (!scrrec_pp)
    {
        printf("ScrCtrlCurrentSearch index[%d] is NULL line!!\n", index);
        return NULL;
    }

    if (index >= score_str.stdat_dat_pp->scr_pp->scr_ctrl_num)
    {
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

    printf("ScrCtrlCurrentSearch index[%d] is over!!\n", index);
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

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlExamClear);

void ScrCtrlExamClearIndv(SCR_EXAM_STR *sexam_pp)
{
    sexam_pp->exam_do = EXAM_DO_NON;
    sexam_pp->exam_start = -1;
}

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvNextTime);

INCLUDE_ASM(const s32, "main/scrctrl", ScrCtrlIndvNextReadLine);

INCLUDE_ASM(const s32, "main/scrctrl", getLvlTblRand);

INCLUDE_ASM(const s32, "main/scrctrl", tapLevelChangeSub);
int tapLevelChangeSub(void);

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

INCLUDE_ASM(const s32, "main/scrctrl", intIndvStatusSet);

INCLUDE_ASM(const s32, "main/scrctrl", allIndvNextContinue);

INCLUDE_ASM(const s32, "main/scrctrl", allIndvGoContinue);

INCLUDE_ASM(const s32, "main/scrctrl", otherIndvPause);

INCLUDE_ASM(const s32, "main/scrctrl", otherIndvTapReset);

INCLUDE_ASM(const s32, "main/scrctrl", selectIndvTapResetPlay);

INCLUDE_ASM(const s32, "main/scrctrl", IndivMoveChange);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevAllMove);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevCodeGet);

INCLUDE_ASM(const s32, "main/scrctrl", targetTimeGet);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevSndKill);

INCLUDE_ASM(const s32, "main/scrctrl", useAllClearKeySnd);
void useAllClearKeySnd(void);

INCLUDE_ASM(const s32, "main/scrctrl", useIndevSndKillOther);

INCLUDE_ASM(const s32, "main/scrctrl", TapKeyCheckNum);

INCLUDE_ASM(const s32, "main/scrctrl", TapKeyGetDatPP);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupInit);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupTapClear);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroup);

INCLUDE_ASM(const s32, "main/scrctrl", tapReqGroupPoll);

INCLUDE_ASM(const s32, "main/scrctrl", tapEventCheck);

INCLUDE_ASM(const s32, "main/scrctrl", otehon_all_make);

INCLUDE_ASM(const s32, "main/scrctrl", treateTimeChange);

//INCLUDE_ASM(const s32, "main/scrctrl", thnum_get);
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
