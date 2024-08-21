#include "menu/pksprite.h"

#include <eekernel.h>

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

/* sbss - static */
sceGsScissor _PkDefSCISSOR;
sceGsZbuf _PkDefZBUFFER;

/* sbss - static */
int _PkScrW;
int _PkScrH;

float FLT_003990DC;

static void _tsWorkEnd(TS_WORKMEM *emem)
{
    if (emem->isAlloc && emem->top)
    {
        free(emem->top);

        emem->top = NULL;
        emem->size = 0;
    }
}

#if 1
INCLUDE_ASM(const s32, "menu/pksprite", _tsWorkInit);
static u_int* _tsWorkInit(TS_WORKMEM *emem, u_int *buf, u_int size);
#else
static u_int* _tsWorkInit(/* s0 16 */ TS_WORKMEM *emem, /* s2 18 */ u_int *buf, /* s1 17 */ u_int size)
{
    u_int *tmp;

    _tsWorkEnd(emem);

    if (buf == NULL)
    {
        if (size != 0)
            tmp = memalign(128, size * sizeof(u_long128));
        else
            tmp = NULL;

        emem->top = tmp;
        emem->size = size;
        emem->isAlloc = 1;
    }
    else
    {
        emem->top = buf;
        emem->size = size;
        emem->isAlloc = 0;
    }

    return emem->top;
}
#endif

u_long128* TsInitUPacket(TsUSERPKT *pk, u_long128 *buf, u_int size)
{
    u_int      top;
    TsUSERPKT *p;
    u_int      b0, b1; /* Function matches, but we should be using these variables */

    memset(pk, 0, sizeof(TsUSERPKT));

    p = pk;
    top = (u_int)_tsWorkInit(&p->mem, (u_int*)buf, size);

    if (top == NULL)
    {
        return NULL;
    }
    else
    {
        p->size = size / 16;

        p->pkt[0].PaketTop = top;
        p->pkt[1].PaketTop = top;

        p->btop = p->ptop = p->pkt[p->idx].PaketTop | 0x20000000;
        return (u_long128*)top;
    }
}

void TsEndUPacket(TsUSERPKT *pk)
{
    TsUSERPKT *p = pk;

    _tsWorkEnd(&p->mem);

    p->pkt[0].PaketTop = NULL;
    p->pkt[1].PaketTop = NULL;

    p->size = 0;
    p->btop = NULL;
    p->ptop = NULL;
}

/* EUC-JP string: "★パケットSizeOver!!(User)(%x/%x)\n" */
#if 1
INCLUDE_ASM(const s32, "menu/pksprite", TsDrawUPacket);
#else
void TsDrawUPacket(/* s1 17 */ TsUSERPKT *up)
{
    /*
        up:P397
        pk:r398=*375
        qwc:r22
        PktChan:r399=*252
        -------
        tp:r255
        top:r22
        ------
        qwc:r1
        addr:r29
        ------
        addr:r29
    */

    /* s2 */ TSPAKET *pk = &up->pkt[up->idx];
    /* s0 */ u_int    qwc = (up->ptop & ~0x20000000) - pk->PaketTop;
    /* s0 */ sceDmaChan *PktChan;
    /* a3 7 */ sceDmaTag *tp;
	/* a1 5 */ u_int top;
	/* s0 16 */ //int qwc;
	/* a1 5 */ //void *addr;
	/* a1 5 */ //void *addr;

    if (qwc != 0)
    {
        if (qwc > up->size)
        {
            printf("★パケットSizeOver!!(User)(%x/%x)\n", qwc);
        }
        //else
        {
            top = pk->PaketTop;
            tp  = &pk->tag[0];

            if (qwc < 0xfff0)
            {
                void* addr = sceDmaAddRef(&tp, qwc, &top);
            }
            else
            {
                // int qwc;
                void *addr;

                while (qwc >= 0xfff0)
                {
                    addr = sceDmaAddRef(&tp, 0xfff0, &top);
                    qwc -= 0xfff0;

                    top += 0xfff0;
                }

                addr = sceDmaAddRef(&tp, qwc, &top);
            }

            sceDmaAddEnd(&tp, 0, NULL);

            up->idx  = 1 - up->idx;
            up->btop = up->pkt[1 - up->idx].PaketTop | 0x20000000;
            up->ptop = up->pkt[1 - up->idx].PaketTop | 0x20000000;

            PktChan = sceDmaGetChan(SCE_DMA_GIF);
            FlushCache(0);

            sceDmaSync(PktChan, 0, 0);
            sceDmaSend(PktChan, pk);
        }
    }
}
#endif

INCLUDE_ASM(const s32, "menu/pksprite", PkTEX0_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkTEX1_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkCLAMP_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkALPHA_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkTEST_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkSCISSOR_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkDefSCISSOR_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkOFFSET_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkPABE_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkFBA_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkCCLAMP_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkDefReg_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkTEX0_SetAdd);

INCLUDE_ASM(const s32, "menu/pksprite", GetDToneColor);

INCLUDE_ASM(const s32, "menu/pksprite", GetToneColorA);

INCLUDE_ASM(const s32, "menu/pksprite", GetToneColorH);

INCLUDE_ASM(const s32, "menu/pksprite", SetSprDefOfsXY);

INCLUDE_ASM(const s32, "menu/pksprite", SetSprScreenXYWH);

INCLUDE_ASM(const s32, "menu/pksprite", PkSprPkt_SetDrawEnv);

INCLUDE_ASM(const s32, "menu/pksprite", PkZBUFMask_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkSprPkt_SetTexVram);

INCLUDE_ASM(const s32, "menu/pksprite", PkSprPkt_SetDefault);

INCLUDE_ASM(const s32, "menu/pksprite", PkNSprite_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkNSprite_Add2);

INCLUDE_ASM(const s32, "menu/pksprite", PkNSprite_AddAdj);

INCLUDE_ASM(const s32, "menu/pksprite", PkCRect_Add);

#if 1
INCLUDE_ASM(const s32, "menu/pksprite", PkCGRect_Add);
void PkCGRect_Add(/* t4 12 */ SPR_PKT pk, /* a1 5 */ SPR_PRM *ppspr, /* a2 6 */ int flg);
#else
{
    /* a3 7 */ SprTagCG *sp = (SprTagCG*)*pk;

    *(u_long*)&sp->GifCord[0] = 0x9400000000008001;
    *(u_long*)&sp->GifCord[2] = 0x414141410;

    sp->prim  = 0x14c;
    sp->rgba0 = ppspr->rgba0;
    sp->rgba1 = ppspr->rgba1;
    sp->rgba2 = ppspr->rgba2;
    sp->rgba3 = ppspr->rgba3;

    asm __volatile__
    ("
        lqc2         vf01, 0(%0)
        lqc2         vf02, 0x10(%0)
        vitof0.xyzw  vf01, vf01
        vmul.zw      vf01, vf01, vf02
    " :: "r"(ppspr));

    if (flg & 2 && ppspr->zoom.isOn)
    {
        asm __volatile__
        ("
            lqc2         vf03, 0(%0)
            vmr32.xyzw   vf04, vf03
            vsub.xy      vf01, vf01, vf03
            vmr32.xyzw   vf04, vf04
            vmul.zw      vf01, vf01, vf03
            vmul.xy      vf01, vf01, vf04
            vadd.xy      vf01, vf01, vf03
        " :: "r"(&ppspr->zoom));
    }

    asm __volatile__
    ("
        vadd.xy vf01, vf01, vf02
        lw      $9, 0(%0)

        vmr32.xyzw   vf03, vf01
        vmr32.xyzw   vf03, vf03
        vadd.zw      vf01, vf01, vf03
        vftoi4.xyzw  vf01, vf01

        qmfc2        $8, vf01
        pexow        $11, $8
        ppach        $8, $0, $8
        ppach        $11, $0, $11
        pextlw       $10, $9, $8

        sd           $10, 0x20(%1) # a3
        pextlw       $10, $9, $11
        sd           $10, 0x30(%1)
        dsrl32       $10, $11, 0x0
        pextlw       $10, $9, $10
        sd           $10, 0x40(%1)
        dsrl32       $10, $8, 0x0
        pextlw       $10, $9, $10
        sd           $10, 0x50(%1) # a3
    " :: "r"(ppspr->zdepth), "r"(sp));

    *pk = sp->GifCord;
}
#endif

extern float S5432[4]; /* see SCE's libvu0 */

static void rotcossin(float rot)
{
    asm __volatile__
    ("
        mtc1	    $0, $f0
        c.olt.s     %0, $f0
        lwc1        $f0, FLT_003990DC   # li.s $f0, 1.57079637050628662109e0
        bc1f        _RotCosSin_01
        add.s       %0, $f0, %0         # rx += (π/2)
        li          $7, 0x1
        j           _RotCosSin_02

_RotCosSin_01:
        sub.s       %0, $f0, %0         # rx = (π/2)-rx
        move	    $7, $0

_RotCosSin_02:
        mfc1        $8, %0
        qmtc2       $8, vf03

        la          $8,   S5432         # Transfer coefficients of S5-S2 to VF05
        lqc2        vf01, 0($8)         #

        vmr32.w     vf03, vf03          # Copy VF03.x(v) to VF03.w
        vaddx.x     vf04, vf00, vf03    # Copy VF03.x(v) to VF04.x
        vmul.x      vf03, vf03, vf03    # Square VF03.x to v^2
        vmulx.yzw   vf04, vf04, vf00    # VF04.yzw = 0
        vmulw.xyzw  vf02, vf01, vf03    # Apply VF03.w(v) to S2-S5
        vmulx.xyzw  vf02, vf02, vf03    # Multiply by VF03.x(v^2)
        vmulx.xyz   vf02, vf02, vf03    # Multiply by VF03.x(v^2)
        vaddw.x     vf04, vf04, vf02    # s += k2
        vmulx.xy    vf02, vf02, vf03    # Multiply by VF03.x(v^2)
        vaddz.x     vf04, vf04, vf02    # s += z
        vmulx.x     vf02, vf02, vf03    # Multiply by VF03.x(v^2)
        vaddy.x     vf04, vf04, vf02    # s += y
        vaddx.x     vf04, vf04, vf02    # s += x (sin is over)
        vaddx.xy    vf04, vf19, vf04    # .xy = s (append)
        vmul.x      vf05, vf04, vf04    # VF05.x = s*s
        vsubx.w     vf05, vf00, vf05    # VF05.w = 1-(s*s)

        vsqrt       Q, vf05w            # Q = sqrt(1-s*s) (cos is over)
        vwaitq
        
        cfc2        $8, $vi22
        qmtc2       $8, vf05

        bne         $7, $0, _rcossin_01

        vaddx.x     vf04, vf19, vf05    # VF04.x = s
        b           _rcossin_02

_rcossin_01:
        vsubx.x     vf04, vf19, vf05    # VF04.x = s
_rcossin_02:
    " :: "f"(rot));
}

void _pkVU0RotMatrixZ(float rz)
{
    asm("vsub.xyzw    vf19, vf0, vf00");

    rotcossin(rz);

    asm __volatile__
    ("
        vmove.xyzw    vf09, vf00      
        vmove.zw      vf06, vf19      
        vmove.zw      vf07, vf19      
        vsub.zw       vf04, vf04, vf04
        vmr32.xyzw    vf08, vf09      
        vaddx.y       vf06, vf19, vf04
        vaddy.x       vf06, vf19, vf04
        vsubx.x       vf07, vf19, vf04
        vaddy.y       vf07, vf19, vf04
    ");
}

#if 1
INCLUDE_ASM(const s32, "menu/pksprite", PkRSprite_Add);
#else
void PkRSprite_Add(/* s3 19 */ SPR_PKT pk, /* s0 16 */ SPR_PRM *ppspr, /* s2 18 */ int flg)
{
    /* s1 17 */ SprTagTFR *sp = (SprTagTFR*)*pk;

    _pkVU0RotMatrixZ(ppspr->rot);

    *(u_long*)&sp->GifCord[0] = 0xa400000000008001;
    *(u_long*)&sp->GifCord[2] = 0x4343434310;

    sp->prim = 0x154;
    sp->rgba = ppspr->rgba0;

    if (flg & 1)
    {
        asm __volatile__("lq $9, 0x20(%0)" :: "r"(ppspr)); // ppspr->ux => $t1
    }
    else
    {
        asm __volatile__("lq $9,  0x0(%0)" :: "r"(ppspr));
    }

    asm __volatile__
    ("
        # t0 -> $8
        # t1 -> $9
        # t2 -> $10

        lq     $8, 0x20(%0)     # ppspr->ux => $t0 ($8)
        li     $10, -8
        daddu  $9,  $0,  $8
        pcpyld $8,  $8,  $0
        paddw  $9,  $8,  $9
        psllw  $9,  $9,  0x4

        li     $8,  0x8         # t0 = 8
        pextlw $8,  $10, $8
        pextlw $8,  $8,  $8
        paddw  $8,  $9,  $8
        ppach  $9,  $0,  $8
        sd     $9,  0x20(%1)    # sp->uv0

        pexew  $10, $8
        ppach  $8,  $0, $10
        sd     $8,  0x30(%1)    # sp->uv1

        prot3w $10, $8
        sd     $10, 0x40(%1)    # sp->uv2

        prot3w $10, $9
        sd     $10, 0x50(%1)    # sp->uv3
    " :: "r"(ppspr), "r"(sp));

    asm __volatile__
    ("
        lqc2        vf01, 0x0(%0)
        lq          $8,   0x10(%0)

        vitof0.xyzw vf01, vf01

        qmtc2       $8, vf02
        pcpyud      $8, $8, $8

        qmtc2       $8, vf03
        lq          $8, 0x30(%0)
        pcpyld      $8, $8, $8
        qmtc2       $8, vf04
        vmul.xy     vf04, vf04, vf03
    " :: "r"(ppspr));

    if (flg & 2 && ppspr->zoom.isOn)
    {
        asm __volatile__
        ("
            lq       $8, 0x0(%0)

            qmtc2    $8, vf05
            pcpyud   $8, $8, $8

            qmtc2    $8, vf08
            vadda.xy ACC,  vf01, vf0
            vmadd.xy vf01, vf04, vf08
            vmul.zw  vf01, vf01, vf05
            vmul.zw  vf04, vf04, vf08
            vsub.xy  vf01, vf01, vf05
            vadda.xy ACC,  vf05, vf00
            vmadd.xy vf01, vf01, vf08
        " :: "r"(&ppspr->zoom));
    }

    asm __volatile__
    ("
        vadd.xy      vf01, vf01, vf04
        vadd.xy      vf08, vf01, vf02
        vmr32.xyzw   vf02, vf02
        vmr32.xyzw   vf04, vf04
        vmr32.xyzw   vf05, vf01
        vmr32.xyzw   vf02, vf02
        vmr32.xyzw   vf04, vf04
        vmr32.xyzw   vf05, vf05

        # t4 -> $12
        lw           $12,  0x0(%0)
        vmul.xy      vf06, vf06, vf02
        vmul.xy      vf07, vf07, vf02
        vsub.xy      vf09, vf00, vf04
        vsub.y       vf10, vf00, vf04
        vsub.x       vf10, vf05, vf04
        vsub.x       vf11, vf00, vf04
        vsub.y       vf11, vf05, vf04
        vsub.xy      vf12, vf05, vf04
        vmulax.xyzw  ACC,  vf06, vf09
        vmadday.xyzw ACC,  vf07, vf09
        vmaddw.xyzw  vf09, vf08, vf00
        vmulax.xyzw  ACC,  vf06, vf10
        vmadday.xyzw ACC,  vf07, vf10
        vmaddw.xyzw  vf10, vf08, vf00
        vftoi4.xyzw  vf09, vf09
        vmulax.xyzw  ACC,  vf06, vf11
        vmadday.xyzw ACC,  vf07, vf11
        vmaddw.xyzw  vf11, vf08, vf00
        vftoi4.xyzw  vf10, vf10

        qmtc2        $8,   vf09
        vmulax.xyzw  ACC,  vf06, vf12
        vmadday.xyzw ACC,  vf07, vf12
        vmaddw.xyzw  vf12, vf08, vf00
        vftoi4.xyzw  vf11, vf11

        qmfc2        $9,   vf10
        ppach        $8,  $0,  $8
        pextlw       $8,  $12, $8
        sd           $8,  0x28(%1)

        vftoi4.xyzw  vf12, vf12

        ppach        $8,  $0,  $9
        pextlw       $8,  $12, $8
        sd           $8,  0x38(%1)

        qmfc2        $10, vf11
        ppach        $8,  $0,  $10
        pextlw       $8,  $12, $8
        sd           $8,  0x48(%1)

        qmfc2        $11, vf12
        ppach        $8, $0,  $11
        pextlw       $8, $12, $8
        sd           $8, 0x58(%1)
    " :: "r"(ppspr->zdepth), "r"(sp));

    *pk = sp->GifCord;
}
#endif

INCLUDE_ASM(const s32, "menu/pksprite", PkCLine2_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkCLineS_AddStart);

INCLUDE_ASM(const s32, "menu/pksprite", PkCLineS_AddNext);

INCLUDE_ASM(const s32, "menu/pksprite", PkPolyF3_Add);

INCLUDE_ASM(const s32, "menu/pksprite", PkPolyF4_Add);
void PkPolyF4_Add(/* t3 11 */ SPR_PKT pk, /* a1 5 */ SPR_PRM *ppspr, /* a2 6 */ int flg);

#if 1
INCLUDE_ASM(const s32, "menu/pksprite", PkPolyFT4_Add);
void PkPolyFT4_Add(/* t3 11 */ SPR_PKT pk, /* a1 5 */ SPR_PRM *ppspr, /* a2 6 */ int flg);
#else
void PkPolyFT4_Add(/* t3 11 */ SPR_PKT pk, /* a1 5 */ SPR_PRM *ppspr, /* a2 6 */ int flg)
{
    /* a3 7 */ SprTagTFR *sp = (SprTagTFR*)*pk;

    *(u_long*)&sp->GifCord[0] = 0xa400000000008001;
    *(u_long*)&sp->GifCord[2] = 0x4343434310;

    sp->prim = 0x154;
    sp->rgba = ppspr->rgba0;

    asm __volatile__
    ("
        # t0 -> $8
        # t1 -> $9
        # t2 -> $10

        lq     $9, 0x20(%0)     # ppspr->ux => $t1 ($9)
        lq     $8, 0x20(%0)     # ppspr->ux => $t0 ($8)
        li     $10, -8
        daddu  $9,  $0,  $8
        pcpyld $8,  $8,  $0
        paddw  $9,  $8,  $9
        psllw  $9,  $9,  0x4

        li     $8,  0x8         # t0 = 8
        pextlw $8,  $10, $8
        pextlw $8,  $8,  $8
        paddw  $8,  $9,  $8
        ppach  $9,  $0,  $8
        sd     $9,  0x20(%1)    # sp->uv0

        pexew  $10, $8
        ppach  $8,  $0, $10
        sd     $8,  0x30(%1)    # sp->uv1

        prot3w $10, $8
        sd     $10, 0x40(%1)    # sp->uv2

        prot3w $10, $9
        sd     $10, 0x50(%1)    # sp->uv3
    " :: "r"(ppspr), "r"(sp));

    asm __volatile__
    ("
        lqc2        vf01, 0x50(%0)
        lqc2        vf02, 0x60(%0)
        lqc2        vf04, 0x10(%0)

        vmr32.xyzw  vf03, vf04
        vmr32.xyzw  vf03, vf03
        vadd.xy     vf03, vf00, vf04
    " :: "r"(ppspr));

    if (flg & 2 && ppspr->zoom.isOn)
    {
        asm __volatile__
        ("
            lq       $8, 0x0(%0)

            qmtc2    $8, vf05
            pcpyud   $8, $8, $8

            qmtc2    $8, vf08
            vadda.xy ACC,  vf01, vf0
            vmadd.xy vf01, vf04, vf08
            vmul.zw  vf01, vf01, vf05
            vmul.zw  vf04, vf04, vf08
            vsub.xy  vf01, vf01, vf05
            vadda.xy ACC,  vf05, vf00
            vmadd.xy vf01, vf01, vf08
        " :: "r"(&ppspr->zoom));
    }

    asm __volatile__
    ("
        vadd.xy      vf01, vf01, vf04
        vadd.xy      vf08, vf01, vf02
        vmr32.xyzw   vf02, vf02
        vmr32.xyzw   vf04, vf04
        vmr32.xyzw   vf05, vf01
        vmr32.xyzw   vf02, vf02
        vmr32.xyzw   vf04, vf04
        vmr32.xyzw   vf05, vf05

        # t4 -> $12
        lw           $12,  0x0(%0)
        vmul.xy      vf06, vf06, vf02
        vmul.xy      vf07, vf07, vf02
        vsub.xy      vf09, vf00, vf04
        vsub.y       vf10, vf00, vf04
        vsub.x       vf10, vf05, vf04
        vsub.x       vf11, vf00, vf04
        vsub.y       vf11, vf05, vf04
        vsub.xy      vf12, vf05, vf04
        vmulax.xyzw  ACC,  vf06, vf09
        vmadday.xyzw ACC,  vf07, vf09
        vmaddw.xyzw  vf09, vf08, vf00
        vmulax.xyzw  ACC,  vf06, vf10
        vmadday.xyzw ACC,  vf07, vf10
        vmaddw.xyzw  vf10, vf08, vf00
        vftoi4.xyzw  vf09, vf09
        vmulax.xyzw  ACC,  vf06, vf11
        vmadday.xyzw ACC,  vf07, vf11
        vmaddw.xyzw  vf11, vf08, vf00
        vftoi4.xyzw  vf10, vf10

        qmtc2        $8,   vf09
        vmulax.xyzw  ACC,  vf06, vf12
        vmadday.xyzw ACC,  vf07, vf12
        vmaddw.xyzw  vf12, vf08, vf00
        vftoi4.xyzw  vf11, vf11

        qmfc2        $9,   vf10
        ppach        $8,  $0,  $8
        pextlw       $8,  $12, $8
        sd           $8,  0x28(%1)

        vftoi4.xyzw  vf12, vf12

        ppach        $8,  $0,  $9
        pextlw       $8,  $12, $8
        sd           $8,  0x38(%1)

        qmfc2        $10, vf11
        ppach        $8,  $0,  $10
        pextlw       $8,  $12, $8
        sd           $8,  0x48(%1)

        qmfc2        $11, vf12
        ppach        $8, $0,  $11
        pextlw       $8, $12, $8
        sd           $8, 0x58(%1)
    " :: "r"(ppspr->zdepth), "r"(sp));

    // **pk = *(u_long128*)&sp->GifCord;
    *pk = sp->GifCord;
}
#endif

PKMESH* PkMesh_Create(int w, int h)
{
    PKMESH *pmesh = malloc(sizeof(PKMESH));

    if (pmesh == NULL)
        return NULL;

    memset(pmesh, 0, sizeof(PKMESH));
    pmesh->pmspt = malloc(sizeof(PKMSPT) * (w + 1) * (h + 1));

    if (pmesh->pmspt == NULL)
    {
        free(NULL);
        return NULL;
    }
    else
    {
        pmesh->mw = w;
        pmesh->mh = h;
    }

    return pmesh;
}

void PkMesh_Delete(PKMESH *mesh)
{
    if (mesh == NULL)
        return;
    
    if (mesh->pmspt != NULL)
        free(mesh->pmspt);

    free(mesh);
}

void PkMesh_SetXYWH(PKMESH *mesh, float px0, float py0, float sw, float sh)
{
    PKMSPT *pt;
    int     x, y;
    float   fmw, fmh;
    float   py;

    pt = mesh->pmspt;

    mesh->px = px0;
    mesh->py = py0;
    
    mesh->sw = sw;
    mesh->sh = sh;

    fmw = 1.0f / mesh->mw;
    fmh = 1.0f / mesh->mh;

    for (y = 0; y < (mesh->mh + 1); y++)
    {
        py = py0 + ((y * sh) * fmh);

        for (x = 0; x < (mesh->mw + 1); x++, pt++)
        {
            pt->x = px0 + ((x * sw) * fmw);
            pt->y = py;

            pt->ofsx = pt->ofsy = 0.0f;
        }
    }
}

void PkMesh_SetUVWH(PKMESH *mesh, float ux0, float uy0, float uw, float uh)
{
    PKMSPT *pt;
    int     x, y;
    float   fmw, fmh;
    int     uy;

    pt = mesh->pmspt;

    fmw = 1.0f / mesh->mw;
    fmh = 1.0f / mesh->mh;

    for (y = 0; y < (mesh->mh + 1); y++)
    {
        uy = uy0 + ((y * uh) * fmh);

        for (x = 0; x < (mesh->mw + 1); x++, pt++)
        {
            pt->u = ux0 + ((x * uw) * fmw);
            pt->v = uy;         
        }
    }
}

void PkCMesh_Add(SPR_PKT pk, SPR_PRM *spr, PKMESH *mesh)
{
    int     cidx, lidx;
    int     x, y;
    PKMSPT *pt;

    cidx = 0;
    lidx = mesh->mw + 1;

    for (y = 0; y < mesh->mh; y++)
    {
        pt = &mesh->pmspt[cidx];

        for (x = 0; x < mesh->mw; x++, pt++)
        {
            spr->px0 = pt[0].x + pt[0].ofsx;
            spr->py0 = pt[0].y + pt[0].ofsy;

            spr->px1 = pt[1].x + pt[1].ofsx;
            spr->py1 = pt[1].y + pt[1].ofsy;

            spr->px2 = pt[lidx].x + pt[lidx].ofsx;
            spr->py2 = pt[lidx].y + pt[lidx].ofsy;

            spr->px3 = pt[lidx + 1].x + pt[lidx + 1].ofsx;
            spr->py3 = pt[lidx + 1].y + pt[lidx + 1].ofsy;

            PkPolyF4_Add(pk, spr, 0);
        }

        cidx += lidx;
    }
}

void PkFTMesh_Add(SPR_PKT pk, SPR_PRM *spr, PKMESH *mesh)
{
    int     cidx, lidx;
    int     x, y;
    PKMSPT *pt;

    cidx = 0;
    lidx = mesh->mw + 1;

    for (y = 0; y < mesh->mh; y++)
    {
        pt = &mesh->pmspt[cidx];

        for (x = 0; x < mesh->mw; x++, pt++)
        {
            spr->ux  = pt[0].u;
            spr->uy  = pt[0].v;

            spr->uw  = pt[1].u    - pt[0].u;
            spr->uh  = pt[lidx].v - pt[0].v;

            spr->px0 = pt[0].x + pt[0].ofsx;
            spr->py0 = pt[0].y + pt[0].ofsy;

            spr->px1 = pt[1].x + pt[1].ofsx;
            spr->py1 = pt[1].y + pt[1].ofsy;

            spr->px2 = pt[lidx].x + pt[lidx].ofsx;
            spr->py2 = pt[lidx].y + pt[lidx].ofsy;

            spr->px3 = pt[lidx + 1].x + pt[lidx + 1].ofsx;
            spr->py3 = pt[lidx + 1].y + pt[lidx + 1].ofsy;

            PkPolyFT4_Add(pk, spr, 2);
        }

        cidx += lidx;
    }
}

#if 1
INCLUDE_ASM(const s32, "menu/pksprite", PkMesh_SetHLinOfs);
#else
void PkMesh_SetHLinOfs(/* v1 3 */ PKMESH *mesh, /* a3 7 */ int no, /* f12 50 */ float x, /* f13 51 */ float y)
{
    /* a0 4 */ PKMSPT *pt;
    /* v1 3 */ int i;

    if (mesh->mh >= no)
    {
        i  = mesh->mw + 1;
        pt = &mesh->pmspt[i * no];

        /* For loop or while loop? */
        #if 0
        for (i; i > 0; i--)
        {
            pt->ofsx = x;
            pt->ofsy = y;
            pt++;
        }
        #else
        if (i > 0)
        {
            while (i != 0)
            {
                pt->ofsx = x;
                pt->ofsy = y;

                pt++;
                i--;
            }
        }
        #endif
    }
}
#endif

INCLUDE_ASM(const s32, "menu/pksprite", PkMesh_SetVLinOfs);

INCLUDE_ASM(const s32, "menu/pksprite", PkMesh_SetHLinOfsLRX);

INCLUDE_ASM(const s32, "menu/pksprite", PkMesh_SetVLinOfsUDY);
