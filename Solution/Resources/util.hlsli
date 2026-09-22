#ifndef __UTIL_HLSLI__
#define __UTIL_HLSLI__
#include "engine.hlsli"

#define PI 3.1415926535897932384626433832795f

float3 CalcNormalWithNormalMap(Texture2D normal_map, float2 uv, float3 normal, float3 tangent, float tangent_sign)
{
    float3 raw_color = normal_map.Sample(smp, uv).rgb;
    float3 local_normal = raw_color * 2.0f - 1.0f;

    float3 n = normalize(normal);
    float3 t = normalize(tangent - n * dot(n, tangent));
    float3 b = cross(n, t) * tangent_sign;

    float3x3 tbn = float3x3(t, b, n);
    float3 world_normal = normalize(mul(local_normal, tbn));

    return world_normal;
}

float CalcSpecularPowerWithSpecularMap(Texture2D specular_map, float2 uv, float3 light_dir, float3 camera_dir,
                                       float3 normal)
{
    float3 h = light_dir + camera_dir;
    float spec_val = specular_map.Sample(smp, uv).r;
    float power = 1.0f;
    float spec = pow(max(1e-4, saturate(dot(normal, h))), power);

    float n_dot_l = saturate(dot(normal, light_dir));
    return spec * n_dot_l * spec_val;
}

#endif
