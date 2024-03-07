#include "main/fadectrl.h"

static tGS_BGCOLOR bgcolor_tmp[2] = {{}, {255, 255, 255, 0, 0}};
int PAD_FADECTRL[] = { 0 }; /* pad */

/* bss - static */
FMODE_CTRL_STR fmode_ctrl_str;

void FadeCtrlMain(void *x)
{
    int tmp_time;
    FADE_MAKE_STR fade_make_str;

    fade_make_str.r = bgcolor_tmp[fmode_ctrl_str.fmode & 255].R;
    fade_make_str.g = bgcolor_tmp[fmode_ctrl_str.fmode & 255].G;
    fade_make_str.b = bgcolor_tmp[fmode_ctrl_str.fmode & 255].B;

    if (fmode_ctrl_str.current_time < fmode_ctrl_str.max_time)
    {
        do 
        {
            tmp_time = fmode_ctrl_str.current_time;

            if ((fmode_ctrl_str.fmode & FMODE_BLACK_OUT) == 0)
                tmp_time = fmode_ctrl_str.max_time - fmode_ctrl_str.current_time;

            fade_make_str.alp = (tmp_time * 128) / fmode_ctrl_str.max_time;
            
            CG_FadeDisp(&fade_make_str, 100, NULL);
            
            fmode_ctrl_str.current_time++;
            MtcWait(1);
        } while (fmode_ctrl_str.current_time < fmode_ctrl_str.max_time);
    }

    MtcExit();
}

void FadeCtrlReq(FADE_MODE fmode, int time)
{
    fmode_ctrl_str.current_time = 0;
    fmode_ctrl_str.fmode        = fmode;
    fmode_ctrl_str.max_time     = time;

    MtcExec(FadeCtrlMain, MTC_TASK_FACECTRL);
}
