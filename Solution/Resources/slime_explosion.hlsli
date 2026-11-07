#ifndef __SLIME_EXPLOSION_HLSLI__
#define __SLIME_EXPLOSION_HLSLI__

#define SLIME_EXPLOSION_MAX 128
#define SLIME_EXPLOSION_HEADER 0

struct SlimeExplosion
{
    float3 position;
    uint generation;
    uint frame;
    float3 padding;
};

#endif
