#ifndef ANIMATION_H
#define ANIMATION_H

#include <eetypes.h>

#include "scene.h"

class PrSceneObject;

class SpaFileHeader
{
public:
    void Initialize();
public:
    u_int m_magic;
    u_short m_version;
    u_short m_flags;
    char pad0[0x0c];
    float m_endFrame;
    char m_name[32];
    void* m_unk38;
    u_int m_unk3C;
    PrSceneObject *m_unk40;
    u_int m_unk44;
    int m_userData;
    u_int m_unk4C;
    void* m_unk50;
};

#endif