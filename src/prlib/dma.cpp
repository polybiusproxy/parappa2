#include "dma.h"

#include <eeregs.h>

#define PR_CHCR volatile tD_CHCR

void PrWaitDmaFinish(u_int dmaChannel)
{
    static PR_CHCR* chcr_table[12] =
    {
        (tD_CHCR*)D0_CHCR,
        (tD_CHCR*)D1_CHCR,
        (tD_CHCR*)D2_CHCR,
        (tD_CHCR*)D3_CHCR,
        (tD_CHCR*)D4_CHCR,
        (tD_CHCR*)D5_CHCR,
        (tD_CHCR*)D6_CHCR,
        (tD_CHCR*)D7_CHCR,
        (tD_CHCR*)D8_CHCR,
        (tD_CHCR*)D9_CHCR,
        NULL,
        NULL,
    };

    PR_CHCR* chcr = chcr_table[dmaChannel];

    if (dmaChannel >= 8)
    {
        while (chcr->STR & 1);
    }
    else
    {
        *D_STAT = (1 << dmaChannel);
        if (chcr->STR & 1)
        {
            u_int pcr = *D_PCR;
            u_int pcrOld = pcr;
            
            pcr &= ~0x3ff;
            pcr |= (1 << dmaChannel);
            *D_PCR = pcr;
            asm("sync.l");

            asm volatile
            ("
            cpcond0_wait_loop:
                bc0f cpcond0_wait_loop
                nop
            ");

            *D_STAT = (1 << dmaChannel);
            *D_PCR = pcrOld;
        }
    }
}
