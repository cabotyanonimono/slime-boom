#include"engine.hlsli"
#include"Light.hlsli"

static const float PI = 3.14159265f;
static const float MAX_REFLECTION_LOD = 0.0f;

cbuffer Params : register(b5)
{
    float2 tiling;
    float intensity;
    float4 emission_color;
    float emission_intensity;
    float metallic;
    float roughness;
};

Texture2D albedo_texture    : register(t4);
Texture2D normal_texture    : register(t5);
TextureCube env_texture     : register(t6);

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

float3 CalcNormalWithNormalMap(Texture2D normalMap, float2 uv, float3 normal, float3 tangent, float tangentSign)
{
    float3 mapNormal = normalMap.Sample(smp, uv).xyz * 2.0f - 1.0f;
    float3 N = normalize(normal);
    float3 T = normalize(tangent);
    float3 B = normalize(cross(N, T)) * tangentSign;
    float3x3 TBN = float3x3(T, B, N);
    return normalize(mul(mapNormal, TBN));
}

float3 FresnelSchlick(float costheta, float3 f0)
{
    return f0 + (1.0f - f0) * pow(saturate(1.0f - costheta), 5.0f);
}

float DistributionGGX(float n_dot_h, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float n_dot_h2 = n_dot_h * n_dot_h;
    float denom = (n_dot_h2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
    return a2 / max(denom, 0.0001f);
}

float GeometrySchlickGGX(float n_dot_v, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    return n_dot_v / (n_dot_v * (1.0f - k) + k);
}

float GeometrySmith(float n_dot_v, float n_dot_l, float roughness)
{
    float ggx1 = GeometrySchlickGGX(n_dot_v, roughness);
    float ggx2 = GeometrySchlickGGX(n_dot_l, roughness);
    return ggx1 * ggx2;
}

float3 CalcLighting(float3 albedo, float metallic, float roughness, float3 normal, float3 light_dir, float3 view_dir)
{
    float3 n = normalize(normal);
    float3 v = normalize(view_dir);
    float3 l = normalize(light_dir);
    float3 h = normalize(v + l);

    float n_dot_l = saturate(dot(n, l));
    float n_dot_v = saturate(dot(n, v));
    float n_dot_h = saturate(dot(n, h));
    float h_dot_v = saturate(dot(h, v));

    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metallic);

    float3 f = FresnelSchlick(h_dot_v, f0);
    float d = DistributionGGX(n_dot_h, max(roughness, 0.0001f));
    float g = GeometrySmith(n_dot_v, n_dot_l, roughness);

    float3 direct_specular = (f * d * g) / (4.0f * n_dot_l * n_dot_v + 0.0001f);

    float3 kS = f;
    float3 kD = (float3(1.0f, 1.0f, 1.0f) - kS) * (1.0f - metallic);
    float3 direct_diffuse = kD * albedo / PI;

    return (direct_diffuse + direct_specular) * n_dot_l;
}

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
    }

    float4 worldPos = mul(World, local_pos);
    float4 projPos = mul(Proj, mul(View, worldPos));

    float3 worldNormal = mul((float3x3)World, local_normal);
    float3 worldTangent = mul((float3x3)World, input.tangent);

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
    
    float4 albedo_sample = albedo_texture.Sample(smp, uv);
    float3 albedo = albedo_sample.rgb;

    if (light_count == 0)
    {
        float2 flippedUV = float2(input.uv.x, 1.0 - input.uv.y);
        float4 mainColor = albedo_texture.Sample(smp, flippedUV);
        return float4(mainColor.rgb, mainColor.a);
    }

    float3 view_dir = normalize(camera_pos - input.worldpos);

    // 1. 直接光の累積計算
    float3 total_direct_light = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < light_count; ++i)
    {
        float3 light_dir;
        if (Lights[i].type == 0)
            light_dir = normalize(-Lights[i].direction);
        else
            light_dir = normalize(Lights[i].pos - input.worldpos);

        total_direct_light += CalcLighting(albedo, metallic, roughness, normal, light_dir, view_dir);
    }

    // 2. 環境光 (Specular IBL) の計算
    float3 r = reflect(-view_dir, normal);
    float mip_level = roughness * MAX_REFLECTION_LOD;
    float3 env_specular = env_texture.SampleLevel(smp, r, mip_level).rgb;

    float n_dot_v = saturate(dot(normal, view_dir));
    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metallic);
    float3 f = FresnelSchlick(n_dot_v, f0);
    float3 ibl_specular = env_specular * f * (1.0f - roughness);

    // 3. 影の適用
    float4 viewPos = mul(View, float4(input.worldpos, 1.0));
    float depth = abs(viewPos.z);
    float3 shadow_brightness = CalcAllShadow(depth, normal, input.worldpos) * intensity;

    // 4. 最終カラーの合成
    float3 final_color = (total_direct_light + ibl_specular) * shadow_brightness;
    final_color += emission_color.rgb * emission_intensity;

    return float4(final_color, albedo_sample.a);
}