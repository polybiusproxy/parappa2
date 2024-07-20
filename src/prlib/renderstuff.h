#ifndef RENDERSTUFF_H
#define RENDERSTUFF_H

#include <eetypes.h>
#include <eestruct.h>

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