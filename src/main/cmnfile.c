#include "main/cmnfile.h"

/* Common files array */
extern char common_ipk_start[];
extern char common_ipk_end[];

/* ====================== COMMON FILES OVERVIEW ====================== */
/* PaRappa the Rapper 2 stores some of its assets inside of the ELF    */
/* to reduce loading times -- these are assets use commonly thru the   */
/* game, like the models for the loading screen.                       */
/* ------------------------------------------------------------------- */
/* The files are separated in categories (see CMN_FILE_TYPE_ENUM).     */
/* When we want to access a file, we first search for the category     */
/* the file is in (VRAM for textures, SND for sounds, etc.), then      */
/* access the address based on the file's ID number.                   */
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
