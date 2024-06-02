#ifndef TAPCTRL_RPC_H
#define TAPCTRL_RPC_H

#include "common.h"

#include <eetypes.h>
#include <sifrpc.h>

int TapCtInit(void);
int TapCt(int command, int data1, int data2);

#endif