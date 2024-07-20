#ifndef SCENE_H
#define SCENE_H

#include <eetypes.h>
#include <eestruct.h>
#include <libgraph.h>

#include "database.h"

#include "model.h"

struct PrPERSPECTIVE_CAMERA { // 0x40
    /* 0x00 */ float position[4];
    /* 0x10 */ float interest[4];
    /* 0x20 */ float up[4];
    /* 0x30 */ float aspect;
    /* 0x34 */ float field_of_view;
    /* 0x38 */ float near_clip;
    /* 0x3c */ float far_clip;
};

class SpcFileHeader
{
public:
    char pad0[0x14];

    float m_endFrame;

    char m_name[32];

    char pad1[0x8];

    float m_position[4];
    float m_interest[4];

    u_int m_unk60;

    float m_fieldOfView;
    float m_aspect;
    float m_nearClip;
    float m_farClip;

    char pad2[0x10];

    int m_userData;

    char pad3[0x10];
};

class PrObjectDatabase;

class PrModelObject;

class PrSceneObject
{
public:
    void SelectCamera(SpcFileHeader *spcImage);
    PrPERSPECTIVE_CAMERA* GetCurrentCamera();
public:
    char pad[0x8];
    PrObjectDatabase* m_objectDatabase;
    u_int m_unk0C;
    PrPERSPECTIVE_CAMERA m_camera;
    sceGsFrame m_gsFrame;
    sceGsXyoffset m_xyOffset;

    PrModelObject* m_unk60_modelObject;
    PrModelObject* m_unk64_modelObject;

    u_int m_modelCount;
    
    SpcFileHeader* m_pCurrentCamera;

    sceGsDrawEnv1* m_pDrawEnv;

    u_int m_unk74;
    u_int m_unk78;
    float m_animTime;

    char* m_name;

    float m_fFocalLen;
    float m_fDefocusLen;
    u_int m_fDepthLevel;

    sceGsDBuff* m_pDBuff;

    u_int m_unk94;

    PrModelObject* m_unk98_modelObject;
    PrModelObject* m_unk9C_modelObject;
    PrModelObject* m_unkA0_modelObject;
};

#endif