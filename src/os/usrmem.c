#include "os/usrmem.h"

/* static - bss */
extern char usrMemoryData[25690112]; // 25 MB
extern u_int usrMemPos[2048];

/* static - sbss */
static int usrMemPosCnt;
static int usrMemPosEndCnt;

void UsrMemClear(void)
{
    usrMemPos[0] = (u_int)usrMemoryData;
    usrMemPos[2047] = (u_int)usrMemoryData + sizeof(usrMemoryData);
    
    usrMemPosCnt = 0;
    usrMemPosEndCnt = 0;
}

void UsrMemClearTop(void)
{
    usrMemPosCnt = 0;
    usrMemPos[0] = (u_int)usrMemoryData;
}

void UsrMemClearEnd(void)
{
    usrMemPosEndCnt = 0;
    usrMemPos[2047] = (u_int)usrMemoryData + sizeof(usrMemoryData);
}

u_int UsrMemGetAdr(int id)
{
    if (id >= usrMemPosCnt)
    {
        printf("UsrMemGetAdr ID over[%d]\n", id);
        return 0;
    }
    else
    {
        return usrMemPos[id];
    }
}

u_int UsrMemGetSize(int id)
{
    if (id >= usrMemPosCnt)
    {
        printf("UsrMemGetSize ID over[%d]\n", id);
        return 0;
    }
    else
    {
        return usrMemPos[id + 1] - usrMemPos[id];
    }
}

u_int UsrMemGetEndAdr(int id)
{   
    if (id >= usrMemPosEndCnt)
    {
        printf("UsrMemGetEndAdr ID over[%d]\n", id);
        return 0;
    } 
    else
    {
        return usrMemPos[2047 - id];
    }
}

u_int UsrMemGetEndSize(int id)
{   
    if (id >= usrMemPosEndCnt)
    {
        printf("UsrMemGetEndSize ID over[%d]\n", id);
        return 0;
    }
    else
    {
        return usrMemPos[2047 - id] - usrMemPos[2046 - id];
    }
}

u_int UsrMemAllocNext(void)
{
    return usrMemPos[usrMemPosCnt];
}

u_int UsrMemAllocEndNext(void)
{
    return usrMemPos[2047 - usrMemPosEndCnt];
}

u_int UsrMemAlloc(int size)
{
    u_int ret = UsrMemAllocNext();

    usrMemPosCnt++;
    usrMemPos[usrMemPosCnt] = ret + ((size + 15) / 16) * 16;

    if (usrMemPos[usrMemPosCnt] > UsrMemAllocEndNext())
    {
        printf("UsrMemAlloc size over [0x%08x]\n", usrMemPos[usrMemPosCnt] - UsrMemAllocEndNext());
        usrMemPosCnt--;

        return 0;
    }

    return ret;
}

u_int UsrMemEndAlloc(int size)
{
    u_int ret = UsrMemAllocEndNext() - ((size + 15) / 16) * 16;
    
    if (ret < UsrMemAllocNext())
    {
        printf("UsrMemEndAlloc size over [0x%08x]\n", UsrMemAllocNext() - ret);
        return 0;
    }
    else
    {
        usrMemPosEndCnt++;
        usrMemPos[2047 - usrMemPosEndCnt] = ret;
    }

    return ret;
}

void UsrMemFree(void)
{
    if (usrMemPosCnt)
        usrMemPosCnt--;
}

void UsrMemEndFree(void)
{
    if (usrMemPosEndCnt)
        usrMemPosEndCnt--;
}
