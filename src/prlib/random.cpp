#include "random.h"

#include <stdlib.h>

extern u_int randomSeed;
extern u_int randomPool[97];

static u_int RawRandom()
{
    randomSeed = (randomSeed * 0x5d588b65 + 1);
    return randomSeed;
}

u_int PrRandom()
{
    extern u_int poolIndex;
    int ret;

    poolIndex = randomPool[poolIndex] % 97;
    ret = randomPool[poolIndex] / 2;

    randomPool[poolIndex] = RawRandom();
    return ret;
}

void PrInitializeRandomPool()
{
    u_int i;

    for (i = 0; i < 97; i++)
    {
        randomPool[i] = RawRandom();
    }

    PrRandom();
    PrRandom();
}

float PrFloatRandom()
{
    float ret;
    u_int rand;

generate_rand:
    ret = PrRandom();

    if ((ret / RAND_MAX) >= 1.0f)
        goto generate_rand;

    return ret / RAND_MAX;
}
