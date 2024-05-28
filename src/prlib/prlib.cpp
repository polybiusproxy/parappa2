#include <prlib/prlib.h>

#include <nalib/navector.h>
#include <nalib/namatrix.h>

#include "prpriv.h"

#include "random.h"
#include "model.h"

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

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupModule);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeScene);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeSceneDBuff);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupScene);

PR_EXTERN void PrSetSceneFrame() 
{
    // Empty
}

INCLUDE_ASM(const s32, "prlib/prlib", PrSetSceneEnv);

INCLUDE_ASM(const s32, "prlib/prlib", PrPreprocessSceneModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrInitializeCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrCleanupAllSceneModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetAnimationStartFrame);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetAnimationEndFrame);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetCameraStartFrame);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetCameraEndFrame);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetModelUserData);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetAnimationUserData);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetCameraUserData);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetModelUserData);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetAnimationUserData);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetCameraUserData);

INCLUDE_ASM(const s32, "prlib/prlib", PrLinkAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrUnlinkAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetLinkedAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrLinkPositionAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrUnlinkPositionAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetLinkedPositionAnimation);

INCLUDE_ASM(const s32, "prlib/prlib", PrSelectCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetSelectedCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetCurrentCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetDefaultCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetAppropriateDefaultCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrShowModel);

PR_EXTERN NaMATRIX<float, 4, 4>* PrGetModelMatrix(PrModelObject *model)
{
    if ((model->m_posture & 1) == 0)
        return NULL;

    return &model->m_matrix;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrHideModel);

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

INCLUDE_ASM(const s32, "prlib/prlib", PrAnimateModel);

INCLUDE_ASM(const s32, "prlib/prlib", PrAnimateModelPosition);

INCLUDE_ASM(const s32, "prlib/prlib", PrAnimateSceneCamera);

INCLUDE_ASM(const s32, "prlib/prlib", PrRender);

PR_EXTERN void PrWaitRender()
{
    prRenderStuff.WaitRender();
}

PR_EXTERN void PrSetStage(int stage)
{
    prCurrentStage = stage;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrSetDepthOfField);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetDepthOfFieldLevel);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetFocalLength);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetDefocusLength);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetDepthOfFieldLevel);

INCLUDE_ASM(const s32, "prlib/prlib", PrSaveContour);

INCLUDE_ASM(const s32, "prlib/prlib", PrResetContour);

INCLUDE_ASM(const s32, "prlib/prlib", PrSavePosture);

INCLUDE_ASM(const s32, "prlib/prlib", PrResetPosture);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetContourBlurAlpha);

INCLUDE_ASM(const s32, "prlib/prlib", PrSetTransactionBlendRatio);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetContourBlurAlpha);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetContourBlurAlpha2);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetTransactionBlendRatio);

PR_EXTERN void PrSetModelDisturbance(PrModelObject *model, float disturbance)
{
    model->m_disturbance = disturbance;
}

PR_EXTERN float PrGetModelDisturbance(PrModelObject *model)
{
    return model->m_disturbance;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrGetVertexNum);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetModelName);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetAnimationName);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetCameraName);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetSceneName);

PR_EXTERN PrRENDERING_STATISTICS* PrGetRenderingStatistics()
{
    PrRenderStuff* renderStuff = &prRenderStuff;
    return &renderStuff->m_renderStatistics;
}

INCLUDE_ASM(const s32, "prlib/prlib", PrSetModelVisibility);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetModelImage);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetAnimationImage);

INCLUDE_ASM(const s32, "prlib/prlib", PrGetCameraImage);

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
