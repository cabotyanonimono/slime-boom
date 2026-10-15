#include "Light.hlsli"
#include "engine.hlsli"
#include "slime.hlsli"

#include "dithering.hlsli"
#include "math.hlsli"
#include "cabo_pbr.hlsli"

cbuffer DamageColor : register(b5)
{
    float4 damage_color;
}

cbuffer PlayerPos : register(b6)
{
    float3 player_pos;
}

cbuffer DitheringRadius : register(b7)
{
    float dithering_radius;
}

cbuffer Intensity : register(b8)
{
    float intensity;
}

StructuredBuffer<SlimeData> slimes : register(t4);
Texture2D MainTex : register (t5);
TextureCube env_texture : register (t6);

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

VSOutput vrt(VSInput input, uint instance_id : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    float4 local_pos = float4(input.pos, 1.0f);
    float3 local_normal = input.normal;

    local_pos.xyz = mul(EulerToRotationMatrix(slimes[instance_id].rotation), local_pos.xyz);

    if(slimes[instance_id].slime_type == SLIME_TYPE::TANK)
    {
        local_pos.xyz *= 1.5f;
    }
	//slimeを揺らす処理
    local_pos.y += sin(time * 5.0f + input.pos.x + instance_id) * 0.15f;

    local_pos.xyz += slimes[instance_id].position;

    float4 world_pos = mul(World, local_pos);
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

float4 pix(VSOutput input) : SV_Target
{
    float3 normal = normalize(input.normal);
    Dithering(input.svpos.xy, input.worldpos, camera_pos, player_pos, dithering_radius);
    float3 brightness = float3(0, 0, 0);

    float4 viewPos = mul(View, float4(input.worldpos, 1.0));
    float depth = abs(viewPos.z);

    brightness = CalcAllShadow(depth, normal, input.worldpos);

    float4 albedo_color;

    if (slimes[input.instance_id].slime_type == SLIME_TYPE::NORMAL)
        albedo_color = slimes[input.instance_id].damage_color_timer <= 0.0f
                         ? MainTex.Sample(smp, input.uv)
                         : damage_color;
    else if(slimes[input.instance_id].slime_type == SLIME_TYPE::SPEED)
        albedo_color = slimes[input.instance_id].damage_color_timer <= 0.0f
                         ? float4(1.0f, 1.0f, 1.0f, 1.0f)
                         : damage_color;
    else if(slimes[input.instance_id].slime_type == SLIME_TYPE::TANK)
        albedo_color = slimes[input.instance_id].damage_color_timer <= 0.0f
                                 ? float4(0.5f, 0.0f, 0.5f, 1.0f)
                                 : damage_color;

    float3 direct_light;
    float3 view_dir = normalize(camera_pos - input.worldpos);
    for (int i = 0; i < light_count; ++i)
    {
        float3 light_dir;
        if (Lights[i].type == 0)
            light_dir = normalize(-Lights[i].direction);
        else
            light_dir = normalize(Lights[i].pos - input.worldpos);
        direct_light = CalcLighting(albedo_color.rgb, 0.0f, 1, normal, light_dir, view_dir);
    }

    float3 r = reflect(-view_dir, normal);
    float3 env_specular = env_texture.Sample(smp, r).rgb;

    float n_dot_v = saturate(dot(normal, view_dir));
    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo_color.rgb, 0.0f);
    float3 f = min(FresnelSchlick(n_dot_v, f0), 1.0f);
    float3 ibl_specular = env_specular * f * (1.0f - 1);

    float3 final_color = (direct_light + ibl_specular) * brightness;
    
    brightness.rgb += intensity;
    return float4(final_color * brightness, albedo_color.a); 
}
