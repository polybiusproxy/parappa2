#ifndef RENDERSTUFF_H
#define RENDERSTUFF_H

#include <eetypes.h>
#include <eestruct.h>

#include "prlib_intern.h"

class PrRenderStuff
{
public:
    void Initialize(sceGsZbuf zbuf);
    void WaitRender();

    void Cleanup();
public:
    char pad0[0x1C];
    int _1C;
    sceGsZbuf m_gsZbuf;
    char pad1[0x4];
    PrRENDERING_STATISTICS m_renderStatistics;
};

#endif