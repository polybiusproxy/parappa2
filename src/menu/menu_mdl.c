#include "menu/menu_mdl.h"

#include <eetypes.h>
#include <eestruct.h>
#include <libgraph.h>
#include <libvu0.h>

#include <prlib/prlib.h>

#include <math.h>

/* data */
extern PRPROOT PRP_RootTbl[];

/* sbss - static */
u_int AMusicFitTime;

/* lit4 */
float D_00398F4C;
float D_00398F50;
float D_00398F54;
float D_00398F58;
float D_00398F5C;

void MNSceneMusicFitTimerClear(void)
{
    AMusicFitTime = 0;
}

void MNSceneMusicFitTimerFrame(void)
{
    AMusicFitTime++;
}

int MNSceneGetMusicFitTimer(void)
{
    return AMusicFitTime;
}

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_Init);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_End);

static void SetDrawEnv12(sceGsDrawEnv1 *pdenv)
{
    DRAWENV_TAG12 denvTag;

    if (pdenv != NULL)
    {
        ((u_long*)&denvTag.giftag)[0] = SCE_GIF_SET_TAG(8, 1, 0, SCE_GS_PRIM_POINT, 0, 1);
        ((u_long*)&denvTag.giftag)[1] = SCE_GIF_PACKED_AD;

        denvTag.denv1 = *pdenv;

        sceGsSyncPath(0, 0);
        FlushCache(0);

        sceGsPutDrawEnv(&denvTag.giftag);
        sceGsSyncPath(0, 0);

        denvTag.denv1.frame1addr = 0x4D;
        denvTag.denv1.zbuf1addr = 0x4F;
        denvTag.denv1.xyoffset1addr = 0x19;
        denvTag.denv1.scissor1addr = 0x41;
        denvTag.denv1.test1addr = 0x48;
        FlushCache(0);

        sceGsPutDrawEnv(&denvTag.giftag);
        sceGsSyncPath(0, 0);
    }
}

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_Draw);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_DispSw);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_SetAnimete);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_StartAnime);

void MNScene_ContinueAnime(MN_SCENE *pshdl, int no, MNANM_TBL *anime)
{
    if (anime == NULL)
    {
        if (no < 11u)
        {
            pshdl->cntani[no] = NULL;
        }
    }
    else
    {
        if (no < 0)
            no = anime->aTimNo;
        if (no > 9)
            no = 0;

        pshdl->speed[no]  = anime->aSpeed;
        pshdl->cntani[no] = anime;
    }
}

void MNScene_StopAnime(MN_SCENE *pshdl,int no)
{
    MNScene_StartAnime(pshdl, no, NULL);
    MNScene_ContinueAnime(pshdl, no, NULL);
}

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_ExecAnime);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_CopyState);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_CopyStateMdl);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_SetAnimeSpeed);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_SetAnimeEnd);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_SetAnimeBankEnd);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_isAnime);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_isAnimeBank);

INCLUDE_ASM(const s32, "menu/menu_mdl", MNScene_isSeniAnime);

int MNScene_ModelDispSw(MN_SCENE *pshdl, int nmdl, int bsw)
{
    int      ret;
    MN_HMDL *mdl;

    if (pshdl->nmdl <= nmdl)
        return 0;
    
    mdl = &pshdl->mdl[nmdl];

    if (mdl->spm == NULL)
        return 0;

    ret = mdl->dspSw;
    mdl->dspSw = bsw;

    if (bsw != 0)
    {
        PrShowModel(mdl->spm, NULL);
        return ret;
    }
    else
    {
        PrHideModel(mdl->spm);
        return ret;
    }
}

/* 
    Calculates the length of a vector
       - v0 -> Output vector
       - v1 -> Input vector
*/
static void _myVu0Length(float *v0, float *v1)
{
    asm __volatile__
    ("
        lqc2     vf04, 0x0(%1)     # load v1(vf04)
                                   # vf04 = v1

        vmul.xyz vf05, vf04, vf04  # vf05.xyz = (vf04.xyz)²
        vaddy.x  vf05, vf05, vf05y # vf05.x += vf05.y
        vaddz.x  vf05, vf05, vf05z # vf05.x += vf05.z

        vsqrt    Q, vf05x          # sqrt(vf05.x)
        vwaitq                     # wait for sqrt

        vaddq.x  vf05, vf00, Q     # vf05 = Q
        qmfc2    $8,   vf05        # $t0 = vf05

        sw       $8,   0x0(%0)     # save v0
                                   # v0 = $t0
    " :: "r"(v0), "r"(v1));
}

INCLUDE_ASM(const s32, "menu/menu_mdl", MnMoveMode_InitRoot);

INCLUDE_ASM(const s32, "menu/menu_mdl", _MnParMovRoot_GetPos);
static void _MnParMovRoot_GetPos(/* a0 4 */ PRPROOT *prt, /* f12 50 */ float rate, /* a1 5 */ float *pos);

#if 1
INCLUDE_ASM(const s32, "menu/menu_mdl", MnMoveModelPosition);
#else
static void MnMoveModelPosition(/* s2 18 */ void *spm, /* s0 16 */ int movNo, /* a2 6 */ int ttim, /* a3 7 */ int ttim0)
{
	/* f20 58 */ float rate;
	/* f22 60 */ float WRATE;
	/* f20 58 */ float fry;
	/* -0xb0(sp) */ sceVu0FVECTOR cpos;
	/* -0xa0(sp) */ sceVu0FMATRIX mt;

    WRATE = D_00398F54;

    fry = 1.0f;

    if (ttim < ttim0)
        fry = (float)ttim / (float)ttim0;       

    if (spm != NULL)
    {
        rate = sinf(fry * 0.5f * D_00398F58);

        if (movNo & 0x80)
            rate = 1.0f - rate;

        _MnParMovRoot_GetPos(&PRP_RootTbl[movNo], rate, cpos);

        fry = 0.0f;
        if (rate != 1.0f)
        {
            if (movNo & 0x80)
                cpos[3] += D_00398F58;

            fry = cpos[3];

            if (cpos[3] > D_00398F58)
                fry = cpos[3] - D_00398F5C;
        }
        
        sceVu0UnitMatrix(mt);
        sceVu0RotMatrixY(mt, mt, fry);

        mt[3][0] = cpos[0] * WRATE;
        mt[3][1] = cpos[1] * WRATE;
        mt[3][2] = cpos[2] * WRATE;
        mt[3][3] = WRATE;

        PrShowModel(spm, &mt);
    }
}
#endif
