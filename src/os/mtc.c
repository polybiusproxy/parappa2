#include "os/mtc.h"

/* bss */
extern MTC_TASK_CONB mtcTaskConB[16];
extern struct ThreadParam th_para_Ctrl;
extern char mtcStack_Ctrl[4096];

extern char mtcStack_CTRL[4096];
extern char mtcStack_MAIN[4096];
extern char mtcStack_02[256];
extern char mtcStack_03[2048];
extern char mtcStack_04[4096];
extern char mtcStack_05[16384];
extern char mtcStack_06[256];
extern char mtcStack_07[16384];
extern char mtcStack_08[256];
extern char mtcStack_09[256];
extern char mtcStack_0A[4096];
extern char mtcStack_0B[256];
extern char mtcStack_0C[256];
extern char mtcStack_0D[4096];
extern char mtcStack_0E[256];
extern char mtcStack_0F[4096];

/* data */
// Size for each stack
extern int mtcStackSizeTbl[]/* = { 
    0x1000, 0x1000, 0x100,  0x800,
    0x1000, 0x4000, 0x100,  0x4000,
    0x100,  0x100,  0x1000, 0x100,
    0x100,  0x1000, 0x100,  0x1000
}*/;

extern char* mtcStack[16] /*= {
    mtcStack_CTRL, mtcStack_MAIN, mtcStack_02, mtcStack_03,
    mtcStack_04,   mtcStack_05,   mtcStack_06, mtcStack_07,
    mtcStack_08,   mtcStack_09,   mtcStack_0A, mtcStack_0B,
    mtcStack_0C,   mtcStack_0D,   mtcStack_0E, mtcStack_0F
}*/;

extern struct SemaParam mtcSemaPara /*= { 0, 0, 0, 0, 0, 0 }*/;

/* sdata */
extern int mtcCurrentTask;
extern int mtcSemaEnd;

/* sbss */
extern short int th_id_Ctrl;

static void mtcStackErrorCheck(int level) {
    if ( *(int*)mtcStack[level] != 0x572a8b4c ) {
        printf("stack over level[%d]\n\0\0\0", level);
        while (1)
            sceGsSyncV(0);
    }
}

void MtcChangeThCtrl(void* x) {
    while (1) {
        mtcCurrentTask++;

        if (mtcCurrentTask > 15) {
            mtcCurrentTask = 0;

            if (MtcResetCheck() != 0)
                SignalSema(mtcSemaEnd);
        }

        if (mtcTaskConB[mtcCurrentTask].status == MTC_COND_EXEC) {
            SyoriLineCnt(mtcCurrentTask);
            RotateThreadReadyQueue(16);

            StartThread(mtcTaskConB[mtcCurrentTask].th_id, 0);
            SleepThread();
        }
        else if ((mtcTaskConB[mtcCurrentTask].status == MTC_COND_WAIT) && (--mtcTaskConB[mtcCurrentTask].wtime <= 0)) 
        {
            SyoriLineCnt(mtcCurrentTask);
            RotateThreadReadyQueue(16);

            WakeupThread(mtcTaskConB[mtcCurrentTask].th_id);
            SleepThread();
        }
    }
}

void MtcInit(void) {
    PR_CLEAR(mtcTaskConB);

    mtcSemaPara.maxCount = 1;
    mtcSemaPara.initCount = 0;

    mtcSemaEnd = CreateSema(&mtcSemaPara);

    th_para_Ctrl.entry = MtcChangeThCtrl;
    th_para_Ctrl.stack = mtcStack_Ctrl;
    th_para_Ctrl.stackSize = MTC_TASK_SIZE_CTRL;
    th_para_Ctrl.initPriority = MTC_CTRL_END;
    th_para_Ctrl.gpReg = &_gp;

    th_id_Ctrl = CreateThread(&th_para_Ctrl);
}

void MtcQuit(void) {
    int i;

    TerminateThread(th_id_Ctrl);
    DeleteThread(th_id_Ctrl);
    DeleteSema(mtcSemaEnd);

    for (i = 0; i < PR_ARRAYSIZE(mtcTaskConB); i++) {
        MtcKill(i);
    }
}

void MtcStart(void* ctrlTh_pp) {
    MtcExec(ctrlTh_pp, NULL);

    mtcCurrentTask = -1;

    StartThread(th_id_Ctrl, NULL);
    WaitSema(mtcSemaEnd);
}

void MtcExec(void* prg_pp, long int level) {
    struct ThreadParam* th_pp;
    MTC_TASK_CONB* mc_pp = &mtcTaskConB[level];

    if (mc_pp->status != MTC_COND_KILL) {
        MtcKill(level);
    }

    th_pp = &mc_pp->th_para;
    th_pp->entry = prg_pp;
    th_pp->stack = mtcStack[level];
    th_pp->stackSize = mtcStackSizeTbl[level];
    th_pp->initPriority = MTC_EACH;
    th_pp->gpReg = &_gp;
    th_pp->option = level;

    mc_pp->th_id = CreateThread(&mc_pp->th_para);
    mc_pp->wtime = 0;
    mc_pp->status = MTC_COND_EXEC;

    *(int*)mtcStack[level] = 0x572a8b4c;
}

void MtcWait(long int wt) {
    FlushCache(0);
    mtcStackErrorCheck(mtcCurrentTask);

    mtcTaskConB[mtcCurrentTask].status = MTC_COND_WAIT;
    mtcTaskConB[mtcCurrentTask].wtime  = wt;

    ChangeThreadPriority(th_id_Ctrl, MTC_CTRL_END);

    WakeupThread(th_id_Ctrl);
    SleepThread();
}

void MtcKill(long int level) {
    MTC_TASK_CONB* tcb_pp = &mtcTaskConB[level];
    MTC_COND_ENUM mtc_f = tcb_pp->status;

    mtc_f |= ~MTC_COND_PAUSE;

    if (mtc_f != MTC_COND_KILL) {
        tcb_pp->status = MTC_COND_KILL;

        if (mtc_f != MTC_COND_EXEC)
            TerminateThread(tcb_pp->th_id);

        DeleteThread(tcb_pp->th_id);
    }
}

void MtcPause(long int level) {
    MTC_TASK_CONB* tcb_pp = &mtcTaskConB[level];

    if (tcb_pp->status != 0) {
        tcb_pp->status |= MTC_COND_PAUSE;
    }
}

void MtcContinue(long int level) {
    MTC_TASK_CONB* tcb_pp = &mtcTaskConB[level];
    tcb_pp->status &= ~MTC_COND_PAUSE;
}

void MtcExit(void) {
    MTC_TASK_CONB* tcb_pp;
    mtcStackErrorCheck(mtcCurrentTask);

    tcb_pp = &mtcTaskConB[mtcCurrentTask];
    tcb_pp->status = MTC_COND_KILL;

    ChangeThreadPriority(th_id_Ctrl, MTC_CTRL_END);
    WakeupThread(th_id_Ctrl);
    ExitDeleteThread();
}

int MtcGetCondition(long int level) {
    return mtcTaskConB[level].status;
}

int MtcResetCheck(void) {
    return 0;
}
