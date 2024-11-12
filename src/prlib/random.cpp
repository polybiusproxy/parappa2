#include "random.h"

extern u_int randomSeed;
extern u_int randomPool[97];

static u_int RawRandom()
{
    randomSeed = (randomSeed * 0x5d588b65 + 1);
    return randomSeed;
}

int PrRandom()
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

#define FLOAT_RAND 4.656613e-10f

float PrFloatRandom()
{
    float ret;
    u_int rand;

generate_rand:
    rand = PrRandom();
    if (rand >= 0)
    {
        ret = rand;
    }
    else
    {
        ret = ((rand & 1) | (rand >> 1));
        ret += ret;
    }

    if ((ret * FLOAT_RAND) >= 1.0f)
        goto generate_rand;

    ret *= FLOAT_RAND;
    return ret;
}
