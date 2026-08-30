#ifndef __LIGHT_HLSLI__
#define __LIGHT_HLSLI__

#include "engine.hlsli"
#define SHADOW_CASCADE_COUNT 3

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
};

cbuffer ShadowCascadeSlices : register(b3)
{
    float4 cascade_slices;
}

cbuffer LightCount : register (b4)
{
    int light_count;
}

StructuredBuffer<Light> Lights : register (t3);

float SampleShadowPCF(float3 shadowCoord, int shadow_map_index)
{
    float shadow = 0.0;
    const float2 texelSize = 1.0 / float2(shadow_map_size.x, shadow_map_size.y);

    shadowCoord.z -= 0.005f;

    [unroll]
    for (int x = -1; x <= 1; x++)
    {
        [unroll]
        for (int y = -1; y <= 1; y++)
        {
            float2 offset = float2(x, y) * texelSize;
            shadow += ShadowMaps.SampleCmpLevelZero(
                shadowSampler,
                float3(shadowCoord.xy + offset, shadow_map_index),
                shadowCoord.z
                );
        }
    }

    shadow /= 9.0;
    return shadow;
}

float CalcShadow(float3 world_pos, int shadow_map_index)
{
    float4 worldPos = float4(world_pos, 1);
    float4 lightClip = mul(LightViewProj[shadow_map_index], worldPos);
    float3 shadowCoord;
    shadowCoord.xy = lightClip.xy / lightClip.w * 0.5f + 0.5f;
    shadowCoord.y = 1 - shadowCoord.y;
    shadowCoord.z = lightClip.z / lightClip.w;

    float shadow = 0;
    if (shadowCoord.x < 0 || shadowCoord.x > 1 ||
        shadowCoord.y < 0 || shadowCoord.y > 1 || shadowCoord.z >= 1.0f)
    {
        shadow = 1.0f;
    }
    else
    {
        shadow = SampleShadowPCF(shadowCoord, shadow_map_index);
    }

    return shadow;
}

float3 CalcDirectionalShadow(Light light, float3 normal, float3 world_pos, int shadow_map_index)
{
    float3 L = normalize(-light.direction);
    float NdotL = saturate(dot(normal, L));
    float3 brightness = 0;

    if (light.cast_shadow == 0)
    {
        brightness += NdotL * light.color.rgb * light.intensity;
        return brightness;
    }

    float shadow = CalcShadow(world_pos, shadow_map_index);
    brightness += shadow * NdotL * light.color.rgb * light.intensity;
    return brightness;
}

float3 CalcSpotShadow(Light light, float3 normal, float3 world_pos, int shadow_map_index)
{
    float3 L = light.pos - world_pos;
    float dist = length(L);
    L /= dist;

    float spotFactor = dot(L, -light.direction);
    float outer = light.outer_cos;
    float inner = light.inner_cos;
    float smoothEdge = saturate((spotFactor - outer) / (inner - outer));

    float attenuation = saturate(1.0 - dist / light.range);
    float3 lighting = smoothEdge * attenuation * max(dot(normal, L), 0) * light.color * light.intensity;
    if (light.cast_shadow == 0)
        return lighting;

    float shadow = CalcShadow(world_pos, shadow_map_index);
    return lighting * shadow;
}

float3 CalcAllShadow(float depth, float3 normal, float3 world_pos)
{
    float3 brightness;
    int cascade_index = SHADOW_CASCADE_COUNT - 1;
    for (int i = 0; i < SHADOW_CASCADE_COUNT; ++i)
    {
        if (depth < cascade_slices[i])
        {
            cascade_index = i;
            break;
        }
    }
    
    int current_shadowmap_count = 0;
    for (int i = 0; i < light_count; ++i)
    {
        switch (Lights[i].type)
        {
        case 0:
            int index = current_shadowmap_count + cascade_index;
            brightness += CalcDirectionalShadow(Lights[i], normal, world_pos, index);
            current_shadowmap_count += SHADOW_CASCADE_COUNT;
            break;
        case 1:
            brightness += CalcSpotShadow(Lights[i], normal, world_pos, current_shadowmap_count);
            current_shadowmap_count += 1;
            break;
        default:
            break;
        }
    }

    return brightness;
}

#endif