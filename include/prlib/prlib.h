#ifndef PRLIB_H
#define PRLIB_H

#include "common.h"

#include <eetypes.h>
#include <libgraph.h>
#include <libvu0.h>

typedef void *PR_MODELHANDLE;
typedef void *PR_ANIMATIONHANDLE;
typedef void *PR_CAMERAHANDLE;
typedef void *PR_SCENEHANDLE;
typedef float PR_FRAME;

struct PrPERSPECTIVE_CAMERA { // 0x40
    /* 0x00 */ float position[4];
    /* 0x10 */ float interest[4];
    /* 0x20 */ float up[4];
    /* 0x30 */ float aspect;
    /* 0x34 */ float field_of_view;
    /* 0x38 */ float near_clip;
    /* 0x3c */ float far_clip;
};

struct PrRENDERING_STATISTICS { // 0x2c
    /* 0x00 */ unsigned int node_num;
    /* 0x04 */ unsigned int opaque_context1_node_num;
    /* 0x08 */ unsigned int transmit_context1_node_num;
    /* 0x0c */ unsigned int opaque_context2_node_num;
    /* 0x10 */ unsigned int transmit_context2_node_num;
    /* 0x14 */ unsigned short render_time0;
    /* 0x16 */ unsigned short render_time1;
    /* 0x18 */ unsigned short render_time2;
    /* 0x1a */ unsigned short render_time3;
    /* 0x1c */ unsigned short render_time4;
    /* 0x1e */ unsigned short render_time5;
    /* 0x20 */ unsigned short render_time6;
    /* 0x22 */ unsigned short render_time7;
    /* 0x24 */ unsigned short render_time8;
    /* 0x28 */ int dynamic_append_transmit_node; /* bool */
};

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

PR_SCENEHANDLE PrInitializeScene(sceGsDrawEnv1 *drawEnv, char *name, u_int fbp);
void PrInitializeSceneDBuff(sceGsDBuff *buff, char *name, u_int fbp);

void PrCleanupScene(PR_SCENEHANDLE scene);

void PrSetSceneFrame(PR_SCENEHANDLE scene, sceGsFrame frame);
void PrSetSceneEnv(PR_SCENEHANDLE scene, sceGsDrawEnv1 *drawEnv);

void PrPreprocessSceneModel(PR_SCENEHANDLE scene);

PR_MODELHANDLE PrInitializeModel(void* modelImage, PR_SCENEHANDLE scene);
PR_ANIMATIONHANDLE PrInitializeAnimation(PR_ANIMATIONHANDLE animImage);
PR_CAMERAHANDLE PrInitializeCamera(PR_CAMERAHANDLE cameraImage);

void PrCleanupModel(PR_MODELHANDLE model);
void PrCleanupAnimation(PR_ANIMATIONHANDLE animation);
void PrCleanupCamera(PR_CAMERAHANDLE camera);
void PrCleanupAllSceneModel(PR_SCENEHANDLE scene);

float PrGetAnimationStartFrame(PR_ANIMATIONHANDLE animation);
float PrGetAnimationEndFrame(PR_ANIMATIONHANDLE animation);

float PrGetCameraStartFrame(PR_CAMERAHANDLE camera);
float PrGetCameraEndFrame(PR_CAMERAHANDLE camera);

void PrSetModelUserData(PR_MODELHANDLE model, int userData);
void PrSetAnimationUserData(PR_ANIMATIONHANDLE animation, int userData);
void PrSetCameraUserData(PR_CAMERAHANDLE camera, int userData);

int PrGetModelUserData(PR_MODELHANDLE model);
int PrGetAnimationUserData(PR_ANIMATIONHANDLE animation);
int PrGetCameraUserData(PR_CAMERAHANDLE camera);

void PrLinkAnimation(PR_MODELHANDLE model, PR_ANIMATIONHANDLE animation);
void PrUnlinkAnimation(PR_MODELHANDLE model);

PR_ANIMATIONHANDLE PrGetLinkedAnimation(PR_MODELHANDLE model);

void PrLinkPositionAnimation(PR_MODELHANDLE model, PR_ANIMATIONHANDLE animation);
void PrUnlinkPositionAnimation(PR_MODELHANDLE model);

PR_ANIMATIONHANDLE PrGetLinkedPositionAnimation(PR_MODELHANDLE model);

void PrSelectCamera(PR_CAMERAHANDLE camera, PR_SCENEHANDLE scene);
PR_CAMERAHANDLE PrGetSelectedCamera(PR_SCENEHANDLE scene);

void* PrGetCurrentCamera(PR_SCENEHANDLE scene);

void PrSetDefaultCamera(PR_SCENEHANDLE scene, PR_CAMERAHANDLE camera);
void PrSetAppropriateDefaultCamera(PR_SCENEHANDLE scene);

void PrShowModel(PR_MODELHANDLE model, sceVu0FMATRIX *matrix);

float* PrGetModelMatrix(PR_MODELHANDLE model);

void PrHideModel(PR_MODELHANDLE model);

float* PrGetModelPrimitivePosition(PR_MODELHANDLE model);
float* PrGetModelScreenPosition(PR_MODELHANDLE model);

void PrAnimateModel(PR_MODELHANDLE model, float time);
void PrAnimateModelPosition(PR_MODELHANDLE model, float time);
void PrAnimateSceneCamera(PR_SCENEHANDLE scene, float time);

void PrRender(PR_SCENEHANDLE scene);
void PrWaitRender(void);

void PrSetStage(int stage);

void PrSetDepthOfField(PR_MODELHANDLE scene, float focalLen, float defocusLen);
void PrSetDepthOfFieldLevel(PR_SCENEHANDLE scene, u_int depthLevel);

float PrGetFocalLength(PR_SCENEHANDLE scene);
float PrGetDefocusLength(PR_SCENEHANDLE scene);

u_int PrGetDepthOfFieldLevel(PR_SCENEHANDLE scene);

void PrSaveContour(PR_MODELHANDLE model);
void PrResetContour(PR_MODELHANDLE model);

void PrSavePosture(PR_MODELHANDLE model);
void PrResetPosture(PR_MODELHANDLE model);

void PrSetContourBlurAlpha(PR_MODELHANDLE model, float blurAlpha, float blurAlpha2);

void PrSetTransactionBlendRatio(PR_MODELHANDLE model, float blendRatio);

float PrGetContourBlurAlpha(PR_MODELHANDLE model);
float PrGetContourBlurAlpha2(PR_MODELHANDLE model);

float PrGetTransactionBlendRatio(PR_MODELHANDLE model);

void PrSetModelDisturbance(PR_MODELHANDLE model, float disturbance);
float PrGetModelDisturbance(PR_MODELHANDLE model);

u_int PrGetVertexNum(PR_MODELHANDLE model);

char* PrGetModelName(PR_MODELHANDLE model);
char* PrGetAnimationName(PR_ANIMATIONHANDLE animation);
char* PrGetCameraName(PR_CAMERAHANDLE camera);
char* PrGetSceneName(PR_SCENEHANDLE scene);

void* PrGetRenderingStatistics();

void PrSetModelVisibillity(PR_MODELHANDLE model, u_int nodeIndex, u_int visible);

void* PrGetModelImage(PR_MODELHANDLE model);
void* PrGetAnimationImage(PR_ANIMATIONHANDLE animation);
void* PrGetCameraImage(PR_CAMERAHANDLE camera);

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