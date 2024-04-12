#ifndef USRMEM_H
#define USRMEM_H

#include "common.h"
#include <eetypes.h>

#include <stdio.h>

void UsrMemClear(void);
void UsrMemClearTop(void);
void UsrMemClearEnd(void);

u_int UsrMemGetAdr(int id);
u_int UsrMemGetSize(int id);
u_int UsrMemGetEndAdr(int id);
u_int UsrMemGetEndSize(int id);

u_int UsrMemAllocNext(void);
u_int UsrMemAllocEndNext(void);

u_int UsrMemAlloc(int size);
u_int UsrMemEndAlloc(int size);

void UsrMemFree(void);
void UsrMemEndFree(void);

#endif