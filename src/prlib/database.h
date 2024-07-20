#ifndef DATABASE_H
#define DATABASE_H

#include "scene.h"

#include "model.h"

#include "animation.h"

class SpaFileHeader;
class SpcFileHeader;

class PrObjectDatabase
{
public:
    void Initialize();
    void Cleanup();

    PrSceneObject* CreateScene(sceGsDrawEnv1 *drawEnv, const char* name, u_int fbp);
    void DeleteScene(PrSceneObject *scene);
public:
    int _0;
    int _4;
    int _8;
    SpaFileHeader *m_unk0C_animation;
    SpaFileHeader *m_unk10_animation;
    int _14;
    SpcFileHeader *m_unk18_camera;
    SpcFileHeader *m_unk1C_camera;
    int _20;
};

#endif