#ifndef __LIGHT_HLSLI__
#define __LIGHT_HLSLI__

// 旧エンジンのライト関数を、CaboRenderingSystem(CRS)のライト(lights, shadow_maps)で動かす互換レイヤー。

#include "engine.hlsli"

// 旧エンジンのライトの並び。CRSのGpuLightから詰め替える(GetLight)
struct Light
{
    int type;
    int cast_shadow;
    float intensity;
    float range;

    float3 pos;
    float3 direction;
    float4 color;

    float inner_cos;
    float outer_cos;
    // 影の行列の先頭。平行光源はカスケード分だけ続く
    int shadow_index;
};

Light GetLight(uint index)
{
    const GpuLight src = lights[index];
    Light light;
    light.type = src.type;
    light.cast_shadow = src.cast_shadow;
    light.intensity = src.intensity;
    light.range = src.range;
    light.pos = src.position;
    light.direction = src.direction;
    light.color = src.color;
    light.inner_cos = src.inner_cos;
    light.outer_cos = src.outer_cos;
    light.shadow_index = src.shadow_index;
    return light;
}

float CalcShadow(float3 world_pos, int shadow_map_index)
{
    return SampleShadow(world_pos, shadow_map_index);
}

int CalcCascadeIndex(float depth)
{
    return SelectCascade(depth);
}

// 1ライト分の入射光 (色 × 強度 × 減衰 × 影) を返す。N·L は含まない (BRDF側で掛ける)
// shadow_map_offset は旧エンジンとの互換のために残している（CRSではライトが影の位置を持つので使わない）
float3 CalcLightRadiance(Light light, float3 world_pos, int cascade_index, inout int shadow_map_offset, out float3 light_dir)
{
    float3 radiance = float3(0, 0, 0);
    int shadow_map_index = 0;
    light_dir = float3(0, 1, 0);

    switch (light.type)
    {
    case LIGHT_TYPE_DIRECTIONAL:
        light_dir = normalize(-light.direction);
        radiance = light.color.rgb * light.intensity;
        shadow_map_index = light.shadow_index + cascade_index;
        break;
    case LIGHT_TYPE_SPOT:
        {
            float3 to_light = light.pos - world_pos;
            float dist = max(length(to_light), 0.0001f);
            light_dir = to_light / dist;
            float spot_factor = dot(light_dir, -light.direction);
            float smooth_edge = saturate((spot_factor - light.outer_cos) / max(light.inner_cos - light.outer_cos, 0.0001f));
            float attenuation = saturate(1.0 - dist / light.range);
            radiance = light.color.rgb * light.intensity * smooth_edge * attenuation;
            shadow_map_index = light.shadow_index;
        }
        break;
    default:
        return radiance;
    }

    if (light.cast_shadow != 0)
        radiance *= CalcShadow(world_pos, shadow_map_index);
    return radiance;
}

// 全ライトの拡散光 (N·L × 色 × 強度 × 影) の合計。環境光は含まない (AmbientLight() を足すこと)
float3 CalcAllShadow(float depth, float3 normal, float3 world_pos)
{
    const int cascade_index = CalcCascadeIndex(depth);
    int shadow_map_offset = 0;

    float3 brightness = float3(0, 0, 0);
    for (uint i = 0; i < light_count; ++i)
    {
        float3 light_dir;
        const float3 radiance = CalcLightRadiance(GetLight(i), world_pos, cascade_index, shadow_map_offset, light_dir);
        brightness += saturate(dot(normal, light_dir)) * radiance;
    }
    return brightness;
}

#endif
