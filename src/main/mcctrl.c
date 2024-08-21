#include "main/mcctrl.h"

#include "os/syssub.h"
#include "os/system.h"

#include <string.h>
#include <stdio.h>

/* data - static */
extern u_char ascii2sjiscng_tbl[];

/* bss - static */
extern MC_REP_STR mc_rep_str_local;

static u_char mc_holdTmp[4];
static u_char mc_resetTmp[4];

/* bss - static */
extern MC_REP_CTRL mc_rep_ctrl;

int setAscii2SjisCode(u_char *saki_pp, u_char *moto_pp)
{
    int    i;
    u_char dat_tmp;

    *saki_pp = '\0';

    for (i = 0; i < strlen(moto_pp); i++)
    {
        dat_tmp = moto_pp[i];

        if (dat_tmp < 0x20 || dat_tmp > 0x7f)
        {
            printf("dat over!!\n");
            return 1;
        }

        *saki_pp++ = ascii2sjiscng_tbl[(dat_tmp - 32) * 2];
        *saki_pp++ = ascii2sjiscng_tbl[(dat_tmp - 32) * 2 + 1];
    }

    *saki_pp = '\0';
    return 0;
}

void mccReqInit(void)
{
    WorkClear(&mc_rep_str_local, sizeof(mc_rep_str_local));
    WorkClear(mc_holdTmp, sizeof(mc_holdTmp));
    WorkClear(mc_resetTmp, sizeof(mc_resetTmp));
}

void mccReqSaveInit(void)
{   
    mc_rep_str_local.scoreN_cnt = 0;
    mc_rep_str_local.levelN_cnt = 0;
    mc_rep_str_local.mc_rep_dat_cnt = 0;
    mc_rep_str_local.mc_vsoth_cnt = 0;
}

void mccReqCtrlClr(void)
{
    WorkClear(&mc_rep_ctrl, sizeof(mc_rep_ctrl));
}

void mccReqScrSet(MC_REP_SCR *mcr_scr_pp)
{
    if (mc_rep_str_local.scoreN_cnt >= 256)
    {
        printf("score save over!!\n");
        return;
    }

    mc_rep_str_local.mc_rep_scr[mc_rep_str_local.scoreN_cnt] = *mcr_scr_pp;
    mc_rep_str_local.scoreN_cnt++;
}

MC_REP_SCR* mccReqScrGet(void)
{
    MC_REP_SCR *ret;

    if (mc_rep_ctrl.cl_scoreN_cnt >= 256 || mc_rep_ctrl.cl_scoreN_cnt >= mc_rep_str_local.scoreN_cnt)
    {
        printf("score load over!!\n");
        return NULL;
    }

    ret = &mc_rep_str_local.mc_rep_scr[mc_rep_ctrl.cl_scoreN_cnt];
    mc_rep_ctrl.cl_scoreN_cnt++;
    return ret;
}

void mccReqLvlSet(u_int lvl)
{
    if (mc_rep_str_local.levelN_cnt >= 256)
    {
        printf("level save over!!\n");
        return;
    }

    mc_rep_str_local.levelN[mc_rep_str_local.levelN_cnt] = lvl;
    mc_rep_str_local.levelN_cnt++;
}

u_int mccReqLvlGet(void)
{
    u_int ret;

    if (mc_rep_ctrl.cl_levelN_cnt >= 256 || mc_rep_ctrl.cl_levelN_cnt >= mc_rep_str_local.levelN_cnt)
    {
        printf("level load over!!\n");
        return 0;
    }

    ret = mc_rep_str_local.levelN[mc_rep_ctrl.cl_levelN_cnt];
    mc_rep_ctrl.cl_levelN_cnt++;
    return ret;
}

void mccReqTapSet(u_int time, u_int useLine, u_int id, PLAYER_ENUM ply)
{
    MC_REP_DAT *mcrd_pp;

    if (mc_rep_str_local.mc_rep_dat_cnt >= 2560)
    {
        printf("tap save over!!\n");
        return;
    }

    mcrd_pp = &mc_rep_str_local.mc_rep_dat[mc_rep_str_local.mc_rep_dat_cnt];

    mcrd_pp->timeP = time;
    mcrd_pp->padId = id;
    mcrd_pp->useL = useLine;
    mcrd_pp->ply = ply;

    mcrd_pp->resT = 0;
    mcrd_pp->holdT = 0;

    if (mc_resetTmp[ply] != 0)
        mcrd_pp->resT = 1;

    if (mc_holdTmp[ply] != 0)
        mcrd_pp->holdT = 1;

    mc_resetTmp[ply] = 0;
    mc_holdTmp[ply] = 0;

    mc_rep_str_local.mc_rep_dat_cnt++;
}

void mccReqTapResetSet(PLAYER_ENUM ply)
{
    mc_resetTmp[ply] = 1;
}

void mccReqTapHoldSet(PLAYER_ENUM ply)
{
    mc_holdTmp[ply] = 1;
}

void mccReqVSOTHSAVEset(VSOTHSAVE *sv)
{
    int i;

    if (mc_rep_str_local.mc_vsoth_cnt >= 100)
    {
        printf("vs oth save over!!\n");
        return;
    }

    for (i = 0; i < 32; i++)
    {
        mc_rep_str_local.vsothsave[mc_rep_str_local.mc_vsoth_cnt][i] = (*sv)[i];
    }

    mc_rep_str_local.mc_vsoth_cnt++;
}

int mccReqVSOTHSAVEget(VSOTHSAVE *sv)
{
    int i;

    if (mc_rep_ctrl.cl_vsoth_cnt >= 100 || mc_rep_ctrl.cl_vsoth_cnt >= mc_rep_str_local.mc_vsoth_cnt)
    {
        printf("vs oth load over!!\n");
        return 0;
    }
    
    for (i = 0; i < 32; i++)
    {
        (*sv)[i] = mc_rep_str_local.vsothsave[mc_rep_ctrl.cl_vsoth_cnt][i];
    }

    mc_rep_ctrl.cl_vsoth_cnt++;
    return 1;
}

u_short mccReqTapGet(u_int time, u_int useLine, u_int *time_pp, PLAYER_ENUM ply)
{
    u_short     ret;
    MC_REP_DAT *mcrd_pp;
    u_int      *rep_cnt;

    rep_cnt = &mc_rep_ctrl.cl_mc_rep_dat_cnt[ply];

    while (1)
    {
        if (*rep_cnt >= 2560 || *rep_cnt >= mc_rep_str_local.mc_rep_dat_cnt)
            return 0;

        mcrd_pp = &mc_rep_str_local.mc_rep_dat[*rep_cnt];

        if (mcrd_pp->ply == ply)
            break;

        (*rep_cnt)++;
    }

    if (mcrd_pp->timeP > time)
        return 0;

    if (mcrd_pp->useL != useLine)
        return 0;

    ret = GetIndex2KeyCode(mcrd_pp->padId);

    if (mcrd_pp->holdT)
        ret |= 0x2000;
    if (mcrd_pp->resT)
        ret |= 0x8000;

    *time_pp = mcrd_pp->timeP;
    (*rep_cnt)++;
        
    return ret;
}

extern const char D_00393800[]; /* rodata - "TAP forward!!\n" */

#if 1
INCLUDE_ASM(const s32, "main/mcctrl", mccReqTapForward);
#else
void mccReqTapForward(/* s5 21 */ u_int time, /* s4 20 */ u_int useLine)
{
    /* v1 3 */ MC_REP_DAT *mcrd_pp;
    /* s2 18 */ int i;
    /* s1 17 */ u_int *rep_cnt;

    rep_cnt = mc_rep_ctrl.cl_mc_rep_dat_cnt;

    for (i = 0; i < 4; i++, rep_cnt++)
    {
        if (*rep_cnt >= 2560 || *rep_cnt >= mc_rep_str_local.mc_rep_dat_cnt)
            return;

        mcrd_pp = &mc_rep_str_local.mc_rep_dat[*rep_cnt];

        if ((mcrd_pp->timeP >= time) && (mcrd_pp->useL == useLine))
        {
            while (mcrd_pp->ply == i)
            {
                printf(D_00393800);

                if (*rep_cnt >= 2560 || *rep_cnt >= mc_rep_str_local.mc_rep_dat_cnt)
                    return;

                if (mcrd_pp->timeP > time || mcrd_pp->useL != useLine)
                    break;
            }
        }
    }
}
#endif

INCLUDE_ASM(const s32, "main/mcctrl", mccReqTapForwardOwn);
#if 0
void mccReqTapForwardOwn(/* s3 19 */ u_int time, /* s2 18 */ u_int useLine, /* s4 20 */ int ply)
{
    /* v1 3 */ MC_REP_DAT *mcrd_pp;
    /* s1 17 */ u_int *rep_cnt;

    rep_cnt = &mc_rep_ctrl.cl_mc_rep_dat_cnt[ply];

    if (*rep_cnt >= 2560 || *rep_cnt >= mc_rep_str_local.mc_rep_dat_cnt)
        return;

    mcrd_pp = &mc_rep_str_local.mc_rep_dat[*rep_cnt];

    if ((mcrd_pp->timeP >= time) && (mcrd_pp->useL == useLine))
    {
        while (mcrd_pp->ply == ply)
        {
            
            (*rep_cnt)++;

            printf(D_00393800);

            mcrd_pp = &mc_rep_str_local.mc_rep_dat[*rep_cnt];

            if (*rep_cnt >= 2560 || *rep_cnt >= mc_rep_str_local.mc_rep_dat_cnt)
                break;

            if (mcrd_pp->timeP > time || mcrd_pp->useL != useLine)
                break; 
        }
    }
}
#endif

void mccLocalGlobalCopy(void)
{
    mc_rep_str = mc_rep_str_local;

    mc_rep_str.play_modeS       = game_status.play_modeG;
    mc_rep_str.play_typeS       = game_status.play_typeG;
    mc_rep_str.roundS           = game_status.roundG;
    mc_rep_str.play_stageS      = game_status.play_stageG;
    mc_rep_str.play_table_modeS = game_status.play_table_modeG;
}

void mccGlobalLocalCopy(void)
{
    mc_rep_str_local = mc_rep_str;
}

INCLUDE_RODATA(const s32, "main/mcctrl", D_00393800);
