#define SLIME_TYPE_PARAMS_REGISTER t5
#include "Light.hlsli"
#include "engine.hlsli"
#include "slime.hlsli"

#include "dithering.hlsli"
#include "math.hlsli"
#include "cabo_pbr.hlsli"

cbuffer DamageColor : register(b0)
{
    float4 damage_color;
}

cbuffer PlayerPos : register(b1)
{
    float3 player_pos;
}

cbuffer DitheringRadius : register(b2)
{
    float dithering_radius;
}

cbuffer Intensity : register(b3)
{
    float intensity;
    float metallic;
    float roughness;
    float env_intensity; // 環境(景色)反射の強さ
    float ambient_influence; // 環境光の影響度 (1 = そのまま, 0 = 環境光なし)
}

StructuredBuffer<SlimeData> slimes : register(t0);
Texture2D base_texture : register(t1);
Texture2D speed_texture : register(t2);
Texture2D tank_texture : register(t3);
TextureCube env_texture : register(t4);

// 種類の表の texture_index で、使うテクスチャを選ぶ。テクスチャを足すときはここに追加する。
float4 SampleSlimeTexture(uint texture_index, float2 uv)
{
    if (texture_index == 1)
        return speed_texture.Sample(smp, uv);
    if (texture_index == 2)
        return tank_texture.Sample(smp, uv);
    return base_texture.Sample(smp, uv);
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
    uint instance_id : Instance_ID;
};

// スライムの頂点をワールド座標に変換する（VSMainとVSShadowで共通）
// rot: 進行方向への回転（法線の変換に使う）
float4 SlimeToWorld(float3 pos, uint instance_id, out float3x3 rot)
{
    float4 local_pos = float4(pos, 1.0f);

    local_pos.xyz = mul(EulerToRotationMatrix(slimes[instance_id].rotation), local_pos.xyz);

    local_pos.xyz *= GetSlimeTypeParams(slimes[instance_id].slime_type).size_scale;

    rot = LookRotation(normalize(float3(slimes[instance_id].velocity.x, 0.0f, slimes[instance_id].velocity.z)), float3(0, 1, 0));
	local_pos.z += sin(time.x * 5.0f + local_pos.y * 0.03f + instance_id) * 5.15f;
	local_pos.y += sin(time.x * 5.0f + local_pos.z * 0.03f + instance_id) * 5.15f;
	local_pos.xyz = mul(rot, local_pos.xyz);

    float4 world_pos = mul(World, local_pos);
    world_pos.xyz += slimes[instance_id].position;
    return world_pos;
}

VSOutput VSMain(VSInput input, uint instance_id : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    float3x3 rot;
    float4 world_pos = SlimeToWorld(input.pos, instance_id, rot);
    float3 local_normal = mul(rot, input.normal);

    float4 proj_pos = mul(Proj, mul(View, world_pos));

    float3 world_normal = mul((float3x3)World, local_normal);
    float3 world_tangent = mul((float3x3)World, input.tangent);

    output.svpos = proj_pos;
    output.normal = normalize(world_normal);
    output.tangent = normalize(world_tangent);
    output.tangent_sign = input.tangent.w;
    output.color = input.color;
    output.uv = input.uv;
    output.worldpos = world_pos.xyz;
    output.instance_id = instance_id;
    return output;
}

float4 PSMain(VSOutput input) : SV_Target
{
    float3 normal = normalize(input.normal);
    Dithering(input.svpos.xy, input.worldpos, camera_pos, player_pos, dithering_radius);
    float4 viewPos = mul(View, float4(input.worldpos, 1.0));
    float depth = abs(viewPos.z);
    int cascade_index = CalcCascadeIndex(depth);
    int shadow_map_offset = 0;

    SlimeTypeParams type_params = GetSlimeTypeParams(slimes[input.instance_id].slime_type);
    float4 albedo_color = slimes[input.instance_id].damage_color_timer <= 0.0f
                              ? SampleSlimeTexture(type_params.texture_index, input.uv) * type_params.albedo_tint
                              : damage_color;

    // 直接光: Σ BRDF × N·L × radiance
    float3 direct_light = float3(0.0f, 0.0f, 0.0f);
    float3 view_dir = normalize(camera_pos - input.worldpos);
    for (int i = 0; i < light_count; ++i)
    {
        float3 light_dir;
        float3 radiance = CalcLightRadiance(GetLight(i), input.worldpos, cascade_index, shadow_map_offset, light_dir);
        direct_light += CalcLighting(albedo_color.rgb, metallic, roughness, normal, light_dir, view_dir) * radiance;
    }
    direct_light *= intensity;

    float3 r = reflect(-view_dir, normal);
    float3 env_specular = env_texture.Sample(smp, r).rgb;

    float n_dot_v = saturate(dot(normal, view_dir));
    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo_color.rgb, metallic);
    float3 f = min(FresnelSchlick(n_dot_v, f0), 1.0f);
    float3 ibl_specular = env_specular * f * (1.0f - roughness) * env_intensity;

    float3 final_color = direct_light + ibl_specular;
    final_color += PbrAmbientLight(albedo_color.rgb, f, metallic) * ambient_influence;
    return float4(final_color, albedo_color.a);
}

// 影用。VSMainと同じ位置に変形して深度だけを書く（view_projectionはライトの行列）
float4 VSShadow(VSInput input, uint instance_id : SV_InstanceID) : SV_POSITION
{
    float3x3 rot;
    float4 world_pos = SlimeToWorld(input.pos, instance_id, rot);
    return mul(view_projection, world_pos);
}
