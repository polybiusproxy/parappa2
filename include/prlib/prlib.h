#ifndef PRLIB_H
#define PRLIB_H

#include <eetypes.h>
#include <libgraph.h>

typedef enum
{
    false = 0,
    true = 1
} bool;

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
    /* 0x14 */ short unsigned int render_time0;
    /* 0x16 */ short unsigned int render_time1;
    /* 0x18 */ short unsigned int render_time2;
    /* 0x1a */ short unsigned int render_time3;
    /* 0x1c */ short unsigned int render_time4;
    /* 0x1e */ short unsigned int render_time5;
    /* 0x20 */ short unsigned int render_time6;
    /* 0x22 */ short unsigned int render_time7;
    /* 0x24 */ short unsigned int render_time8;
    /* 0x28 */ bool dynamic_append_transmit_node;
};

enum PrDEBUG_PARAM
{
    PR_FLOAT_PARAM_DISTURBANCE = 0,
    PR_DEBUG_PARAM_NUM = 1
};

PR_SCENEHANDLE PrInitializeScene(sceGsDrawEnv1 *drawEnv, char *name, u_int fbp);
void PrCleanupScene(PR_SCENEHANDLE scene);

void PrSetSceneEnv(PR_SCENEHANDLE model, sceGsDrawEnv1 *drawEnv);
void PrPreprocessSceneModel(PR_SCENEHANDLE scene);

PR_MODELHANDLE PrInitializeModel(PR_MODELHANDLE model, PR_SCENEHANDLE scene);
PR_ANIMATIONHANDLE PrInitializeAnimation(PR_ANIMATIONHANDLE animation);
PR_CAMERAHANDLE PrInitializeCamera(PR_CAMERAHANDLE camera);

void PrCleanupModel(PR_MODELHANDLE model);
void PrCleanupAnimation(PR_ANIMATIONHANDLE animation);
void PrCleanupCamera(PR_CAMERAHANDLE camera);

void PrLinkAnimation(PR_MODELHANDLE model, PR_ANIMATIONHANDLE animation);
void PrLinkPositionAnimation(PR_MODELHANDLE model, PR_ANIMATIONHANDLE animation);
void PrUnlinkPositionAnimation(PR_MODELHANDLE model);

void PrSelectCamera(PR_CAMERAHANDLE camera, PR_SCENEHANDLE scene);

void PrShowModel(PR_MODELHANDLE model, u_int unk);
void PrHideModel(PR_MODELHANDLE model);

void PrAnimateModel(PR_MODELHANDLE model, float time);
void PrAnimateModelPosition(PR_MODELHANDLE model, float time);
void PrAnimateSceneCamera(PR_SCENEHANDLE scene, float time);

void PrRender(PR_SCENEHANDLE scene);
void PrWaitRender(void);

#endif