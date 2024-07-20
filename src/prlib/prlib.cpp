#include <nalib/navector.h>
#include <nalib/namatrix.h>

#include "prpriv.h"

#include "random.h"
#include "model.h"
#include "scene.h"
#include "animation.h"

#include "common.h"

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

PR_EXTERN PrSceneObject* PrInitializeScene(sceGsDrawEnv1 *drawEnv, char *name, u_int fbp)
{
   return prObjectDatabase.CreateScene(drawEnv, name, fbp);
}

PR_EXTERN void PrInitializeSceneDBuff(sceGsDBuff *buff, char *name, u_int fbp)
{
    PrSceneObject* scene = prObjectDatabase.CreateScene(&buff->draw0, name, fbp);
    scene->m_pDBuff = buff;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupScene);

PR_EXTERN void PrSetSceneFrame(PrSceneObject *scene, sceGsFrame frame)
{
    // Empty
}

PR_EXTERN void PrSetSceneEnv(PrSceneObject *scene, sceGsDrawEnv1 *drawEnv)
{
    scene->m_gsFrame = drawEnv->frame1;
    scene->m_xyOffset = drawEnv->xyoffset1;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrPreprocessSceneModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupAllSceneModel);

PR_EXTERN float PrGetAnimationStartFrame(SpaFileHeader *animation)
{
    return 0.f;
}

PR_EXTERN float PrGetAnimationEndFrame(SpaFileHeader *animation)
{
    return animation->m_endFrame * prFrameRate;
}

PR_EXTERN float PrGetCameraStartFrame(SpcFileHeader *camera)
{
    return 0.f;
}

PR_EXTERN float PrGetCameraEndFrame(SpcFileHeader *camera)
{
    return camera->m_endFrame * prFrameRate;
}

PR_EXTERN void PrSetModelUserData(PrModelObject *model, int userData)
{
    model->m_userData = userData;
}

PR_EXTERN void PrSetAnimationUserData(SpaFileHeader *animation, int userData)
{
    animation->m_userData = userData;
}

PR_EXTERN void PrSetCameraUserData(SpcFileHeader *camera, int userData)
{
    camera->m_userData = userData;
}

PR_EXTERN int PrGetModelUserData(PrModelObject *model)
{
    return model->m_userData;
}

PR_EXTERN int PrGetAnimationUserData(SpaFileHeader *animation)
{
    return animation->m_userData;
}

PR_EXTERN int PrGetCameraUserData(SpcFileHeader *camera)
{
    return camera->m_userData;
}

PR_EXTERN void PrLinkAnimation(PrModelObject *model, SpaFileHeader *animation)
{
    model->LinkAnimation(animation);
}

PR_EXTERN void PrUnlinkAnimation(PrModelObject *model)
{
    model->LinkAnimation(NULL);
}

PR_EXTERN SpaFileHeader* PrGetLinkedAnimation(PrModelObject *model)
{
    return model->m_linkedAnim;
}

PR_EXTERN void PrLinkPositionAnimation(PrModelObject *model, SpaFileHeader *animation)
{
    model->LinkPositionAnimation(animation);
}

PR_EXTERN void PrUnlinkPositionAnimation(PrModelObject *model)
{
    model->LinkPositionAnimation(NULL);
}

PR_EXTERN SpaFileHeader* PrGetLinkedPositionAnimation(PrModelObject *model)
{
    return model->m_linkedPosAnim;
}

PR_EXTERN void PrSelectCamera(SpcFileHeader *camera, PrSceneObject *scene)
{
    scene->SelectCamera((SpcFileHeader*)camera);
}

PR_EXTERN SpcFileHeader* PrGetSelectedCamera(PrSceneObject *scene)
{
    return scene->m_pCurrentCamera;
}

PR_EXTERN void* PrGetCurrentCamera(PrSceneObject *scene)
{
    return scene->GetCurrentCamera();
}

INCLUDE_ASM(const s32, "prlib/prlib", PrSetDefaultCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetAppropriateDefaultCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrShowModel);

PR_EXTERN NaMATRIX<float, 4, 4>* PrGetModelMatrix(PrModelObject *model)
{
    // Check LSB (show model flag)
    if ((model->m_posture & 1) == 0)
        return NULL;

    return &model->m_matrix;
}

PR_EXTERN void PrHideModel(PrModelObject *model)
{
    // Clear LSB (show model flag)
    model->m_posture &= ~1u;
}

PR_EXTERN NaVECTOR<float, 4>* PrGetModelPrimitivePosition(PrModelObject *model)
{
    extern NaVECTOR<float, 4> vector_0;
    static int tmp_0 = 0;
    
    if (tmp_0 == 0)
        tmp_0 = 1;
    
    model->GetPrimitivePosition(&vector_0);
    return &vector_0;
}

PR_EXTERN NaVECTOR<float, 4>* PrGetModelScreenPosition(PrModelObject *model)
{
    extern NaVECTOR<float, 4> vector_1;
    static int tmp_1 = 0;
    
    if (tmp_1 == 0)
        tmp_1 = 1;

    model->GetScreenPosition(&vector_1);
    return &vector_1;
}

int prCurrentStage = 0;

PR_EXTERN void PrAnimateModel(PrModelObject *model, float time)
{
    model->m_animTime = time * prInverseFrameRate;
}

PR_EXTERN void PrAnimateModelPosition(PrModelObject *model, float time)
{
    model->m_animTimePosition = time * prInverseFrameRate;
}

PR_EXTERN void PrAnimateSceneCamera(PrSceneObject *scene, float time)
{
    scene->m_animTime = time * prInverseFrameRate;
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

PR_EXTERN void PrSetDepthOfField(PrSceneObject *scene, float focalLen, float defocusLen)
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
    {
        defocusLen = 0.f;
    }

    scene->m_fFocalLen = focalLen;
    scene->m_fDefocusLen = defocusLen;
}

PR_EXTERN void PrSetDepthOfFieldLevel(PrSceneObject *scene, u_int depthLevel)
{
    scene->m_fDepthLevel = depthLevel;
}

PR_EXTERN float PrGetFocalLength(PrSceneObject *scene)
{
    return scene->m_fFocalLen;
}

PR_EXTERN float PrGetDefocusLength(PrSceneObject *scene)
{
    return scene->m_fDefocusLen;
}

PR_EXTERN u_int PrGetDepthOfFieldLevel(PrSceneObject *scene)
{
    return scene->m_fDepthLevel;
}

PR_EXTERN void PrSaveContour(PrModelObject *model)
{
    model->SaveContour();
}
PR_EXTERN void PrResetContour(PrModelObject *model)
{
    model->ResetContour();
}

PR_EXTERN void PrSavePosture(PrModelObject *model)
{
    model->SavePosture();
}
PR_EXTERN void PrResetPosture(PrModelObject *model)
{
    model->ResetPosture();
}

PR_EXTERN void PrSetContourBlurAlpha(PrModelObject *model, float blurAlpha, float blurAlpha2)
{
    model->m_contourBlurAlpha = blurAlpha;
    model->m_contourBlurAlpha2 = blurAlpha2;
}

PR_EXTERN void PrSetTransactionBlendRatio(PrModelObject *model, float blendRatio)
{
    model->m_transactionBlendRatio = blendRatio;
}

PR_EXTERN float PrGetContourBlurAlpha(PrModelObject *model)
{
    return model->m_contourBlurAlpha;
}

PR_EXTERN float PrGetContourBlurAlpha2(PrModelObject *model)
{
    return model->m_contourBlurAlpha2;
}

PR_EXTERN float PrGetTransactionBlendRatio(PrModelObject *model)
{
    return model->m_transactionBlendRatio;
}

PR_EXTERN void PrSetModelDisturbance(PrModelObject *model, float disturbance)
{
    model->m_disturbance = disturbance;
}

PR_EXTERN float PrGetModelDisturbance(PrModelObject *model)
{
    return model->m_disturbance;
}

PR_EXTERN u_int PrGetVertexNum(PrModelObject *model)
{
    return model->m_spmImage->m_vertexNum;
}

PR_EXTERN char* PrGetModelName(PrModelObject *model)
{
    return model->m_spmImage->m_name;
}

PR_EXTERN char* PrGetAnimationName(SpaFileHeader *animation)
{
    return animation->m_name;
}

PR_EXTERN char* PrGetCameraName(SpcFileHeader *camera)
{
    return camera->m_name;
}

PR_EXTERN char* PrGetSceneName(PrSceneObject *scene)
{
    return scene->m_name;
}

PR_EXTERN void* PrGetRenderingStatistics()
{
    PrRenderStuff* renderStuff = &prRenderStuff;
    return &renderStuff->m_renderStatistics;
}

PR_EXTERN void PrSetModelVisibillity(PrModelObject *model, u_int nodeIndex, u_int visible /* bool */)
{
    SpmNode* spmNode;

    if (nodeIndex < model->m_spmImage->m_nodeNum)
    {
        spmNode = model->m_spmImage->m_nodes[nodeIndex];

        if (visible)
            spmNode->m_flags &= ~0x20000u;
        else
            spmNode->m_flags |= 0x20000u;
    }
}

PR_EXTERN SpmFileHeader* PrGetModelImage(PrModelObject *model)
{
    return model->m_spmImage;
}

PR_EXTERN SpaFileHeader* PrGetAnimationImage(SpaFileHeader *animation)
{
    return animation;
}

PR_EXTERN SpcFileHeader* PrGetCameraImage(SpcFileHeader *camera)
{
    return camera;
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
