#ifndef PRPRIV_H
#define PRPRIV_H

#include "database.h"
#include "renderstuff.h"

#include <eetypes.h>
#include <libgraph.h>
#include <libvu0.h>

#include "model.h"
#include "scene.h"
#include "animation.h"

extern PrObjectDatabase prObjectDatabase;
extern PrRenderStuff prRenderStuff;

enum PrDEBUG_PARAM
{
    PR_FLOAT_PARAM_DISTURBANCE = 0,
    PR_DEBUG_PARAM_NUM = 1
};

union PrDebugParam
{
    int   d;
    float f;
};

#ifdef __cplusplus
extern "C" {
#endif

/* model.cpp */
void PrSetPostureWorkArea(u_int areaTop, int areaSize);

/* prlib.cpp */
void  PrSetFrameRate(float frameRate);
float PrGetFrameRate(void);

void PrInitializeModule(sceGsZbuf zbuf);
void PrCleanupModule();

PrSceneObject* PrInitializeScene(sceGsDrawEnv1 *drawEnv, char *name, u_int fbp);
void PrInitializeSceneDBuff(sceGsDBuff *buff, char *name, u_int fbp);

void PrCleanupScene(PrSceneObject *scene);

void PrSetSceneFrame(PrSceneObject *scene, sceGsFrame frame);
void PrSetSceneEnv(PrSceneObject *scene, sceGsDrawEnv1 *drawEnv);

void PrPreprocessSceneModel(PrSceneObject *scene);

PrModelObject* PrInitializeModel(void* modelImage, PrSceneObject *scene);
SpaFileHeader* PrInitializeAnimation(SpaFileHeader *animImage);
SpcFileHeader* PrInitializeCamera(SpcFileHeader *cameraImage);

void PrCleanupModel(PrModelObject *model);
void PrCleanupAnimation(SpaFileHeader *animation);
void PrCleanupCamera(SpcFileHeader *camera);
void PrCleanupAllSceneModel(PrSceneObject *scene);

float PrGetAnimationStartFrame(SpaFileHeader *animation);
float PrGetAnimationEndFrame(SpaFileHeader *animation);

float PrGetCameraStartFrame(SpcFileHeader *camera);
float PrGetCameraEndFrame(SpcFileHeader *camera);

void PrSetModelUserData(PrModelObject *model, int userData);
void PrSetAnimationUserData(SpaFileHeader *animation, int userData);
void PrSetCameraUserData(SpcFileHeader *camera, int userData);

int PrGetModelUserData(PrModelObject *model);
int PrGetAnimationUserData(SpaFileHeader *animation);
int PrGetCameraUserData(SpcFileHeader *camera);

void PrLinkAnimation(PrModelObject *model, SpaFileHeader *animation);
void PrUnlinkAnimation(PrModelObject *model);

SpaFileHeader* PrGetLinkedAnimation(PrModelObject *model);

void PrLinkPositionAnimation(PrModelObject *model, SpaFileHeader *animation);
void PrUnlinkPositionAnimation(PrModelObject *model);

SpaFileHeader* PrGetLinkedPositionAnimation(PrModelObject *model);

void PrSelectCamera(SpcFileHeader *camera, PrSceneObject *scene);
SpcFileHeader* PrGetSelectedCamera(PrSceneObject *scene);

void* PrGetCurrentCamera(PrSceneObject *scene);

void PrSetDefaultCamera(PrSceneObject *scene, SpcFileHeader *camera);
void PrSetAppropriateDefaultCamera(PrSceneObject *scene);

void PrShowModel(PrModelObject *model, sceVu0FMATRIX *matrix);

NaMATRIX<float, 4, 4>* PrGetModelMatrix(PrModelObject *model);

void PrHideModel(PrModelObject *model);

NaVECTOR<float, 4>* PrGetModelPrimitivePosition(PrModelObject *model);
NaVECTOR<float, 4>* PrGetModelScreenPosition(PrModelObject *model);

void PrAnimateModel(PrModelObject *model, float time);
void PrAnimateModelPosition(PrModelObject *model, float time);
void PrAnimateSceneCamera(PrSceneObject *scene, float time);

void PrRender(PrSceneObject *scene);
void PrWaitRender(void);

void PrSetStage(int stage);

void PrSetDepthOfField(PrSceneObject *scene, float focalLen, float defocusLen);
void PrSetDepthOfFieldLevel(PrSceneObject *scene, u_int depthLevel);

float PrGetFocalLength(PrSceneObject *scene);
float PrGetDefocusLength(PrSceneObject *scene);

u_int PrGetDepthOfFieldLevel(PrSceneObject *scene);

void PrSaveContour(PrModelObject *model);
void PrResetContour(PrModelObject *model);

void PrSavePosture(PrModelObject *model);
void PrResetPosture(PrModelObject *model);

void PrSetContourBlurAlpha(PrModelObject *model, float blurAlpha, float blurAlpha2);

void PrSetTransactionBlendRatio(PrModelObject *model, float blendRatio);

float PrGetContourBlurAlpha(PrModelObject *model);
float PrGetContourBlurAlpha2(PrModelObject *model);

float PrGetTransactionBlendRatio(PrModelObject *model);

void PrSetModelDisturbance(PrModelObject *model, float disturbance);
float PrGetModelDisturbance(PrModelObject *model);

u_int PrGetVertexNum(PrModelObject *model);

char* PrGetModelName(PrModelObject *model);
char* PrGetAnimationName(SpaFileHeader *animation);
char* PrGetCameraName(SpcFileHeader *camera);
char* PrGetSceneName(PrSceneObject *scene);

void* PrGetRenderingStatistics();

void PrSetModelVisibillity(PrModelObject *model, u_int nodeIndex, u_int visible /* bool */);

SpmFileHeader* PrGetModelImage(PrModelObject *model);
SpaFileHeader* PrGetAnimationImage(SpaFileHeader *animation);
SpcFileHeader* PrGetCameraImage(SpcFileHeader *camera);

void PrSetDebugParam(int param, int value);
void PrSetDebugParamFloat(int param, float value);
int PrGetDebugParam(int param);
float PrGetDebugParamFloat(int param);

/* menderer.cpp */
void  PrDecelerateMenderer(u_int speed);
void  PrRestartMenderer();

void  PrSetMendererRatio(float ratio);
float PrGetMendererRatio();

void  PrSetMendererDirection(int direction);
int   PrGetMendererDirection();

void  PrSetMendererColorModulation(int color);
int   PrIsMendererColorModulation();

void  PrInitializeMenderer(u_int tbp, void *noodlePicture, u_int fbp);
void  PrRenderMenderer();

#ifdef __cplusplus
}
#endif

#endif