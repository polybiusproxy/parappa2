#include "main/drawctrl.h"

#include <eetypes.h>
#include <eestruct.h>
#include <libpad.h>
#include <sifdev.h>
#include <stdio.h>

#include "dbug/dbgmsg.h"
#include "dbug/vramsave.h"

#include "os/tim2.h"
#include "os/syssub.h"
#include "os/system.h"
#include "os/cmngifpk.h"

#include "main/p3str.h"
#include "main/cdctrl.h"
#include "main/sprite.h"

#include <prlib/prlib.h>

// FIXME: From system.c
int oddeven_idx;
int outbuf_idx;

/* data */
/* static */ extern float bra_tap[10][2];
/* static */ extern MOZAIKU_POLL_STR mozaiku_poll_str[];

/* sdata - static */
VCLR_PARA vclr_black_tmp;

/* sdata - static */
int bthrow_ctrl_time;
int mend_title_req;
int ddbg_event_num;
int ddbg_scene_num;
int ddbg_go_event_scene;
int ddbg_tap_num;
int ddbg_bmp_frame;

/* sbss - static */
int scenectrl_outside_cnt;
int scenectrl_outside_read_cnt;
MOZAIKU_POLL_STR *mozaiku_poll_str_current_pp;
int dr_tap_req_num;
float men_ctrl_ratio;
MEN_CTRL_ENUM men_ctrl_enum;
int drawCurrentLine;
int drawCurrentTime;
int drawCurrentTimeOld;
EVENTREC *drawEventrec;

u_int useDispFlag;
u_int drDispFlag;
u_char ddbg_pause_f;

/* bss - static */
extern SCENECTRL scenectrl_outside[8];
extern BTHROW_CTRL bthrow_ctrl[2];
extern SCENECTRL *check_scenectrl[20];
extern void *tmp_buf_adrs[16];

/* lit4 */
float D_00398F14;
float D_00398F18;
float D_00398F1C;
float D_00398F20;
float D_00398F24;
float D_00398F28;
float D_00398F2C;
float D_00398F30;
float D_00398F34;

static void UseGsRegSet(void)
{
    sceGifPacket gifpk;
  
    CmnGifADPacketMake(&gifpk, NULL);
    CmnGifADPacketMakeTrans(&gifpk);
}

void UseGsSetXyOffset(int ofs)
{
    sceGifPacket gifpk;
    int offsy;
  
    if (ofs != 0)
        offsy = 0x7908;
    else
        offsy = 0x7900;

    CmnGifADPacketMake(&gifpk, NULL);
    sceGifPkAddGsAD(&gifpk, SCE_GS_XYOFFSET_1, SCE_GS_SET_XYOFFSET(0x6c00, offsy));
    CmnGifADPacketMakeTrans(&gifpk);
}

void outsideDrawSceneClear(void)
{
    scenectrl_outside_cnt = 0;
    scenectrl_outside_read_cnt = 0;
    WorkClear(scenectrl_outside, 0x100);
}

int outsideDrawSceneReq(OVL_FUNC prg_pp, u_char pri, u_int useF, u_int drawF, void *param)
{
    SCENECTRL *scenectrl_pp;

    if (scenectrl_outside_cnt >= 8)
    {
        printf("outside draw over!!\n");
        return 1;
    }

    scenectrl_pp = &scenectrl_outside[scenectrl_outside_cnt];

    scenectrl_pp->pri = pri;
    scenectrl_pp->condition = 0;

    scenectrl_pp->prg_pp = prg_pp;
    scenectrl_pp->useDisp = useF;
    scenectrl_pp->drDisp = drawF;
    scenectrl_pp->param_pp = param;

    scenectrl_outside_cnt++;
    return 0;
}

static SCENECTRL* getOutsideCtrlScene(int time)
{
    SCENECTRL *scenectrl_pp;

    if (scenectrl_outside_read_cnt >= scenectrl_outside_cnt)
        return NULL;

    scenectrl_pp = &scenectrl_outside[scenectrl_outside_read_cnt];

    scenectrl_pp->start_flame = time;
    scenectrl_pp->end_flame = time + 1;

    scenectrl_outside_read_cnt++;
    return scenectrl_pp;
}

#if 1
INCLUDE_ASM(const s32, "main/drawctrl", bra_tap_GetNext);
#else
float* bra_tap_GetNext(/* a0 4 */ PR_MODELHANDLE model)
{
    /* a0 4 */ int i;
    /* f1 39 */ float tmp_f = PrGetContourBlurAlpha(model);
    /* s0 16 */ float *ret;

    if (tmp_f == -1.0f)
        return NULL;

    if (bra_tap[0][0] == tmp_f)
    {
        ret = &bra_tap[1][0];
    }
    else
    {
        
        for (i = 0; i < 10; i++)
        {
            if (bra_tap[i][0] != tmp_f)
                ret = &bra_tap[i][0];
        }
    }

    return ret;
}
#endif

INCLUDE_ASM(const s32, "main/drawctrl", bra_title_GetNext);

INCLUDE_ASM(const s32, "main/drawctrl", bra_ret_GetNext);

INCLUDE_ASM(const s32, "main/drawctrl", XAnimationLinkOption);

void XAnimationPositionLink(PR_MODELHANDLE model, PR_ANIMATIONHANDLE animation, float time)
{
    PrLinkPositionAnimation(model, animation);
    PrAnimateModelPosition(model, time + 0.0f);
}

void XAnimationPositionUnLink(PR_MODELHANDLE model, PR_ANIMATIONHANDLE animation)
{
    if (PrGetLinkedPositionAnimation(model) == animation)
    {
        PrUnlinkPositionAnimation(model);
    }
}

void BallThrowSetFrame(int frame)
{
    bthrow_ctrl_time = frame;
}

void BallThrowInit(void)
{
    WorkClear(bthrow_ctrl, 0x1c28);

    bthrow_ctrl[0].targetY = 112.0f;
    bthrow_ctrl[0].targetX = 120.0f;

    bthrow_ctrl[1].targetX = 520.0f;
    bthrow_ctrl[1].targetY = 112.0f;
}

void BallThrowInitDare(int dare)
{
    WorkClear(&bthrow_ctrl[dare], 0xe14);
}

#if 1
INCLUDE_ASM(const s32, "main/drawctrl", vs06BomAdr);
void* vs06BomAdr(/* a0 4 */ OBJBTHROW_TYPE thtype, /* t0 8 */ int time);
#else
static void* vs06BomAdr(/* a0 4 */ OBJBTHROW_TYPE thtype, /* t0 8 */ int time)
{
    u_short bomdat_tea[12] = 
    {
        0x178, 0x177, 0x176, 0x175, 0x174, 0x173,
        0x172, 0x171, 0x170, 0x16F, 0x16E, 0x16D 
    };

    u_short bomdat_pa[12] = 
    {
        0x184, 0x183, 0x182, 0x181, 0x180, 0x17F,
        0x17E, 0x17D, 0x17C, 0x17B, 0x17A, 0x179
    };

    u_short num;

    if (global_data.play_step == PSTEP_VS)
    {
        if (time >= 12)
            return NULL;

        if (thtype == OBJBTHROW_PARAPPA)
            num = bomdat_pa[time];
        else
            num = bomdat_tea[time];

        return GetIntAdrsCurrent(num);
    }
}
#endif

void BallThrowReq(void *mdlh, OBJBTHROW_TYPE thtype, void *texpp, void *mdlhoming)
{
    BTHROW_STR *bt_pp;

    if (bthrow_ctrl[thtype].bthrow_str_cnt >= 128)
        return;

    bt_pp = &bthrow_ctrl[thtype].bthrow_str[bthrow_ctrl[thtype].bthrow_str_cnt];

    bt_pp->use = 3;
    bt_pp->mdl_adr = mdlh;
    bt_pp->tim2_dat_pp = texpp;
    bt_pp->homingpp = mdlhoming;

    if (global_data.play_step == PSTEP_VS)
    {
        bt_pp->endTime = 32;

        if (bthrow_ctrl_time < 32)
            bt_pp->endTime = bthrow_ctrl_time;
    }
    else
    {
        bt_pp->endTime = bthrow_ctrl_time;
    }

    bthrow_ctrl[thtype].frame = bthrow_ctrl_time;
    bthrow_ctrl[thtype].bthrow_str_cnt++;
}

void BallThrowTarget(void *mdlh, OBJBTHROW_TYPE thtype, int targetframe)
{
    bthrow_ctrl[thtype].targ_mdl_adr = mdlh;
}

void BallThrowPoll(void)
{   
    int i;
    int j;

    sceGifPacket gifP;

    TIM2INFO info;
    int w, h;
	int px, py;
    

    for (i = 0; i < 2; i++)
    {
        if (bthrow_ctrl[i].frame == 0)
        {
            BallThrowInitDare(i);
        }
        else
        {
            if (bthrow_ctrl[i].targ_mdl_adr != NULL)
            {
                float *pos_pp = PrGetModelScreenPosition(bthrow_ctrl[i].targ_mdl_adr);

                bthrow_ctrl[i].targetX = pos_pp[0];
                bthrow_ctrl[i].targetY = pos_pp[1] + -16.0f;
                bthrow_ctrl[i].targ_mdl_adr = NULL;
            }

            if (bthrow_ctrl[i].bthrow_str_cnt != 0)
            {
                BTHROW_STR *bts_pp = bthrow_ctrl[i].bthrow_str;
                
                CmnGifADPacketMake(&gifP, DrawGetFrameP(DNUM_DRAW));
                sceGifPkAddGsAD(&gifP, SCE_GS_TEXA, 0x8000008000);
                sceGifPkAddGsAD(&gifP, SCE_GS_ALPHA_1, 0x44);
                sceGifPkAddGsAD(&gifP, SCE_GS_TEST_1, 0x3000d);
                
                for (j = 0; j < bthrow_ctrl[i].bthrow_str_cnt; j++, bts_pp++)
                {
                    if (bts_pp->use & 1)
                    {
                        float *pos_pp = PrGetModelScreenPosition(bts_pp->homingpp);

                        bthrow_ctrl[i].targetX = pos_pp[0];
                        bthrow_ctrl[i].targetY = pos_pp[1] + -16.0f;

                        if (bts_pp->use & 2)
                        {
                            float *pos_pp;

                            bts_pp->use &= 0xFD;
                            
                            pos_pp = PrGetModelScreenPosition(bts_pp->mdl_adr);
                            
                            bts_pp->xp = pos_pp[0];
                            bts_pp->yp = pos_pp[1] + -16.0f;
                            bts_pp->mdl_adr = NULL;

                            if (global_data.play_step == PSTEP_VS && bts_pp->endTime < 12)
                            {
                                bts_pp->xp = bthrow_ctrl[i].targetX;
                                bts_pp->yp = bthrow_ctrl[i].targetY;
                            }
                        }
                        else if (global_data.play_step == PSTEP_VS)
                        {
                            if (bts_pp->endTime < 12)
                            {
                                if (bts_pp->endTime == 0)
                                    bts_pp->use = 0;

                                if (bts_pp->endTime == 11)
                                {
                                    bts_pp->xp = bthrow_ctrl[i].targetX;
                                    bts_pp->yp = bthrow_ctrl[i].targetY;
                                }

                                bts_pp->tim2_dat_pp = vs06BomAdr(i, bts_pp->endTime);
                            }
                            else if (bts_pp->endTime != 12)
                            {
                                bts_pp->xp += (bthrow_ctrl[i].targetX - bts_pp->xp) / (bts_pp->endTime - 12);
                                bts_pp->yp += (bthrow_ctrl[i].targetY - bts_pp->yp) / (bts_pp->endTime - 12);
                            }
                            else
                            {
                                bts_pp->xp = bthrow_ctrl[i].targetX;
                                bts_pp->yp = bthrow_ctrl[i].targetY;
                            }
                        }
                        else if (bts_pp->endTime == 0)
                        {
                            bts_pp->use = 0;
                            bts_pp->xp = bthrow_ctrl[i].targetX;
                            bts_pp->yp = bthrow_ctrl[i].targetY;
                        }
                        else 
                        {
                            bts_pp->xp += (bthrow_ctrl[i].targetX - bts_pp->xp) / bts_pp->endTime;
                            bts_pp->yp += (bthrow_ctrl[i].targetY - bts_pp->yp) / bts_pp->endTime;
                        }

                        GetTim2Info(bts_pp->tim2_dat_pp, &info, 1);

                        w = info.picturH->ImageWidth * 16;
                        h = info.picturH->ImageHeight * 16;

                        sceGifPkAddGsAD(&gifP, SCE_GS_TEX0_1, info.picturH->GsTex0);
                        sceGifPkAddGsAD(&gifP, SCE_GS_PRIM, 0x156);
                        
                        px = ((int)(bts_pp->xp * 16.0f) - (w) + 27648);
                        py = ((int)(bts_pp->yp * 16.0f) - (h / 2) + 30976);

                        sceGifPkAddGsAD(&gifP, SCE_GS_UV, SCE_GS_SET_UV(0, 0));

                        px &= 0xffff;
                        py &= 0xffff;
                        
                        sceGifPkAddGsAD(&gifP, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(px, py, 0));

                        px += w * 2;
                        py += h;
                        
                        sceGifPkAddGsAD(&gifP, SCE_GS_UV, SCE_GS_SET_UV(w, h));
                        sceGifPkAddGsAD(&gifP, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(px, py, 0));

                        if (bts_pp->use != 0)
                            bts_pp->endTime--;
                    }
                }

                CmnGifADPacketMakeTrans(&gifP);
                bthrow_ctrl[i].frame--;
            }
        }
    }
}

int BallThrowPollScene(void *para_pp, int frame, int first_f, int useDisp, int drDisp)
{
    ChangeDrawArea(DrawGetDrawEnvP(drDisp));
    BallThrowPoll();
    return 0;
}

static void MozaikuPollSceneInit(void)
{
    mozaiku_poll_str_current_pp = NULL;
}

static void MozaikuPollSceneReq(int id)
{
    if (id < 3u)
    {
        mozaiku_poll_str_current_pp = &mozaiku_poll_str[id];
        mozaiku_poll_str_current_pp->mozaiku_str_frame = 0;
    }
}

static int MozaikuPollScene(void *para_pp, int frame, int first_f, int useDisp, int drDisp)
{
    if (mozaiku_poll_str_current_pp == NULL)
        return 0;

    if (mozaiku_poll_str_current_pp->mozaiku_str_frame >= mozaiku_poll_str_current_pp->mozaiku_str_cnt)
    {
        mozaiku_poll_str_current_pp->mozaiku_str_frame = 0;
        mozaiku_poll_str_current_pp = NULL;
        return 0;
    }
    else
    {
        ChangeDrawArea(DrawGetDrawEnvP(drDisp));

        DrawMozaikuDisp(&mozaiku_poll_str_current_pp->mozaiku_str_pp[mozaiku_poll_str_current_pp->mozaiku_str_frame], frame, first_f, useDisp, drDisp);
        mozaiku_poll_str_current_pp->mozaiku_str_frame++;
        return 0;
    }
}

void DrawTmpBuffInit(void)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        tmp_buf_adrs[i] = NULL;
    }
}

void DrawTmpBuffQuit(void *adrs)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (tmp_buf_adrs[i] != NULL)
        {
            if (adrs == NULL || adrs == tmp_buf_adrs[i])
            {
                usrFree(tmp_buf_adrs[i]);
                tmp_buf_adrs[i] = NULL;
            }
        }
    }
}

#if 1
INCLUDE_ASM(const s32, "main/drawctrl", DrawTmpBuffGetArea);
void* DrawTmpBuffGetArea(/* a0 4 */ int size);
#else
void* DrawTmpBuffGetArea(/* a0 4 */ int size)
{
    /* v1 3 */ int   i;
    /* a1 5 */ void *ret;

    for (i = 0; i < 16; i++)
    {
        if (tmp_buf_adrs[i] == NULL)
        {
            ret = usrMalloc(size);
            tmp_buf_adrs[i] = ret;

            return ret;
        }
    }

    return NULL;
}
#endif

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393128);

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393140);

void DrawObjdatInit(int size, OBJDAT *od_pp, PR_SCENEHANDLE prf)
{
    int    i;
    void  *tmp_adr;
    float  frame;

    for (i = 0; i < size; i++, od_pp++)
    {
        tmp_adr = GetIntAdrsCurrent(od_pp->objdat_num);

        switch (od_pp->objdat_type)
        {
        case ODAT_SPA:
            od_pp->handle = PrInitializeAnimation(tmp_adr);
           
            frame = PrGetAnimationEndFrame(od_pp->handle);
            od_pp->maxfr = frame + frame;  
            break;
        case ODAT_SPM:
            od_pp->handle = PrInitializeModel(tmp_adr, prf);
            break;
        case ODAT_SPC:
            od_pp->handle = PrInitializeCamera(tmp_adr);

            frame = PrGetCameraEndFrame(od_pp->handle);
            od_pp->maxfr = frame + frame;
            break;
        case ODAT_SPF:
        case ODAT_TM2:
        case ODAT_CL2:
            od_pp->handle = tmp_adr;
            break;
        default:
            printf("Objdat Error!! type[%d]\n", od_pp->objdat_type);
        }
    }
}

// FIXME: Is this necessary?
const int drawctrl_rodata_padding[] = { 0, 0 };

void DrawObjdatReset(int size, OBJDAT *od_pp)
{
    int i;

    if (size > 0)
    {
        for (i = size; i != 0; i--, od_pp++)
        {
            GetIntAdrsCurrent(od_pp->objdat_num);

            switch (od_pp->objdat_type)
            {
            case ODAT_SPA:
                break;
            case ODAT_SPM:
                PrResetPosture(od_pp->handle);
                PrResetContour(od_pp->handle);
                PrHideModel(od_pp->handle);

                if (PrGetLinkedAnimation(od_pp->handle) != NULL)
                {
                    PrAnimateModel(od_pp->handle, 0);
                    PrUnlinkAnimation(od_pp->handle);
                }

                if (PrGetLinkedPositionAnimation(od_pp->handle) != NULL)
                {
                    PrUnlinkPositionAnimation(od_pp->handle);
                    PrAnimateModelPosition(od_pp->handle, 0);
                }

                break;
            case ODAT_SPC:
            case ODAT_SPF:
            case ODAT_TM2:
            case ODAT_CL2:
                break;
            default:
                printf("Objdat Error!! type[%d]\n", od_pp->objdat_type);
                break;
            }
        }
    }
}

void DrawSceneInit(sceGsDrawEnv1 *draw_env, SCENE_OBJDATA *scene_pp, int useDisp)
{
    int   i;
    u_int fbp_tmp;

    if (scene_pp->handle == NULL)
    {
        if (useDisp != 0)
            fbp_tmp = DrawGetFbpPos(useDisp);
        else
            fbp_tmp = -1;
        
        scene_pp->handle = PrInitializeScene(draw_env, scene_pp->usrName, fbp_tmp);
        DrawObjdatInit(scene_pp->objdat_size, scene_pp->objdat_pp, scene_pp->handle);

        for (i = 0; i < 3; i++)
        {
            scene_pp->objactprg_ctrl.objactprg[i] = NULL;
        }

        scene_pp->objactprg_ctrl.num = 0;

        PrPreprocessSceneModel(scene_pp->handle);
    }
}

static void DrawObjCtrlInit(OBJCTRL *oc_pp)
{
    oc_pp->PRflag = 0;
    oc_pp->PRdata = 0;
    oc_pp->PRtime = 0;
}

static void DrawObjStrInit(OBJSTR *os_pp)
{
    int i;

    for (i = 0; i < os_pp->size; i++)
    {
        DrawObjCtrlInit(&os_pp->objctrl_pp[i]);
    }

    os_pp->PRflag     = 0;
    os_pp->PRdata     = 0;
    os_pp->PRtime     = 0;
    os_pp->PRpress    = NULL;
    os_pp->PRtimeOld  = -1;

    os_pp->current_pp = NULL;
    os_pp->loop_time  = 0;
    os_pp->loop_pp    = NULL;
}

static void DrawObjTapStrInit(OBJTAPSTR *objtapstr_pp)
{
    objtapstr_pp->ovl_cnt_num = 0;
}

static void DrawObjStrReq(SCENE_OBJDATA *scn_pp, int num, u_int time)
{
    if (scn_pp->objstr_size > num)
    {
        scn_pp->objstr_pp[num].PRflag    = TRUE;
        scn_pp->objstr_pp[num].PRtime    = time;
        scn_pp->objstr_pp[num].PRtimeOld = -1;
    }
}

static void DrawObjStrTapReq(SCENE_OBJDATA *scn_pp, int num, u_int time, u_char *prs_adr)
{
    if (scn_pp->tapstr_size > num)
    {
        scn_pp->tapstr_pp[num].PRflag    = TRUE;
        scn_pp->tapstr_pp[num].PRtime    = 0;
        scn_pp->tapstr_pp[num].PRpress   = prs_adr;
        scn_pp->tapstr_pp[num].PRtimeOld = -1;
    }
}

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjTapStrTapReq);

void DrawObjStrTapQuit(SCENE_OBJDATA *scn_pp, int num, u_int time)
{
    if (num >= 0 && scn_pp->tapstr_size > num)
    {
        scn_pp->tapstr_pp[num].PRflag    = FALSE;
        scn_pp->tapstr_pp[num].PRtime    = 0;
        scn_pp->tapstr_pp[num].PRpress   = NULL;
        scn_pp->tapstr_pp[num].PRtimeOld = -1;
    }
}

void DrawObjTapStrTapQuit(SCENE_OBJDATA *scn_pp, int req_num)
{
    int     i;

    int     max       = scn_pp->objtapstr_pp[req_num].objtap_size;
    OBJTAP *objtap_pp = scn_pp->objtapstr_pp[req_num].objtap_pp;

    if (max > 0)
    {
        for (i = max; i != 0; i--, objtap_pp++)
        {
            if ((u_short)(objtap_pp->channel + 16) > 2)
            {
                DrawObjStrTapQuit(scn_pp, scn_pp->dr_tap_req[objtap_pp->channel].req_no, 0);
                scn_pp->dr_tap_req[objtap_pp->channel].req_no = -1;
            }
        }
    }
}

static void DrawObjStrKill(SCENE_OBJDATA *scn_pp, int num)
{
    if (scn_pp->objstr_size > num)
    {
        scn_pp->objstr_pp[num].PRflag    = FALSE;
        scn_pp->objstr_pp[num].PRtime    = 0;
        scn_pp->objstr_pp[num].PRpress   = NULL;
        scn_pp->objstr_pp[num].PRtimeOld = -1;
    }
}

#if 1
INCLUDE_ASM(const s32, "main/drawctrl", GetSpfTimeCtrl);
#else
static u_int GetSpfTimeCtrl(/* a0 4 */ OBJDAT *objdat_pp, /* v0 2 */ u_int frame) 
{
	/* a2 6 */ SPF_STR *spf_str_pp;
	/* v0 2 */ u_int max_cnt;
	/* v0 2 */ u_int ret;

    spf_str_pp = (SPF_STR*)objdat_pp->handle;
    max_cnt = (spf_str_pp->maxFrame * 2) - 2;

    if (max_cnt >= frame)
        max_cnt = frame;

    if (max_cnt & 1)
        ret = (spf_str_pp->frame[max_cnt / 2] + spf_str_pp->frame[(max_cnt / 2) + 1]) * 0.5f;
    else
        ret = spf_str_pp->frame[max_cnt / 2];

    return ret;
}
#endif

void camOtherKill(OBJACTPRG *objactprg_pp, int objactprg_num, int oya_num) 
{
    int i;

    for (i = 0; i < objactprg_num; i++, objactprg_pp++)
    {
        if (i != oya_num && objactprg_pp->job_type == OCTRL_CAM)
        {
            objactprg_pp->job_type = OCTRL_NON;
        }
    }
}

void posAniOtherKill(OBJACTPRG *objactprg_pp, int objactprg_num, int ani_num, int mod_num)
{
    int i;

    for (i = 0; i < objactprg_num; i++, objactprg_pp++)
    {
        if ((i != ani_num) && (objactprg_pp->job_type == OCTRL_ANIPOS)
        && (objactprg_pp->sub_num == mod_num))
        {
            objactprg_pp->job_type = OCTRL_NON;
        }
    }
}

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrDispTap);

void DrawSceneReset(SCENE_OBJDATA *scene_pp)
{
    int i;

    DrawObjdatReset(scene_pp->objdat_size, scene_pp->objdat_pp);

    for (i = 0; i < scene_pp->objstr_size; i++)
    {
        DrawObjStrInit(&scene_pp->objstr_pp[i]);
    }

    for (i = 0; i < scene_pp->tapstr_size; i++)
    {
        DrawObjStrInit(&scene_pp->tapstr_pp[i]);
    }

    for (i = 0; i < scene_pp->objtapstr_size; i++)
    {
        DrawObjTapStrInit(&scene_pp->objtapstr_pp[i]);
    }

    for (i = 0; i < 3; i++)
    {
        if (scene_pp->objactprg_ctrl.objactprg[i] != NULL)
        {
            DrawTmpBuffQuit(scene_pp->objactprg_ctrl.objactprg[i]);
            scene_pp->objactprg_ctrl.objactprg[i] = NULL;
        } 
    }

    for (i = 0; i < 40; i++)
    {
        scene_pp->dr_tap_req[i].req_no = -1;
    }
}

void DrawSceneFirstSet(SCENE_OBJDATA *scene_pp)
{
    int i;

    for (i = 0; i < 3; i++)
    {
        if (scene_pp->objactprg_ctrl.objactprg[i] == NULL)
        {
            scene_pp->objactprg_ctrl.objactprg[i] = DrawTmpBuffGetArea(scene_pp->objdat_size * 28);
        }

        WorkClear(scene_pp->objactprg_ctrl.objactprg[i], scene_pp->objdat_size * 28);
    }

    scene_pp->objactprg_ctrl.num = scene_pp->objdat_size;

    for (i = 0; i < 40; i++)
    {
        scene_pp->dr_tap_req[i].req_no = -1;
    }
}

INCLUDE_ASM(const s32, "main/drawctrl", Cl2MixTrans);

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393290);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjPrReq);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjStrTapTimeNext);

INCLUDE_ASM(const s32, "main/drawctrl", DrawObjTapCtrl);

INCLUDE_ASM(const s32, "main/drawctrl", DrawSceneObjData);
int DrawSceneObjData(/* s1 17 */ void *para_pp, /* s3 19 */ int frame, /* a2 6 */ int first_f, /* a3 7 */ int useDisp, /* s4 20 */ int drDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawDoubleDispIn);

INCLUDE_ASM(const s32, "main/drawctrl", DrawVramClear);

INCLUDE_ASM(const s32, "main/drawctrl", DrawMoveDispIn);

INCLUDE_ASM(const s32, "main/drawctrl", DrawAlphaBlendDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawMozaikuDisp);
int DrawMozaikuDisp(/* s2 18 */ void *para_pp, /* a1 5 */ int frame, /* a2 6 */ int first_f, /* a0 4 */ int useDisp, /* s0 16 */ int drDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawFadeDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawPlphaIndex8Disp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawTim2DIsp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawNoodlesDisp);

INCLUDE_ASM(const s32, "main/drawctrl", DrawVramLocalCopy);

INCLUDE_ASM(const s32, "main/drawctrl", DrawVramLocalCopy2);

INCLUDE_ASM(const s32, "main/drawctrl", drawUseDrDispCheckInit);
/* static */ void drawUseDrDispCheckInit(void);

INCLUDE_ASM(const s32, "main/drawctrl", drawDispCheckSub);

/* static */ int drawUseDispCheck(/* a0 4 */ int useD)
{
    return drawDispCheckSub(useD, &useDispFlag);
}

// INCLUDE_ASM(const s32, "main/drawctrl", drawDrDispCheck);
/* static */ int drawDrDispCheck(/* a0 4 */ int drD)
{
    return drawDispCheckSub(drD, &drDispFlag);
}

INCLUDE_ASM(const s32, "main/drawctrl", DrawScenectrlReq);
static int DrawScenectrlReq(/* s0 16 */ SCENECTRL *scenectrl_pp, /* s2 18 */ u_int time);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlInit);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlTitle);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlTitleDera);

INCLUDE_RODATA(const s32, "main/drawctrl", D_00393300);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrl);

INCLUDE_ASM(const s32, "main/drawctrl", mendRatioTitleGet);

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlTitleDisp);

void MendererReq(MEN_CTRL_ENUM menum)
{
    men_ctrl_enum = menum;
}

MEN_CTRL_ENUM GetMendererEnum(void)
{
    return men_ctrl_enum;
}

INCLUDE_ASM(const s32, "main/drawctrl", MendererCtrlScene);
int MendererCtrlScene(/* a0 4 */ void *para_pp, /* a1 5 */ int frame, /* a2 6 */ int first_f, /* a3 7 */ int useDisp, /* s0 16 */ int drDisp);

INCLUDE_ASM(const s32, "main/drawctrl", sceneConditionCheck);
int sceneConditionCheck(/* s2 18 */ u_int cond_flag);

static void DrawCtrlMain(void *x)
{
    int i;
    int j;
    int lsjkl;
    int check_cnt;
    SCENESTR *scenestr_pp;
    SCENECTRL *scenectrl_pp;
    int scene_req_flag;
    SCENECTRL *scenectrl_tmp;
    float men_tmp;
    
    BallThrowInit();
    MozaikuPollSceneInit();

    while (1)
    {
        scene_req_flag = 0;

        if (GlobalMendererUseCheck())
            PrCreateMendererTexture();

        if (drawCurrentLine >= drawEventrec->scenestr_size)
        {
            scenestr_pp  = NULL;
            scenectrl_pp = NULL;
        }
        else
        {
            scenestr_pp  = &drawEventrec->scenestr_pp[drawCurrentLine];
            scenectrl_pp = scenestr_pp->scenectrl_pp;
        }

        drawUseDrDispCheckInit();
        check_cnt = 0;

        if (global_data.play_step == PSTEP_XTR)
        {
            scenestr_pp  = NULL;
            scenectrl_pp = NULL;
            p3StrPoll(drawCurrentTime);
        }

        if (scenectrl_pp != NULL)
        {
            for (i = 0; i < 20; i++)
            {
                check_scenectrl[i] = NULL;
            }

            for (i = 0; i < scenestr_pp->scenectrl_num; i++, scenectrl_pp++)
            {
                if (scenectrl_pp->start_flame <= drawCurrentTime &&
                    scenectrl_pp->end_flame    > drawCurrentTime)
                {
                    if (sceneConditionCheck(scenectrl_pp->condition))
                    {
                        check_scenectrl[check_cnt] = scenectrl_pp;
                        check_cnt++;
                        continue;
                    }
                }

                DrawScenectrlReq(scenectrl_pp, 0x7fffffff);
            }
        }

        outsideDrawSceneReq(BallThrowPollScene, 200, DNUM_NON, DNUM_DRAW, NULL);
        outsideDrawSceneReq(MozaikuPollScene, 215, DNUM_DRAW, DNUM_DRAW, NULL);

        if (GlobalMendererUseCheck())
        {
            outsideDrawSceneReq(MendererCtrlScene, 210, DNUM_NON, DNUM_DRAW, &scene_req_flag);
        }

        if (global_data.play_step == PSTEP_VS || global_data.play_step == PSTEP_HOOK)
        {
            outsideDrawSceneReq(DrawVramClear, 229, DNUM_NON, DNUM_ZBUFF, &vclr_black_tmp);
        }

        while ((scenectrl_tmp = getOutsideCtrlScene(drawCurrentTime)) != NULL)
        {
            check_scenectrl[check_cnt] = scenectrl_tmp;
            check_cnt++;
        }

        for (i = 0; i < check_cnt - 1; i++)
        {
            for (j = i + 1; j < check_cnt; j++)
            {
                if (check_scenectrl[i]->pri > check_scenectrl[j]->pri)
                {
                    SCENECTRL *tmp = check_scenectrl[i];
                    check_scenectrl[i] = check_scenectrl[j];
                    check_scenectrl[j] = tmp;
                }
            }
        }

        for (lsjkl = 0; lsjkl < check_cnt; lsjkl++)
        {
            men_tmp = PrGetMendererRatio();

            if (check_scenectrl[lsjkl]->prg_pp == DrawSceneObjData)
            {
                if (check_scenectrl[lsjkl]->pri >= 211)
                    PrSetMendererRatio(0.0f);
                
                scene_req_flag = 1;
                DrawScenectrlReq(check_scenectrl[lsjkl], drawCurrentTime);
                PrSetMendererRatio(men_tmp);
            }
            else
            {
                if (!scene_req_flag)
                {
                    UsrPrSetScene();
                    scene_req_flag = 1;
                }

                DrawScenectrlReq(check_scenectrl[lsjkl], drawCurrentTime);
            }
        }

        dr_tap_req_num = 0;

        ChangeDrawArea(DrawGetDrawEnvP(DNUM_DRAW));
        UseGsRegSet();

        if (!scene_req_flag)
            UsrPrSetScene();

        outsideDrawSceneClear();
        MtcWait(1);
    }
}

static void DrawSceneStrInit(SCENESTR *scstr_pp)
{
    int i;

    for (i = 0; i < scstr_pp->scenectrl_num; i++)
    {
        scstr_pp->scenectrl_pp[i].use_flag = 0;

        if (scstr_pp->scenectrl_pp[i].prg_pp != NULL)
        {
            /* Call the overlay function pointer (DrawSceneObjData)
               to initialize the scenes and handles for each object */
            scstr_pp->scenectrl_pp[i].prg_pp(scstr_pp->scenectrl_pp[i].param_pp, 0, -2, scstr_pp->scenectrl_pp[i].useDisp, scstr_pp->scenectrl_pp[i].drDisp);
        }
    }
}

static void DrawSceneStrReset(SCENESTR *scstr_pp)
{
    int i;

    if (scstr_pp == NULL || scstr_pp->scenectrl_pp == NULL)
        return;

    for (i = 0; i < scstr_pp->scenectrl_num; i++)
    {
        if (scstr_pp->scenectrl_pp[i].use_flag != 0)
        {
            scstr_pp->scenectrl_pp[i].use_flag = 0;

            if (scstr_pp->scenectrl_pp[i].prg_pp != NULL)
            {
                /* Call the overlay function pointer (DrawSceneObjData)
                   to reset the objects */
                scstr_pp->scenectrl_pp[i].prg_pp(scstr_pp->scenectrl_pp[i].param_pp, 0, -1, 0, 0);
            }
        }
    }
}

void resetDrawSceneObjData(SCENESTR *scstr_pp)
{
    int            i;
    SCENE_OBJDATA *sod_pp;

    for (i = 0; i < scstr_pp->scenectrl_num; i++)
    {
        if ((scstr_pp->scenectrl_pp[i].prg_pp != NULL)
         && (scstr_pp->scenectrl_pp[i].prg_pp == DrawSceneObjData))
        {
            sod_pp = scstr_pp->scenectrl_pp[i].param_pp;
            sod_pp->handle = NULL;
        }
    }
}

void DrawCtrlInit(EVENTREC *ev_pp, int ctrlTbl, void *dat_top)
{
    int i;

    PrSetFrameRate(60.0f);
    DrawTmpBuffInit();

    for (i = 0; i < ev_pp->scenestr_size; i++)
    {
        resetDrawSceneObjData(&ev_pp->scenestr_pp[i]);
    }

    for (i = 0; i < ev_pp->scenestr_size; i++)
    {
        DrawSceneStrInit(&ev_pp->scenestr_pp[i]);
    }

    drawEventrec    = ev_pp;
    drawCurrentLine = ctrlTbl;

    drawCurrentTime    = 0;
    drawCurrentTimeOld = -1;
    dr_tap_req_num     = 0;

    outsideDrawSceneClear();
    MendererCtrlInit();
    
    MtcExec(DrawCtrlMain, MTC_TASK_DRAWCTRL);
}

void DrawCtrlQuit(void)
{
    int       i;
    EVENTREC *ev_pp = drawEventrec;

    DrawTmpBuffQuit(NULL);

    for (i = 0; i < ev_pp->scenestr_size; i++)
    {
        resetDrawSceneObjData(&ev_pp->scenestr_pp[i]);
    }

    if (global_data.play_step == PSTEP_XTR)
        p3StrQuitSd();

    MendererCtrlInit();

    PrSetMendererRatio(0.0f);
    PrCleanupModel(NULL);
    PrCleanupAnimation(NULL);
    PrCleanupCamera(NULL);
    PrCleanupScene(NULL);

    MtcKill(MTC_TASK_DRAWCTRL);
}

void DrawCtrlTimeSet(int time)
{
    if (drawCurrentTime == 0)
    {
        drawCurrentTime = time;
        return;
    }

    if (drawCurrentTime == time)
    {
        drawCurrentTime = time + 1;
        return;
    }

    drawCurrentTime = time;
}

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlTblChange);

INCLUDE_ASM(const s32, "main/drawctrl", DrawTapReqTbl);

int ddbg_scene_sel(int pad);

/* static */ int ddbg_event_sel(int pad)
{
    ddbg_event_num += pad;

    if (ddbg_event_num < 0)
        ddbg_event_num = drawEventrec->scenestr_size - 1;
    
    if (ddbg_event_num >= drawEventrec->scenestr_size)
        ddbg_event_num = 0;

    drawCurrentLine = ddbg_event_num;
    
    ddbg_scene_sel(0);
    return 0;
}

extern const char D_00399540[]; // .sdata - "#%02X"

/* static */ void ddbg_event_msg(char *buf)
{
    sprintf(buf, D_00399540, ddbg_event_num);
}

/* static */ int ddbg_scene_sel(int pad) 
{
	SCENESTR *scenestr_pp = &drawEventrec->scenestr_pp[ddbg_event_num];

    ddbg_scene_num += pad;

    if (ddbg_scene_num < 0)
        ddbg_scene_num = scenestr_pp->scenectrl_num - 1;
    
    if (ddbg_scene_num >= scenestr_pp->scenectrl_num)
        ddbg_scene_num = 0;

    return 0;
}

/* static */ void ddbg_scene_msg(char *buf)
{
    sprintf(buf, D_00399540, ddbg_scene_num);
}

/* static */ int ddbg_event_check(int pad)
{
    ddbg_go_event_scene = 0;
    return 1;
}

/* static */ int ddbg_scene_check(int pad)
{
    ddbg_go_event_scene = 1;
    return 1;
}

/* static */ int ddbg_bmp_check(int pad)
{
    ddbg_go_event_scene = 2;
    return 1;
}

static void ddbg_event_sub(void)
{
    int i;
    int j;
    int lsjkl;
    int check_cnt;
    SCENECTRL *scenectrl_pp;
    SCENESTR *scenestr_pp;

    if (drawEventrec->scenestr_pp[drawCurrentLine].scenectrl_pp == NULL)
        return;
    
    scenectrl_pp = drawEventrec->scenestr_pp[drawCurrentLine].scenectrl_pp;
    scenestr_pp = &drawEventrec->scenestr_pp[drawCurrentLine];

    check_cnt = 0;
    drawUseDrDispCheckInit();

    for (i = 0; i < 20; i++)
    {
        check_scenectrl[i] = NULL;
    }

    for (i = 0; i < scenestr_pp->scenectrl_num; i++, scenectrl_pp++)
    {
        if (scenectrl_pp->start_flame <= drawCurrentTime &&
            scenectrl_pp->end_flame    > drawCurrentTime)
        {
            check_scenectrl[check_cnt] = scenectrl_pp;
            check_cnt++;
        }
        else
        {
            DrawScenectrlReq(scenectrl_pp, drawCurrentTime);
        }
    }

    for (i = 0; i < check_cnt - 1; i++)
    {
        for (j = i + 1; j < check_cnt; j++)
        {
            if (check_scenectrl[i]->pri > check_scenectrl[j]->pri)
            {
                SCENECTRL *tmp = check_scenectrl[i];
                check_scenectrl[i] = check_scenectrl[j];
                check_scenectrl[j] = tmp;
            }
        }
    }

    for (lsjkl = 0; lsjkl < check_cnt; lsjkl++)
    {
        DrawScenectrlReq(check_scenectrl[lsjkl], drawCurrentTime);
    }

    dr_tap_req_num = 0;

    ChangeDrawArea(DrawGetDrawEnvP(DNUM_DRAW));
    UseGsRegSet();
}

INCLUDE_RODATA(const s32, "main/drawctrl", D_003933B0);

INCLUDE_RODATA(const s32, "main/drawctrl", D_003933C0);

INCLUDE_RODATA(const s32, "main/drawctrl", D_003933D0);

INCLUDE_RODATA(const s32, "main/drawctrl", D_003933E0);

static void ddbg_event_sub_bmp(void)
{
    int i;
    int j;
    int lsjkl;
    int check_cnt;
    SCENECTRL *scenectrl_pp;
    SCENESTR  *scenestr_pp;

    u_char    *dat1_pp;
    u_char    *dat2_pp;
    
    if (drawEventrec->scenestr_pp[drawCurrentLine].scenectrl_pp == NULL)
        return;

    scenectrl_pp = drawEventrec->scenestr_pp[drawCurrentLine].scenectrl_pp;
    scenestr_pp = &drawEventrec->scenestr_pp[drawCurrentLine];

    check_cnt = 0;
    drawUseDrDispCheckInit();

    drawCurrentTime = ddbg_bmp_frame * 2;

    for (i = 0; i < 20; i++)
    {
        check_scenectrl[i] = NULL;
    }

    for (i = 0; i < scenestr_pp->scenectrl_num; i++, scenectrl_pp++)
    {
        if (scenectrl_pp->start_flame <= drawCurrentTime &&
            scenectrl_pp->end_flame    > drawCurrentTime)
        {
            check_scenectrl[check_cnt] = scenectrl_pp;
            check_cnt++;
        }
        else
        {
            DrawScenectrlReq(scenectrl_pp, drawCurrentTime);
        }
    }

    for (i = 0; i < check_cnt - 1; i++)
    {
        for (j = i + 1; j < check_cnt; j++)
        {
            if (check_scenectrl[i]->pri > check_scenectrl[j]->pri)
            {
                SCENECTRL *tmp = check_scenectrl[i];
                check_scenectrl[i] = check_scenectrl[j];
                check_scenectrl[j] = tmp;
            }
        }
    }

    /* BUG: should use usrMalloc */
    dat1_pp = malloc(0x8C000); // ~573.4 KB
    dat2_pp = malloc(0x8C000);

    sceGsSetHalfOffset(&DBufDc.draw11,  2048, 2048, 0);
    sceGsSetHalfOffset2(&DBufDc.draw12, 2048, 2048, 0);
    sceGsSetHalfOffset(&DBufDc.draw01,  2048, 2048, 0);
    sceGsSetHalfOffset2(&DBufDc.draw02, 2048, 2048, 0);
    FlushCache(0);

    ChangeDrawArea(DrawGetDrawEnvP(DNUM_DRAW));

    for (lsjkl = 0; lsjkl < check_cnt; lsjkl++)
    {
        DrawScenectrlReq(check_scenectrl[lsjkl], drawCurrentTime);
    }

    VramTmpSave(dat1_pp, 640, 224, outbuf_idx ^ 1);

    {
        VCLR_PARA vclr_para = {};
        
        DrawVramClear(&vclr_para, 0, 0, DNUM_NON, DNUM_DRAW);
        DrawVramClear(&vclr_para, 0, 0, DNUM_NON, DNUM_VRAM2);
        DrawVramClear(&vclr_para, 0, 0, DNUM_NON, DNUM_ZBUFF);
    }

    sceGsSetHalfOffset(&DBufDc.draw11,  2048, 2048, 1);
    sceGsSetHalfOffset2(&DBufDc.draw12, 2048, 2048, 1);
    sceGsSetHalfOffset(&DBufDc.draw01,  2048, 2048, 1);
    sceGsSetHalfOffset2(&DBufDc.draw02, 2048, 2048, 1);
    FlushCache(0);

    ChangeDrawArea(DrawGetDrawEnvP(DNUM_DRAW));

    for (lsjkl = 0; lsjkl < check_cnt; lsjkl++)
    {
        DrawScenectrlReq(check_scenectrl[lsjkl], drawCurrentTime);
    }

    ChangeDrawArea(DrawGetDrawEnvP(DNUM_DRAW));

    VramTmpSave(dat2_pp, 640, 224, outbuf_idx ^ 1);

    {
        char save_name[20];

        sprintf(save_name, "scr%04d.bmp", ddbg_bmp_frame);
        VramTmpSaveOutBMP(save_name, 640, 224, outbuf_idx ^ 1, dat1_pp, dat2_pp);
    }

    usrFree(dat2_pp);
    usrFree(dat1_pp);
    
    dr_tap_req_num = 0;
    ChangeDrawArea(DrawGetDrawEnvP(DNUM_DRAW));
    UseGsRegSet();

    ddbg_bmp_frame++;
}

INCLUDE_ASM(const s32, "main/drawctrl", ddbg_tap_check);

static void ddbg_scene_sub(void)
{
    SCENECTRL *scenectrl_pp;

    if (drawEventrec->scenestr_pp[drawCurrentLine].scenectrl_pp == NULL)
        return;

    scenectrl_pp = &drawEventrec->scenestr_pp[drawCurrentLine].scenectrl_pp[ddbg_scene_num];

    drawUseDrDispCheckInit();

    if (drawCurrentTime < scenectrl_pp->start_flame)
        drawCurrentTime = scenectrl_pp->start_flame;

    if (drawCurrentTime >= scenectrl_pp->end_flame)
        drawCurrentTime = scenectrl_pp->end_flame - 1;

    DrawScenectrlReq(scenectrl_pp, drawCurrentTime);

    dr_tap_req_num = 0;
    ChangeDrawArea(DrawGetDrawEnvP(DNUM_DRAW));
    UseGsRegSet();
}

void ddbg_tctrl_sub(void)
{
    u_short paddata = pad[0].shot;
    u_short paddata_one = pad[0].one;

    if (paddata_one & SCE_PADRdown)
        ddbg_pause_f ^= 1;

    if (paddata_one & SCE_PADRup)
        drawCurrentTime = 0;

    if (!ddbg_pause_f)
    {
        drawCurrentTime += 1;
    }
    else
    {
        if (paddata & SCE_PADR1)
            drawCurrentTime++;

        if (paddata_one & SCE_PADR2)
            drawCurrentTime += 600;

        if (paddata & SCE_PADL1)
            drawCurrentTime--;

        if (paddata_one & SCE_PADL2)
            drawCurrentTime -= 600;

        if (paddata_one & SCE_PADRright)
            drawCurrentTime++;

        if (paddata_one & SCE_PADRleft)
            drawCurrentTime--;
    }

    if (drawCurrentTime < 0)
        drawCurrentTime = 0;
}

extern DRAW_DBG_STR draw_dbg_str[5];

INCLUDE_ASM(const s32, "main/drawctrl", DrawCtrlMainDebug);
/* static */ void DrawCtrlMainDebug(/* a0 4 */ void *x);
#if 0
{
    /* s0 16 */ u_short paddata;
    /* s2 18 */ int sel_pos;
    /* s3 19 */ int i;
    /* -0xc0(sp) */ char msg_buff[32];

    u_short y;

    while (1)
    {
        DbgMsgInit();

        while (1)
        {
            MtcWait(1);

            if (pad[0].one & SCE_PADLup)
                sel_pos--;

            if (pad[0].one & SCE_PADLdown)
                sel_pos++;

            if (sel_pos < 0)
                sel_pos = 4;
            if (sel_pos > 4)
                sel_pos = 0;

            if (pad[0].one & SCE_PADRright)
            {
                if (draw_dbg_str[sel_pos].prg_pp(1))
                {
                    ddbg_pause_f = 0;
                    break;
                }
            }

            if (pad[0].one & SCE_PADRleft)
            {
                if (draw_dbg_str[sel_pos].prg_pp(-1))
                {
                    ddbg_pause_f = 0;
                    break;
                }
            }

            DbgMsgClear();
            DbgMsgSetSize(32, 16);

            y = 1948;
            for (i = 0; i < 5; i++)
            {
                if (sel_pos == i)
                {
                    DbgMsgSetColor(128, 128, 0);
                }

                DbgMsgPrint((u_char*)draw_dbg_str[i].dbgmsg, 1770, y);

                if (draw_dbg_str[i].msg_pp != NULL)
                {
                    draw_dbg_str[i].msg_pp(msg_buff);
                    DbgMsgPrint((u_char*)msg_buff, 2150, y);
                }

                y += 20;
            }
            
            DbgMsgFlash();
        }

        drawCurrentTime = 0;
        ddbg_tap_num = 0;
        ddbg_bmp_frame = 0;

        while (1)
        {
            ddbg_tctrl_sub();

            if (pad[0].one & SCE_PADselect)
                break;

            /* Switch statement? */
            if (ddbg_go_event_scene == 1) {
                ddbg_tap_check();
                ddbg_scene_sub();
            }
            else if (ddbg_go_event_scene == 0) {
                ddbg_event_sub();
            }
            else {
                ddbg_event_sub_bmp();
            }

            MtcWait(1);
        };

        DrawSceneStrReset(&drawEventrec->scenestr_pp[drawCurrentLine]);
    }
}
#endif

void DrawCtrlInitDebug(EVENTREC *ev_pp, int ctrlTbl, void *dat_top)
{
    int i;

    PrSetFrameRate(60.0f);
    DrawTmpBuffInit();

    for (i = 0; i < ev_pp->scenestr_size; i++)
    {
        DrawSceneStrInit(&ev_pp->scenestr_pp[i]);
    }

    drawEventrec       = ev_pp;
    drawCurrentLine    = 0;
    drawCurrentTime    = 0;
    drawCurrentTimeOld = -1;

    dr_tap_req_num = 0;

    MtcExec(DrawCtrlMainDebug, MTC_TASK_DRAWCTRL);
}
