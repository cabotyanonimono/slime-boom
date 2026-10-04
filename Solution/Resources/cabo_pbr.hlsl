#include "engine.hlsli"
#include "Light.hlsli"
#include "util.hlsli"
#include "cabo_pbr.hlsli"

Texture2D albedo_texture : register (t4);
Texture2D normal_texture : register (t5);
TextureCube env_texture : register (t6);

cbuffer Params : register (b5)
{
    float2 tiling;
    float intensity;
    float4 emission_color;
    float emission_intensity;
    float metallic;
    float roughness;
}


struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float tangent_sign : TANGENT_SIGN;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 worldpos : TEXCOORD1;
};

float3x3 ExtractRotation(float4x4 m)
{
    return float3x3(
        m[0].xyz,
        m[1].xyz,
        m[2].xyz
    );
}

static const float4x4 identityMatrix = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

VSOutput vrt(VSInput input)
{
    VSOutput output = (VSOutput)0;

    float4 local_pos = float4(input.pos, 1.0f);
    float3 local_normal = input.normal;
    float3 local_tangent = input.tangent;

    float4 skinned_pos = 0;
    float3 skinned_normal = 0;
    float3 skinned_tangent = 0;

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        float w = input.bone_weight[i];
        if (w > 0)
        {
            float4x4 boneMatrix = BoneMatrices[input.bone_id[i]];
            skinned_pos += mul(boneMatrix, local_pos) * w;
            skinned_normal += mul((float3x3)boneMatrix, local_normal) * w;
            skinned_tangent += mul((float3x3)boneMatrix, local_tangent) * w;
        }
    }

    if (input.bones_per_vertex != 0)
    {
        local_pos = skinned_pos;
        local_normal = skinned_normal;
        local_tangent = skinned_tangent;
    }

    float4 worldPos = mul(World, local_pos);
    float4 projPos = mul(Proj, mul(View, worldPos));

    float3 worldNormal = mul((float3x3)World, local_normal);
    float3 worldTangent = mul((float3x3)World, local_tangent);

    output.svpos = projPos;
    output.normal = normalize(worldNormal);
    output.color = input.color;
    output.uv = input.uv;
    output.worldpos = worldPos.xyz;
    output.tangent = normalize(worldTangent);
    output.tangent_sign = input.tangent.w;
    return output;
}

float4 pix(VSOutput input) : SV_TARGET
{
    float2 uv = input.uv * tiling;
    float3 normal = CalcNormalWithNormalMap(normal_texture, uv, input.normal, input.tangent, input.tangent_sign);
    float4 albedo_color = albedo_texture.Sample(smp, uv); 
    if (light_count == 0)
        return albedo_color;
    
    float4 viewPos = mul(View, float4(input.worldpos, 1.0));
    float depth = abs(viewPos.z);
    float3 brightness = CalcAllShadow(depth, normal, input.worldpos) * intensity;

    float3 view_dir = normalize(camera_pos - input.worldpos);
    
    float3 direct_light = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < light_count; ++i)
    {
        float3 light_dir;
        if (Lights[i].type == 0)
            light_dir = normalize(-Lights[i].direction);
        else
            light_dir = normalize(Lights[i].pos - input.worldpos);
        direct_light += CalcLighting(albedo_color.rgb, metallic, roughness, normal, light_dir, view_dir);
    }

    float3 r = reflect(-view_dir, normal);
    float3 env_specular = env_texture.Sample(smp, r).rgb;

    float n_dot_v = saturate(dot(normal, view_dir));
    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo_color.rgb, metallic);
    float3 f = min(FresnelSchlick(n_dot_v, f0), 1.0f);
    float3 ibl_specular = env_specular * f * (1.0f - roughness);

    float3 final_color = (direct_light + ibl_specular) * brightness;
    final_color += emission_color.rgb * emission_intensity;

    return float4(final_color, albedo_color.a);
}
