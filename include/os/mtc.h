#ifndef MTC_H
#define MTC_H

#include "common.h"

#include "libgraph.h"
#include "eekernel.h"

#include "dbug/syori.h"

typedef enum {
	MTC_TASK_SIZE_CTRL = 4096,
	MTC_TASK_SIZE_MAIN = 4096,
	MTC_TASK_SIZE_02 = 256,
	MTC_TASK_SIZE_03 = 2048,
	MTC_TASK_SIZE_04 = 4096,
	MTC_TASK_SIZE_05 = 16384,
	MTC_TASK_SIZE_06 = 256,
	MTC_TASK_SIZE_07 = 16384,
	MTC_TASK_SIZE_08 = 256,
	MTC_TASK_SIZE_09 = 256,
	MTC_TASK_SIZE_0A = 4096,
	MTC_TASK_SIZE_0B = 256,
	MTC_TASK_SIZE_0C = 256,
	MTC_TASK_SIZE_0D = 4096,
	MTC_TASK_SIZE_0E = 256,
	MTC_TASK_SIZE_0F = 4096
} MTC_TASK_SIZE_ENUM;

typedef enum {
	MTC_COND_KILL = 0,
	MTC_COND_WAIT = 1,
	MTC_COND_EXEC = 2,
	MTC_COND_KILL_REQ = 3,
	MTC_COND_PAUSE = 32768
} MTC_COND_ENUM;

typedef enum {
	MTC_TOP_PRI = 16,
	MTC_CTRL_TOP = 16,
	MTC_EACH = 17,
	MTC_CTRL_END = 18
} MTC_PRI_ENUM;

typedef struct { // 0x40
	/* 0x00 */ short int th_id;
	/* 0x04 */ MTC_COND_ENUM status;
	/* 0x08 */ long int wtime;
	/* 0x10 */ struct ThreadParam th_para;
} MTC_TASK_CONB;

void MtcChangeThCtrl(void *x);
void MtcInit(void);
void MtcQuit(void);
void MtcStart(void *ctrlTh_pp);
void MtcExec(void *prg_pp, long int level);
void MtcWait(long int wt);
void MtcKill(long int level);
void MtcPause(long int level);
void MtcContinue(long int level);
void MtcExit(void);
int MtcGetCondition(long int level);
int MtcResetCheck(void);

#endif
