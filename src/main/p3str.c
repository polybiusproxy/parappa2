#include "main/p3str.h"
#include "main/etc.h"

#include "prlib/prlib.h"

static u_int current_time_old = 0;

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

INCLUDE_ASM("main/p3str", p3StrInit);
#if 0
void p3StrInit(/* s7 23 */ u_int adrs)
{
    /* s0 16 */ int i;
    /* s1 17 */ int j;
    /* s2 18 */ P3SRT_OD *p3srt_od_pp;
    /* s3 19 */ P3STR_SD *p3str_sd;
    /* s6 22 */ ADRD *adrd_pp;
    /* s5 21 */ int *usr_pp;

    current_time_old = 0;
    p3str_pp = (P3STR*)adrs;

    if (STR(adrs)->ODcnt != 0)
    {
        p3srt_od_pp = STR(adrs)->p3srt_od;

        for (i = 0; i < p3str_pp->ODcnt; i++, p3srt_od_pp++)
        {
            p3str_sd = (P3STR_SD*)(p3srt_od_pp->adr + (int)adrs);

            if (p3srt_od_pp->od_type == OD_VRAM)
            {
                p3strImage2RealPos(p3str_sd);

                adrd_pp = (ADRD*)p3str_sd->adrD;
                usr_pp  = (int*)p3str_sd->usrD;

                for (j = 0; j < p3str_sd->Dcnt; j++)
                {
                    if (usr_pp[j] == 0 || usr_pp[j] == GetHatRound() + 1)
                        Tim2Trans((void*)adrd_pp[j].adrs);
                }
                
            }
            else if (p3srt_od_pp->od_type == OD_COMMON)
            {
                adrd_common_cnt = 0;
                p3StrInitSd(p3srt_od_pp, p3str_sd, i);

                adrd_common = (ADRD*)p3str_sd->adrD;
                adrd_common_cnt = p3str_sd->adrDsize / 16;
            }
        }
    }

    PrSetFrameRate(60.0f);
}
#endif

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

int p3StrInitSd(P3SRT_OD *p3o_pp, P3STR_SD *p3sd_pp, int datnum)
{
    int    i;

    ADRD  *adrd_pp;
    int    adrd_cnt;

    u_char scnname[4];
    u_int  fbp_tmp;
    
    ADRD  *adrd_tmp;
    u_int  handle_tmp;

    if (p3o_pp->first || p3o_pp->od_type == OD_VRAMCLR)
        return 0;

    p3strImage2RealPos(p3sd_pp);

    adrd_common_clr();
    adrd_pp  = (ADRD*)p3sd_pp->adrD;
    adrd_cnt = p3sd_pp->adrDsize / 16;

    if (p3o_pp->od_type == OD_SCENE)
    {
        sprintf(scnname, "%d", datnum);
        printf("scene no:%d\n", datnum);

        if (p3o_pp->use)
            fbp_tmp = DrawGetFbpPos(p3o_pp->use);
        else
            fbp_tmp = -1;
        
        p3o_pp->pad2 = (u_int)PrInitializeScene(&DBufDc.draw01, scnname, fbp_tmp);
    }

    for (i = 0; i < adrd_cnt; i++)
    {
        handle_tmp = NULL;
        adrd_tmp   = getADRDnum(adrd_pp, i);

        if (adrd_tmp->handle != NULL)
        {
            handle_tmp = adrd_tmp->handle;
        }
        else
        {
            switch (adrd_tmp->ftype)
            {
                case AF_SPM:
                    if (p3sd_pp->type == OD_SCENE)
                        handle_tmp = (u_int)PrInitializeModel((PR_MODELHANDLE)adrd_tmp->adrs, (PR_SCENEHANDLE)p3o_pp->pad2);
                    break;
                case AF_SPA:
                    handle_tmp = (u_int)PrInitializeAnimation((PR_ANIMATIONHANDLE)adrd_tmp->adrs);
                    break;
                case AF_SPC:
                    handle_tmp = (u_int)PrInitializeCamera((PR_CAMERAHANDLE)adrd_tmp->adrs);
                    break;
                case AF_TM2:
                case AF_CL2:
                    handle_tmp = adrd_tmp->adrs;
                    break;
                default:
                    break;
            }
        }

        if (adrd_pp[i].common < 0)
        {
            adrd_pp[i].handle = handle_tmp;
        }
        else
        {
            adrd_pp[i].handle = handle_tmp;
            adrd_tmp->handle  = handle_tmp;
        }
    }

    if (p3sd_pp->type == OD_SCENE)
        PrPreprocessSceneModel((PR_SCENEHANDLE)p3o_pp->pad2);

    printf("init end\n");

    p3o_pp->first = 1;
    return 1;
}

int p3StrQuitSdEach(P3SRT_OD *p3o_pp, P3STR_SD *p3sd_pp)
{
    int   i;
    ADRD *adrd_pp;
    int   adrd_cnt;

    if (!p3o_pp->first)
        return 0;

    adrd_cnt = p3sd_pp->adrDsize / 16;
    adrd_pp  = (ADRD*)p3sd_pp->adrD;

    for (i = 0; i < adrd_cnt; i++)
    {
        if (adrd_pp[i].handle != NULL)
        {
            if (adrd_pp[i].ftype == 0 || adrd_pp[i].common < 0)
            {
                switch (adrd_pp[i].ftype)
                {
                case AF_SPM:
                    if (p3sd_pp->type == OD_SCENE)
                        PrCleanupModel((PR_MODELHANDLE)adrd_pp[i].handle);
                    break;
                case AF_SPA:
                    PrCleanupAnimation((PR_ANIMATIONHANDLE)adrd_pp[i].handle);
                    break;
                case AF_SPC:
                    PrCleanupCamera((PR_CAMERAHANDLE)adrd_pp[i].handle);
                    break;
                case AF_SPF:
                case AF_TM2:
                case AF_CL2:
                default:
                    break;
                }
            }

            adrd_pp[i].handle = NULL;
        }
    }

    if (p3sd_pp->type == OD_SCENE)
    {
        PrCleanupScene((PR_SCENEHANDLE)p3o_pp->pad2);
        p3o_pp->pad2 = 0;
    }

    p3o_pp->first = 0;
    return 1;
}

void p3StrDispDs(P3SRT_OD *p3o_pp, P3STR_SD *p3sd_pp, int subtime)
{
    int        i;

    int        cur_time;
    int        tmp_time;

    SD_SCENE  *scn_pp;
    ADRD      *adrd_pp;

    SD_FADE   *fade_pp;
    SD_DISPIN *dispin_pp;

    if (p3o_pp->od_type == OD_SCENE)
    {
        scn_pp  = (SD_SCENE*)p3sd_pp->usrD;
        adrd_pp = (ADRD*)p3sd_pp->adrD;

        ChangeDrawArea(DrawGetDrawEnvP(p3o_pp->draw));
        PrSetSceneEnv((PR_SCENEHANDLE)p3o_pp->pad2, DrawGetDrawEnvP(p3o_pp->draw));

        for (i = 0; i < p3sd_pp->Dcnt; i++, scn_pp++)
        {
            if (subtime >= scn_pp->start)
            {
                cur_time = subtime - scn_pp->start + scn_pp->top;
                tmp_time = scn_pp->end - scn_pp->top - (subtime - scn_pp->start);

                switch (scn_pp->type)
                {
                case 0:
                    PrShowModel((PR_MODELHANDLE)adrd_pp[scn_pp->adr1num].handle, 0);
                    break;
                case 1:
                    if (tmp_time < 1)
                    {
                        PrHideModel((PR_MODELHANDLE)adrd_pp[scn_pp->adr1num].handle);
                    }
                    else
                    {
                        PrLinkAnimation((PR_MODELHANDLE)adrd_pp[scn_pp->adr1num].handle, (PR_ANIMATIONHANDLE)adrd_pp[scn_pp->adr2num].handle);
                        PrAnimateModel((PR_MODELHANDLE)adrd_pp[scn_pp->adr1num].handle, cur_time);
                        PrShowModel((PR_MODELHANDLE)adrd_pp[scn_pp->adr1num].handle, 0);
                    }
                    break;
                case 2:
                    if (tmp_time < 1)
                    {
                        PrUnlinkPositionAnimation((PR_MODELHANDLE)adrd_pp[scn_pp->adr1num].handle);
                    }
                    else
                    {
                        PrLinkPositionAnimation((PR_MODELHANDLE)adrd_pp[scn_pp->adr1num].handle, (PR_CAMERAHANDLE)adrd_pp[scn_pp->adr2num].handle);
                        PrAnimateModelPosition((PR_MODELHANDLE)adrd_pp[scn_pp->adr1num].handle, cur_time);
                    }
                    break;
                case 3:
                    if (tmp_time > 0)
                    {
                        PrSelectCamera((PR_CAMERAHANDLE)adrd_pp[scn_pp->adr1num].handle, (PR_SCENEHANDLE)p3o_pp->pad2);
                        PrAnimateSceneCamera((PR_SCENEHANDLE)p3o_pp->pad2, cur_time);
                    }
                    break;
                case 4:
                    if (!scn_pp->pad1)
                    {
                        Tim2TransX((void*)adrd_pp[scn_pp->adr1num].handle, scn_pp->top);
                        scn_pp->pad1 = 1;
                    }
                    break;
                case 5:
                    if (tmp_time > 0)
                        Cl2MixTrans(cur_time, scn_pp->end, (u_char*)adrd_pp[scn_pp->adr1num].handle, (u_char*)adrd_pp[scn_pp->adr2num].handle);
                    break;
                default:
                    break;
                }
            }
        }

        PrRender((PR_SCENEHANDLE)p3o_pp->pad2);
        PrWaitRender();
    }

    if (p3o_pp->od_type == OD_FADE)
    {
        fade_pp = (SD_FADE*)p3sd_pp->usrD;
        fade_pp->subDadr = (u_int)fade_pp->subD;
        DrawFadeDisp(fade_pp, subtime, 0, p3o_pp->use, p3o_pp->draw);
    }

    if (p3o_pp->od_type == OD_DISPIN)
    {
        dispin_pp = (SD_DISPIN*)p3sd_pp->usrD;
        dispin_pp->subDadr = (u_int)dispin_pp->subD;
        DrawMoveDispIn(dispin_pp, subtime, 0, p3o_pp->use, p3o_pp->draw);
    }

    if (p3o_pp->od_type == OD_VRAMCLR)
    {
        DrawVramClear(&p3o_pp->pad1, subtime, 0, p3o_pp->use, p3o_pp->draw);
    }
}

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

void p3str_sort_disp(void)
{
    int        i;
    int        j;
    P3STR_SORT p3str_sort_tmp;

    if (p3str_sort_cnt != 0)
    {
        for (i = 0; i < p3str_sort_cnt - 1; i++)
        {
            for (j = i + 1; j < p3str_sort_cnt; j++)
            {
                if (p3str_sort[i].p3o_pp->pri > p3str_sort[j].p3o_pp->pri)
                {
                    p3str_sort_tmp = p3str_sort[i];
                    p3str_sort[i] = p3str_sort[j];
                    p3str_sort[j] = p3str_sort_tmp;
                }
            }
        }

        for (i = 0; i < p3str_sort_cnt; i++)
        {
            p3StrDispDs(p3str_sort[i].p3o_pp, p3str_sort[i].p3sd_pp, p3str_sort[i].subtime);
        }
    }
}

void p3StrPoll(u_int cur_time)
{
    int       i;
    P3SRT_OD *p3srt_od_pp;
    P3STR_SD *p3str_sd;

    if (cur_time < current_time_old)
    {
        printf("back time\n");
        cur_time = current_time_old;
    }

    current_time_old = cur_time;

    p3srt_od_pp = p3str_pp->p3srt_od;
    p3str_sort_init();

    for (i = 0; i < p3str_pp->ODcnt; i++, p3srt_od_pp++)
    {
        p3str_sd = (P3STR_SD*)(p3srt_od_pp->adr + (int)p3str_pp);

        if (cur_time >= p3srt_od_pp->start && cur_time < p3srt_od_pp->end)
        {
            p3StrInitSd(p3srt_od_pp, p3str_sd, i);
            p3str_sort_set(p3srt_od_pp, p3str_sd, cur_time - p3srt_od_pp->start);
        }
        else if (cur_time >= p3srt_od_pp->end)
        {
            p3StrQuitSdEach(p3srt_od_pp, p3str_sd);
        }
    }

    p3str_sort_disp();
    UsrPrSetScene();
}

void p3StrQuitSd(void)
{
    int       i;
    P3SRT_OD *p3srt_od_pp;

    p3srt_od_pp = p3str_pp->p3srt_od;

    for (i = 0; i < p3str_pp->ODcnt; i++, p3srt_od_pp++)
    {
        if (p3srt_od_pp->od_type != OD_VRAMCLR)
        {
            p3StrQuitSdEach(p3srt_od_pp, (P3STR_SD*)(p3srt_od_pp->adr + (int)p3str_pp));
        }
    }

    current_time_old = 0;
}
