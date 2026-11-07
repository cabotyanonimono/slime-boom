#ifndef __SLIME_EXPLOSION_PARTICLE_HLSLI__
#define __SLIME_EXPLOSION_PARTICLE_HLSLI__

#include "slime_explosion.hlsli"

// 連鎖爆発のパーティクル。爆発1つにつき決まった数を、爆発バッファのスロットと同じ並びで持つ
// C++側の SlimeExplosionParticleData / kMaxParticleCount と合わせること
#define EXPLOSION_PARTICLE_PER_EXPLOSION 24
#define EXPLOSION_PARTICLE_MAX (SLIME_EXPLOSION_MAX * EXPLOSION_PARTICLE_PER_EXPLOSION)

#define EXPLOSION_PARTICLE_STATE_DEAD 0
#define EXPLOSION_PARTICLE_STATE_ALIVE 1

struct SlimeExplosionParticle
{
    float3 position;
    float age;
    float3 velocity;
    float random;
    uint state;
    float floor_y;
    float life_ratio; // 0:生成直後 -> 1:消滅
    uint spawned_frame; // 最後に生成元になった爆発のframe。同じ爆発から二重に生成しないために使う
};

#endif
