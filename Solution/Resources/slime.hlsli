#ifndef __SLIME_HLSLI__
#define __SLIME_HLSLI__

namespace SLIME_TYPE
{
static const int NORMAL = 0;
static const int SPEED = 1;
static const int TANK = 2;
}

struct SlimeData
{
    float3 position;
    float3 rotation;
    float3 scale;
    float3 velocity;
    float3 angular_velocity;
    uint hp;
    float damage_color_timer;
    int slime_type;
};

#endif