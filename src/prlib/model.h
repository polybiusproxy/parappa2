#ifndef MODEL_H
#define MODEL_H

#include <nalib/navector.h>
#include <nalib/namatrix.h>

class PrModelObject
{
public:
    void GetPrimitivePosition(NaVECTOR<float, 4> *vector);
    void GetScreenPosition(NaVECTOR<float, 4> *vector);
public:
    char pad0[0x10];
    NaMATRIX<float, 4, 4> m_matrix;
    int m_modelId;
    int m_userData;
    void* m_spmImage;
    int m_posture;
    char pad1[0x40];
    float m_disturbance;
};

#endif