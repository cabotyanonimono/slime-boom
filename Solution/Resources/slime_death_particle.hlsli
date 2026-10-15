#ifndef __SLIME_DEATH_PARTICLE_HLSLI__
#define __SLIME_DEATH_PARTICLE_HLSLI__

// スライム撃破時のパーティクル
// C++側の SlimeDeathParticleData / kMaxParticleCount と合わせること
#define DEATH_PARTICLE_MAX 4096        // 2の累乗にすること（カウンタの折り返しでもリングが崩れないように）
#define DEATH_PARTICLE_PER_SLIME 10    // 1体倒したときに出す数

#define DEATH_PARTICLE_STATE_DEAD 0
#define DEATH_PARTICLE_STATE_SPAWNED 1 // 生成直後（velocityに単位方向ベクトルが入っている）
#define DEATH_PARTICLE_STATE_ALIVE 2

struct SlimeDeathParticle
{
    float3 position;
    float age;
    float3 velocity;
    float random;
    int slime_type;
    uint state;
    float floor_y;
    float life_ratio; // 0:生成直後 -> 1:消滅
};

#endif
