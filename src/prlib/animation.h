#ifndef ANIMATION_H
#define ANIMATION_H

#include "scene.h"

class PrSceneObject;

class SpaFileHeader
{
public:
    void Initialize();
public:
    u32 m_magic;
    u16 m_version;
    u16 m_flags;
    char pad0[0x0c];
    float m_endFrame;
    char m_name[32];
    void* m_unk38;
    u32 m_unk3C;
    PrSceneObject *m_unk40;
    u32 m_unk44;
    int m_userData;
    u32 m_unk4C;
    void* m_unk50;
};

#endif