#ifndef __SLIME_HLSLI__
#define __SLIME_HLSLI__

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

// スライム1種類分のパラメータ。C++側の SlimeTypeParams(slime_types.h)と同じ並び・大きさにすること。
// 種類ごとの違いは、種類の番号で分岐せずにこの値で表す。
struct SlimeTypeParams
{
    float4 death_color;
    float4 albedo_tint;
    uint max_hp;
    float speed_scale;
    float size_scale; 
    uint texture_index;
};
 
#ifdef SLIME_TYPE_PARAMS_REGISTER
StructuredBuffer<SlimeTypeParams> slime_type_params : register(SLIME_TYPE_PARAMS_REGISTER);

uint GetSlimeTypeCount()
{
    uint count, stride;
    slime_type_params.GetDimensions(count, stride);
    return count;
}

// 種類の番号のパラメータを返す。表から種類が減って範囲外になった番号は、最後の種類として扱う。
SlimeTypeParams GetSlimeTypeParams(int slime_type)
{
    uint last = max(GetSlimeTypeCount(), 1u) - 1u;
    return slime_type_params[min((uint)max(slime_type, 0), last)];
}
#endif

#endif
