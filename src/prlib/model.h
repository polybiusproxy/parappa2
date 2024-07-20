#ifndef MODEL_H
#define MODEL_H

#include <eetypes.h>

#include <nalib/navector.h>
#include <nalib/namatrix.h>

#include "scene.h"

#include "animation.h"

class SpmFileHeader;

class SpmNode
{
public:
    NaMATRIX<float, 4, 4> mScaleMatrix;
    NaMATRIX<float, 4, 4> unk40_renderMatrix;
    NaMATRIX<float, 4, 4> unk80_matrix;
    NaMATRIX<float, 4, 4> unkC0_matrix;
    NaMATRIX<float, 4, 4> unk100_matrix;
    NaVECTOR<float, 4> unk140_vec4;

    u_int m_index;
    u_int m_flags;

    SpmFileHeader *unk158_spmHeader;

    SpmNode* unk15C_siblingNode;
    SpmNode* unk160_parentNode;
    SpmNode* unk164_previousNode;

    u_int m_triangleCount;

    void* m_opaqueNodePacket;
    void* m_transmitNodePacket;

    char pad[0x48];
};

class PrModelObject;

class SpmFileHeader
{
public:
    u_int m_magic;
    u_short m_version;
    u_short m_flags;
    char pad[8];
    char m_name[32];
    float m_unk30_vec4[4];
    float m_unk40_vec4[4];
    PrModelObject* m_pLinkedModel;
    u_int m_meshCount;
    int m_unk58;
    u_int m_vertexNum;
    float m_currentFrame;
    float m_currentPosFrame;
    u_int m_nodeNum;
    u_int m_unk6C;
    u_int m_unk70_post;
    SpmNode** m_nodes;
};

class PrModelObject
{
public:
    void GetPrimitivePosition(NaVECTOR<float, 4> *vector);
    void GetScreenPosition(NaVECTOR<float, 4> *vector);

    void LinkAnimation(SpaFileHeader *animation);
    void LinkPositionAnimation(SpaFileHeader *animation);

    void CleanupAnimation();

    void SavePosture();
    void ResetPosture();

    void SaveContour();
    void ResetContour();

    ~PrModelObject();
public:
    char pad0[0x10];
    NaMATRIX<float, 4, 4> m_matrix;
    int m_modelId;
    int m_userData;
    SpmFileHeader *m_spmImage;
    int m_posture;
    float m_animTime;
    float m_animTimePosition;
    SpaFileHeader *m_linkedAnim;
    SpaFileHeader *m_linkedPosAnim;
    char pad1[0x24];
    float m_contourBlurAlpha;
    float m_contourBlurAlpha2;
    float m_transactionBlendRatio;
    float m_disturbance;
    
    float m_unkA4;
    char pad2[0x08];
};

#endif