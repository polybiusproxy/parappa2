#include "main/p3str.h"

/* sdata - static */
u_int current_time_old;

static P3STR *p3str_pp;

static ADRD  *adrd_common;
static int    adrd_common_cnt;

/* bss - static */
P3STR_SORT p3str_sort[16];

static int p3str_sort_cnt;

void adrd_common_clr(void)
{
    int i;

    for (i = 0; i < adrd_common_cnt; i++)
    {
        if (adrd_common[i].ftype == 0)
            adrd_common[i].handle = NULL;
    }
}

ADRD* adrd_common_ck(int id)
{
    return &adrd_common[id];
}

void p3strImage2RealPos(P3STR_SD *p3str_sd_pp)
{
    int   i;
    ADRD *adrd_pp;

    if (!p3str_sd_pp->first)
    {
        if (p3str_sd_pp->usrDsize != 0)
            p3str_sd_pp->usrD += (int)&p3str_sd_pp->type;

        if (p3str_sd_pp->dataDsize != 0)
            p3str_sd_pp->dataD += (int)&p3str_sd_pp->type;

        if (p3str_sd_pp->adrDsize != 0)
        {
            p3str_sd_pp->adrD += (int)&p3str_sd_pp->type;
            adrd_pp = (ADRD*)p3str_sd_pp->adrD;

            for (i = 0; i < p3str_sd_pp->adrDsize / 16; i++, adrd_pp++)
            {
                if (adrd_pp->common < 0)
                    adrd_pp->adrs += (u_int)p3str_sd_pp;
                else
                    adrd_pp->adrs = (u_int)adrd_common_ck(adrd_pp->common);
            }

            p3str_sd_pp->first = 1;
        }
    }
}

INCLUDE_ASM(const s32, "main/p3str", p3StrInit);

int getTopSeekPos(void)
{
    return p3str_pp->seek;
}

ADRD* getADRDnum(ADRD *adr_pp, int id)
{
    ADRD *ret = &adr_pp[id];

    if (ret->common >= 0)
        ret = (ADRD*)ret->adrs;

    return ret;
}

INCLUDE_ASM(const s32, "main/p3str", p3StrInitSd);

INCLUDE_ASM(const s32, "main/p3str", p3StrQuitSdEach);

INCLUDE_ASM(const s32, "main/p3str", p3StrDispDs);

void p3str_sort_init(void)
{
    WorkClear(p3str_sort, sizeof(p3str_sort));
    p3str_sort_cnt = 0;
}

void p3str_sort_set(P3SRT_OD *p3o_pp, P3STR_SD *p3sd_pp, int subtime)
{
    if (p3str_sort_cnt > 16)
    {
        printf("p3str pack over\n");
        return;
    }

    p3str_sort[p3str_sort_cnt].p3o_pp  = p3o_pp;
    p3str_sort[p3str_sort_cnt].p3sd_pp = p3sd_pp;
    p3str_sort[p3str_sort_cnt].subtime = subtime;

    p3str_sort_cnt++;
}

INCLUDE_ASM(const s32, "main/p3str", p3str_sort_disp);

INCLUDE_ASM(const s32, "main/p3str", p3StrPoll);

INCLUDE_ASM(const s32, "main/p3str", p3StrQuitSd);
