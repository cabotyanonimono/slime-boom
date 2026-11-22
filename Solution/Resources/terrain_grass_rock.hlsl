// TerrainGrid用のシェーダー。
// 法線が上を向いている面(平地・坂)は草、それ以外(崖・段差の側面)は岩にする。
#include "engine.hlsli"
#include "Light.hlsli"
#include "util.hlsli"
#include "cabo_pbr.hlsli"

Texture2D grass_albedo : register(t0);
Texture2D grass_normal : register(t1);
Texture2D rock_albedo : register(t2);
Texture2D rock_normal : register(t3);

cbuffer Params : register(b0)
{
    float2 grass_tiling; // 1セルあたりの繰り返し数
    float2 rock_tiling;
    float grass_threshold; // normal.yがこれ以上なら草
    float blend_width; // 草と岩の境目のぼかし幅(normal.y)
    float grass_roughness;
    float rock_roughness;
    float intensity;
    float ambient_influence; // 環境光の影響度 (1 = そのまま, 0 = 環境光なし)
}

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float tangent_sign : TANGENT_SIGN;
    float2 uv : TEXCOORD0;
    float3 worldpos : TEXCOORD1;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput)0;

    float4 world_pos = mul(World, float4(input.pos, 1.0f));
    output.svpos = mul(Proj, mul(View, world_pos));
    output.normal = normalize(mul((float3x3)World, input.normal));
    output.tangent = normalize(mul((float3x3)World, input.tangent.xyz));
    output.tangent_sign = input.tangent.w;
    output.uv = input.uv;
    output.worldpos = world_pos.xyz;
    return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
    float3 geometry_normal = normalize(input.normal);

    float2 grass_uv = input.uv * grass_tiling;
    float2 rock_uv = input.uv * rock_tiling;
    float4 grass_color = grass_albedo.Sample(smp, grass_uv);
    float4 rock_color = rock_albedo.Sample(smp, rock_uv);

    // 境目は岩の明るさで少し崩して、まっすぐな線にならないようにする
    float rock_height = dot(rock_color.rgb, float3(0.333f, 0.333f, 0.333f));
    float half_width = max(blend_width, 0.0001f) * 0.5f;
    float grass_weight = smoothstep(grass_threshold - half_width, grass_threshold + half_width,
                                    geometry_normal.y + (rock_height - 0.25f) * blend_width);

    float3 grass_n = CalcNormalWithNormalMap(grass_normal, grass_uv, geometry_normal, input.tangent, input.tangent_sign);
    float3 rock_n = CalcNormalWithNormalMap(rock_normal, rock_uv, geometry_normal, input.tangent, input.tangent_sign);
    float3 normal = normalize(lerp(rock_n, grass_n, grass_weight));

    float3 albedo = lerp(rock_color.rgb, grass_color.rgb, grass_weight);
    float roughness = lerp(rock_roughness, grass_roughness, grass_weight);
    const float metallic = 0.0f;

    if (light_count == 0)
        return float4(albedo, 1.0f);

    float4 view_pos = mul(View, float4(input.worldpos, 1.0));
    int cascade_index = CalcCascadeIndex(abs(view_pos.z));
    int shadow_map_offset = 0;

    float3 view_dir = normalize(camera_pos - input.worldpos);

    float3 direct_light = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < light_count; ++i)
    {
        float3 light_dir;
        float3 radiance = CalcLightRadiance(GetLight(i), input.worldpos, cascade_index, shadow_map_offset, light_dir);
        direct_light += CalcLighting(albedo, metallic, roughness, normal, light_dir, view_dir) * radiance;
    }
    direct_light *= intensity;

    float n_dot_v = saturate(dot(normal, view_dir));
    float3 f = min(FresnelSchlick(n_dot_v, float3(0.04f, 0.04f, 0.04f)), 1.0f);

    float3 final_color = direct_light + PbrAmbientLight(albedo, f, metallic) * ambient_influence;
    return float4(final_color, 1.0f);
}
