#include "main/cmnfile.h"

/* Common files array */
extern char common_ipk_start[];
extern char common_ipk_end[];

/* =====================  COMMON FILES OVERVIEW  ===================== */
/* PaRappa the Rapper 2 stores some of its assets inside of the ELF    */
/* to reduce loading times -- these are assets use commonly thru the   */
/* game, like the models for the loading screen.                       */
/* ------------------------------------------------------------------- */
/* The files are separated in categories (see CMN_FILE_TYPE_ENUM).     */
/* When we want to access a file, we first search for the category     */
/* the file is in (VRAM for textures, SND for sounds, etc.), then      */
/* access the address based on the file's ID number.                   */
/* =================================================================== */

/* ======================  COMMON FILES TABLE  ======================= */
/* ONMEM category (0x001b8440, NTSC-J proto)                           */
/*     -> hirecord_SPXm.spm     (ID: 00, offset 0x190)                 */
/*     -> label_SPXm.spm        (ID: 01, offset 0x11B0)                */
/*     -> loading_SPXm.spm      (ID: 02, offset 0x3360)                */
/*     -> record_SPXm.spm       (ID: 03, offset 0x2E730)               */
/*     -> turn_SPXm.spm         (ID: 04, offset 0x30D80)               */
/*     -> ???                   (ID: 05, offset 0x230330)              */
/*     -> ???                   (ID: 06, offset 0x230510)              */
/*     -> ???                   (ID: 07, offset ???)                   */
/*     -> ???                   (ID: 08, offset ???)                   */
/*     -> ???                   (ID: 09, offset ???)                   */
/*     -> ???                   (ID: 10, offset ???)                   */
/*     -> ???                   (ID: 11, offset ???)                   */
/*     -> ???                   (ID: 12, offset ???)                   */
/*     -> ???                   (ID: 13, offset ???)                   */
/*     -> ???                   (ID: 14, offset ???)                   */
/*     -> ???                   (ID: 15, offset ???)                   */
/*     -> ???                   (ID: 16, offset ???)                   */
/*     -> ???                   (ID: 17, offset ???)                   */
/*     -> ???                   (ID: 18, offset ???)                   */
/*     -> ???                   (ID: 19, offset ???)                   */
/*     -> ???                   (ID: 20, offset ???)                   */
/*     -> ???                   (ID: 21, offset ???)                   */
/*     -> ???                   (ID: 22, offset ???)                   */
/*     -> ???                   (ID: 23, offset ???)                   */
/*     -> ???                   (ID: 24, offset ???)                   */
/*     -> ???                   (ID: 25, offset ???)                   */
/*     -> ???                   (ID: 26, offset ???)                   */
/*     -> ???                   (ID: 27, offset ???)                   */
/*     -> ???                   (ID: 28, offset ???)                   */
/*     -> ???                   (ID: 29, offset ???)                   */
/*     -> ???                   (ID: 30, offset ???)                   */
/*     -> ???                   (ID: 31, offset ???)                   */
/*     -> ???                   (ID: 32, offset ???)                   */
/*     -> ???                   (ID: 33, offset ???)                   */
/*     -> ???                   (ID: 34, offset ???)                   */
/*     -> ???                   (ID: 35, offset ???)                   */
/*     -> ???                   (ID: 36, offset ???)                   */
/*     -> ???                   (ID: 37, offset ???)                   */
/*     -> ???                   (ID: 38, offset ???)                   */
/*     -> ???                   (ID: 39, offset ???)                   */
/*     -> ???                   (ID: 40, offset ???)                   */
/*     -> ???                   (ID: 41, offset ???)                   */
/*     -> ???                   (ID: 42, offset ???)                   */
/*     -> ???                   (ID: 43, offset ???)                   */
/*     -> ???                   (ID: 44, offset ???)                   */
/*     -> ???                   (ID: 45, offset ???)                   */
/*     -> ???                   (ID: 46, offset ???)                   */
/*     -> ???                   (ID: 47, offset ???)                   */
/*     -> ???                   (ID: 48, offset ???)                   */
/*     -> ???                   (ID: 49, offset ???)                   */
/*     -> ???                   (ID: 50, offset ???)                   */
/*     -> ???                   (ID: 51, offset ???)                   */
/*     -> ???                   (ID: 52, offset ???)                   */
/*     -> ???                   (ID: 53, offset ???)                   */
/*     -> ???                   (ID: 54, offset ???)                   */
/*     -> ???                   (ID: 55, offset ???)                   */
/*     -> ???                   (ID: 56, offset ???)                   */
/*     -> ???                   (ID: 57, offset ???)                   */
/*     -> ???                   (ID: 58, offset ???)                   */
/*     -> ???                   (ID: 59, offset ???)                   */
/*     -> ???                   (ID: 60, offset ???)                   */
/*     -> ???                   (ID: 61, offset ???)                   */
/*     -> ???                   (ID: 62, offset ???)                   */
/*     -> ???                   (ID: 63, offset ???)                   */
/*     -> ???                   (ID: 64, offset ???)                   */
/*     -> ???                   (ID: 65, offset ???)                   */
/*     -> ???                   (ID: 66, offset ???)                   */
/*     -> ???                   (ID: 67, offset ???)                   */
/*     -> ???                   (ID: 68, offset ???)                   */
/*     -> ???                   (ID: 69, offset ???)                   */
/*     -> ???                   (ID: 70, offset ???)                   */
/*     -> ???                   (ID: 71, offset ???)                   */
/*     -> ???                   (ID: 72, offset ???)                   */
/*     -> ???                   (ID: 73, offset ???)                   */
/*     -> ???                   (ID: 74, offset ???)                   */
/*     -> ???                   (ID: 75, offset ???)                   */
/*     -> ???                   (ID: 76, offset ???)                   */
/*     -> ???                   (ID: 78, offset ???)                   */
/*     -> ???                   (ID: 79, offset ???)                   */
/*     -> ???                   (ID: 80, offset ???)                   */
/*     -> ???                   (ID: 81, offset ???)                   */
/*     -> ???                   (ID: 82, offset ???)                   */
/*     -> ???                   (ID: 83, offset ???)                   */
/*     -> ???                   (ID: 84, offset ???)                   */
/*     -> ???                   (ID: 85, offset ???)                   */
/*     -> ???                   (ID: 86, offset ???)                   */
/*     -> ???                   (ID: 87, offset ???)                   */
/*     -> ???                   (ID: 88, offset ???)                   */
/*     -> ???                   (ID: 89, offset ???)                   */
/*     -> ???                   (ID: 90, offset ???)                   */
/*     -> ???                   (ID: 91, offset ???)                   */
/*     -> ???                   (ID: 92, offset ???)                   */
/*     -> ???                   (ID: 93, offset ???)                   */
/*     -> ???                   (ID: 94, offset ???)                   */
/*     -> ???                   (ID: 95, offset ???)                   */
/* =================================================================== */

static CMN_FILE_STR* cmn_file_str_search(CMN_FILE_TYPE_ENUM type)
{
    /* Load the common files array */
    CMN_FILE_STR *cfstr_pp = (CMN_FILE_STR*)common_ipk_start;

    while (1)
    {
        if (cfstr_pp->ftype == type)
            return cfstr_pp;

        /* Not found... null ptr! */
        if (cfstr_pp->ftype == CMN_NONE)
            return NULL;
        
        /* Go through all categories until we find the one we want */
        cfstr_pp = (void*)cfstr_pp + cfstr_pp->f_size;
    }
}

int cmnfTim2Trans(void)
{
    int           i;
    CMN_FILE_STR *cfstr_pp;

    /* Get the VRAM file category */
    cfstr_pp = cmn_file_str_search(CMN_VRAM);

    if (cfstr_pp == NULL)
        return 0;

    /* Transfer all the common textures to VRAM */
    for (i = 0; i < cfstr_pp->fnum; i++)
    {
        Tim2Trans((void*)cfstr_pp + cfstr_pp->adr[i]);
    }

    return 1;
}

void* cmnfGetFileAdrs(int num)
{
    /* Get the ONMEM file category */
    CMN_FILE_STR *cfstr_pp = cmn_file_str_search(CMN_ONMEM);
    
    if (cfstr_pp != NULL)
    {
        if (num >= cfstr_pp->fnum)
        {
            printf("CMN FILE NUM OVER[%d]", num);
            return NULL;
        }
        else
        {
            return (void*)cfstr_pp + cfstr_pp->adr[num];
        }
    }
    
    return NULL;
}

int cmnfGetFileSize(int num)
{
    /* Get the ONMEM file category */
    CMN_FILE_STR *cfstr_pp = cmn_file_str_search(CMN_ONMEM);
    
    if (cfstr_pp != NULL)
    {
        if (num >= cfstr_pp->fnum)
        {
            printf("CMN FILE NUM OVER[%d]", num);
            return NULL;
        }
        else
        {
            return cfstr_pp->adr[num + 1] - cfstr_pp->adr[num];
        }
    }
    
    return 0;
}
