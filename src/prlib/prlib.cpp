#include <prlib/prlib.h>

#include <nalib/navector.h>
#include <nalib/namatrix.h>

#include "prpriv.h"

#include "random.h"
#include "model.h"
#include "scene.h"
#include "animation.h"

/* sdata */
static float prFrameRate = 1.0f;
static float prInverseFrameRate = 1.0f;

/* sbss */
extern PrDebugParam debugParam[2];

static void InitializeDebugParam();

PR_EXTERN void PrSetFrameRate(float frameRate)
{
    prFrameRate = frameRate;
    prInverseFrameRate = 1.0f / frameRate;
}

PR_EXTERN float PrGetFrameRate()
{
    return prFrameRate;
}

PR_EXTERN void PrInitializeModule(sceGsZbuf zbuf)
{
    InitializeDebugParam();
    PrInitializeRandomPool();

    prObjectDatabase.Initialize();
    prRenderStuff.Initialize(zbuf);
}

PR_EXTERN void PrCleanupModule()
{
    prRenderStuff.Cleanup();
    
    PrCleanupModel(NULL);
    PrCleanupAnimation(NULL);
    PrCleanupCamera(NULL);
    PrCleanupScene(NULL);
    
    prObjectDatabase.Cleanup();
}

PR_EXTERN PR_SCENEHANDLE PrInitializeScene(sceGsDrawEnv1 *drawEnv, char *name, u_int fbp)
{
   return (PR_SCENEHANDLE)prObjectDatabase.CreateScene(drawEnv, name, fbp);
}

PR_EXTERN void PrInitializeSceneDBuff(sceGsDBuff *buff, char *name, u_int fbp)
{
    PrSceneObject* scene = prObjectDatabase.CreateScene(&buff->draw0, name, fbp);
    scene->m_pDBuff = buff;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupScene);

PR_EXTERN void PrSetSceneFrame(PR_SCENEHANDLE scene, sceGsFrame frame)
{
    // Empty
}

PR_EXTERN void PrSetSceneEnv(PR_SCENEHANDLE scene, sceGsDrawEnv1 *drawEnv)
{
    ((PrSceneObject*)scene)->m_gsFrame = drawEnv->frame1;
    ((PrSceneObject*)scene)->m_xyOffset = drawEnv->xyoffset1;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrPreprocessSceneModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupAllSceneModel);

PR_EXTERN float PrGetAnimationStartFrame(PR_ANIMATIONHANDLE animation)
{
    return 0.f;
}

PR_EXTERN float PrGetAnimationEndFrame(PR_ANIMATIONHANDLE animation)
{
    return ((SpaFileHeader*)animation)->m_endFrame * prFrameRate;
}

PR_EXTERN float PrGetCameraStartFrame(PR_CAMERAHANDLE camera)
{
    return 0.f;
}

PR_EXTERN float PrGetCameraEndFrame(PR_CAMERAHANDLE camera)
{
    return ((SpcFileHeader*)camera)->m_endFrame * prFrameRate;
}

PR_EXTERN void PrSetModelUserData(PR_MODELHANDLE model, int userData)
{
    ((PrModelObject*)model)->m_userData = userData;
}

PR_EXTERN void PrSetAnimationUserData(PR_ANIMATIONHANDLE animation, int userData)
{
    ((SpaFileHeader*)animation)->m_userData = userData;
}

PR_EXTERN void PrSetCameraUserData(PR_CAMERAHANDLE camera, int userData)
{
    ((SpcFileHeader*)camera)->m_userData = userData;
}

PR_EXTERN int PrGetModelUserData(PR_MODELHANDLE model)
{
    return ((PrModelObject*)model)->m_userData;
}

PR_EXTERN int PrGetAnimationUserData(PR_ANIMATIONHANDLE animation)
{
    return ((SpaFileHeader*)animation)->m_userData;
}

PR_EXTERN int PrGetCameraUserData(PR_CAMERAHANDLE camera)
{
    return ((SpcFileHeader*)camera)->m_userData;
}

PR_EXTERN void PrLinkAnimation(PR_MODELHANDLE model, PR_ANIMATIONHANDLE animation)
{
    ((PrModelObject*)model)->LinkAnimation((SpaFileHeader*)animation);
}

PR_EXTERN void PrUnlinkAnimation(PR_MODELHANDLE model)
{
    ((PrModelObject*)model)->LinkAnimation(NULL);
}

PR_EXTERN PR_ANIMATIONHANDLE PrGetLinkedAnimation(PR_MODELHANDLE model)
{
    return (PR_ANIMATIONHANDLE)((PrModelObject*)model)->m_linkedAnim;
}

PR_EXTERN void PrLinkPositionAnimation(PR_MODELHANDLE model, PR_ANIMATIONHANDLE animation)
{
    ((PrModelObject*)model)->LinkPositionAnimation((SpaFileHeader*)animation);
}

PR_EXTERN void PrUnlinkPositionAnimation(PR_MODELHANDLE model)
{
    ((PrModelObject*)model)->LinkPositionAnimation(NULL);
}

PR_EXTERN PR_ANIMATIONHANDLE PrGetLinkedPositionAnimation(PR_MODELHANDLE model)
{
    return (PR_ANIMATIONHANDLE)((PrModelObject*)model)->m_linkedPosAnim;
}

PR_EXTERN void PrSelectCamera(PR_CAMERAHANDLE camera, PR_SCENEHANDLE scene)
{
    ((PrSceneObject*)scene)->SelectCamera((SpcFileHeader*)camera);
}

PR_EXTERN PR_CAMERAHANDLE PrGetSelectedCamera(PR_SCENEHANDLE scene)
{
    return (PR_CAMERAHANDLE)((PrSceneObject*)scene)->m_pCurrentCamera;
}

PR_EXTERN void* PrGetCurrentCamera(PR_SCENEHANDLE scene)
{
    return ((PrSceneObject*)scene)->GetCurrentCamera();
}

INCLUDE_ASM(const s32, "prlib/prlib", PrSetDefaultCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetAppropriateDefaultCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrShowModel);

PR_EXTERN float* PrGetModelMatrix(PR_MODELHANDLE model)
{
    // Check LSB (show model flag)
    if ((((PrModelObject*)model)->m_posture & 1) == 0)
        return NULL;

    return *((PrModelObject*)model)->m_matrix.m;
}

PR_EXTERN void PrHideModel(PR_MODELHANDLE model)
{
    // Clear LSB (show model flag)
    ((PrModelObject*)model)->m_posture &= ~1u;
}

PR_EXTERN float* PrGetModelPrimitivePosition(PR_MODELHANDLE model)
{
    extern NaVECTOR<float, 4> vector_0;
    static int tmp_0 = 0;
    
    if (tmp_0 == 0)
        tmp_0 = 1;
    
    ((PrModelObject*)model)->GetPrimitivePosition(&vector_0);
    return vector_0.v;
}

PR_EXTERN float* PrGetModelScreenPosition(PR_MODELHANDLE model)
{
    extern NaVECTOR<float, 4> vector_1;
    static int tmp_1 = 0;
    
    if (tmp_1 == 0)
        tmp_1 = 1;

    ((PrModelObject*)model)->GetScreenPosition(&vector_1);
    return vector_1.v;
}

int prCurrentStage = 0;

PR_EXTERN void PrAnimateModel(PR_MODELHANDLE model, float time)
{
    ((PrModelObject*)model)->m_animTime = time * prInverseFrameRate;
}

PR_EXTERN void PrAnimateModelPosition(PR_MODELHANDLE model, float time)
{
    ((PrModelObject*)model)->m_animTimePosition = time * prInverseFrameRate;
}

PR_EXTERN void PrAnimateSceneCamera(PR_SCENEHANDLE scene, float time)
{
    ((PrSceneObject*)scene)->m_animTime = time * prInverseFrameRate;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrRender);

PR_EXTERN void PrWaitRender()
{
    prRenderStuff.WaitRender();
}

PR_EXTERN void PrSetStage(int stage)
{
    prCurrentStage = stage;
}

PR_EXTERN void PrSetDepthOfField(PR_MODELHANDLE scene, float focalLen, float defocusLen)
{
    if (focalLen != 0.f)
    {
        if (focalLen < 0.f || defocusLen <= focalLen)
        {
            defocusLen = 0.f;
            return;
        }
    }
    else
        defocusLen = 0.f;

    ((PrSceneObject*)scene)->m_fFocalLen = focalLen;
    ((PrSceneObject*)scene)->m_fDefocusLen = defocusLen;
}

PR_EXTERN void PrSetDepthOfFieldLevel(PR_SCENEHANDLE scene, u_int depthLevel)
{
    ((PrSceneObject*)scene)->m_fDepthLevel = depthLevel;
}

PR_EXTERN float PrGetFocalLength(PR_SCENEHANDLE scene)
{
    return ((PrSceneObject*)scene)->m_fFocalLen;
}

PR_EXTERN float PrGetDefocusLength(PR_SCENEHANDLE scene)
{
    return ((PrSceneObject*)scene)->m_fDefocusLen;
}

PR_EXTERN u_int PrGetDepthOfFieldLevel(PR_SCENEHANDLE scene)
{
    return ((PrSceneObject*)scene)->m_fDepthLevel;
}

PR_EXTERN void PrSaveContour(PR_MODELHANDLE model)
{
    ((PrModelObject*)model)->SaveContour();
}
PR_EXTERN void PrResetContour(PR_MODELHANDLE model)
{
    ((PrModelObject*)model)->ResetContour();
}

PR_EXTERN void PrSavePosture(PR_MODELHANDLE model)
{
    ((PrModelObject*)model)->SavePosture();
}
PR_EXTERN void PrResetPosture(PR_MODELHANDLE model)
{
    ((PrModelObject*)model)->ResetPosture();
}

PR_EXTERN void PrSetContourBlurAlpha(PR_MODELHANDLE model, float blurAlpha, float blurAlpha2)
{
    ((PrModelObject*)model)->m_contourBlurAlpha = blurAlpha;
    ((PrModelObject*)model)->m_contourBlurAlpha2 = blurAlpha2;
}

PR_EXTERN void PrSetTransactionBlendRatio(PR_MODELHANDLE model, float blendRatio)
{
    ((PrModelObject*)model)->m_transactionBlendRatio = blendRatio;
}

PR_EXTERN float PrGetContourBlurAlpha(PR_MODELHANDLE model)
{
    return ((PrModelObject*)model)->m_contourBlurAlpha;
}

PR_EXTERN float PrGetContourBlurAlpha2(PR_MODELHANDLE model)
{
    return ((PrModelObject*)model)->m_contourBlurAlpha2;
}

PR_EXTERN float PrGetTransactionBlendRatio(PR_MODELHANDLE model)
{
    return ((PrModelObject*)model)->m_transactionBlendRatio;
}

PR_EXTERN void PrSetModelDisturbance(PR_MODELHANDLE model, float disturbance)
{
    ((PrModelObject*)model)->m_disturbance = disturbance;
}

PR_EXTERN float PrGetModelDisturbance(PR_MODELHANDLE model)
{
    return ((PrModelObject*)model)->m_disturbance;
}

PR_EXTERN u_int PrGetVertexNum(PR_MODELHANDLE model)
{
    return ((PrModelObject*)model)->m_spmImage->m_vertexNum;
}

PR_EXTERN char* PrGetModelName(PR_MODELHANDLE model)
{
    return ((PrModelObject*)model)->m_spmImage->m_name;
}

PR_EXTERN char* PrGetAnimationName(PR_ANIMATIONHANDLE animation)
{
    return ((SpaFileHeader*)animation)->m_name;
}

PR_EXTERN char* PrGetCameraName(PR_CAMERAHANDLE camera)
{
    return ((SpcFileHeader*)camera)->m_name;
}

PR_EXTERN char* PrGetSceneName(PR_SCENEHANDLE scene)
{
    return ((PrSceneObject*)scene)->m_name;
}

PR_EXTERN void* PrGetRenderingStatistics()
{
    PrRenderStuff* renderStuff = &prRenderStuff;
    return &renderStuff->m_renderStatistics;
}

PR_EXTERN void PrSetModelVisibillity(PR_MODELHANDLE model, u_int nodeIndex, u_int visible)
{
    SpmNode* spmNode;

    if (nodeIndex < ((PrModelObject*)model)->m_spmImage->m_nodeNum) {
        spmNode = ((PrModelObject*)model)->m_spmImage->m_nodes[nodeIndex];

        if (visible)
            spmNode->m_flags &= ~0x20000u;
        else
            spmNode->m_flags |= 0x20000u;
    }
}

PR_EXTERN void* PrGetModelImage(PR_MODELHANDLE model)
{
    return (void*)((PrModelObject*)model)->m_spmImage;
}

PR_EXTERN void* PrGetAnimationImage(PR_ANIMATIONHANDLE animation)
{
    return (void*)animation; // SpaFileHeader
}

PR_EXTERN void* PrGetCameraImage(PR_CAMERAHANDLE camera)
{
    return (void*)camera; // SpcFileHeader
}

PR_EXTERN void PrSetDebugParam(int param, int value)
{
    debugParam[param].d = value;
}

PR_EXTERN void PrSetDebugParamFloat(int param, float value)
{
    debugParam[param].d = *(int*)&value;
}

PR_EXTERN int PrGetDebugParam(int param)
{
    return debugParam[param].d;
}

PR_EXTERN float PrGetDebugParamFloat(int param)
{
    return debugParam[param].f;
}

static void InitializeDebugParam()
{
    PrSetDebugParamFloat(PR_FLOAT_PARAM_DISTURBANCE, 1.0f);
}
