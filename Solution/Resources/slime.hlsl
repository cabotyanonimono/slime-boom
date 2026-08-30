#include "Light.hlsli"
#include "engine.hlsli"
#include "slime.hlsli"

#include "dithering.hlsli"
#include "math.hlsli"

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

StructuredBuffer<SlimeData> slimes : register(t4);
Texture2D MainTex : register (t5);

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
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
    local_pos.xyz += slimes[instance_id].position;

    float4 world_pos = mul(World, local_pos);
    float4 proj_pos = mul(Proj, mul(View, world_pos));

    float3 world_normal = mul((float3x3)World, local_normal);

    output.svpos = proj_pos;
    output.normal = normalize(world_normal);
    output.color = input.color;
    output.uv = input.uv;
    output.worldpos = world_pos.xyz;
    output.instance_id = instance_id;
    return output;
}

float4 pix(VSOutput input) : SV_Target
{
    Dithering(input.svpos.xy, input.worldpos, camera_pos, player_pos, dithering_radius);
    
    float3 N = normalize(input.normal);
    float3 brightness = float3(0, 0, 0);
    if (light_count == 0)
    {
        float2 flippedUV = float2(input.uv.x, 1.0 - input.uv.y);
        float4 mainColor = MainTex.Sample(smp, flippedUV);
        return float4(mainColor.rgb, mainColor.a);
    }

    float4 viewPos = mul(View, float4(input.worldpos, 1.0));
    float depth = abs(viewPos.z);

    brightness = CalcAllShadow(depth, normalize(input.normal), input.worldpos);

    float4 main_color;

    if (slimes[input.instance_id].slime_type == SLIME_TYPE::NORMAL)
        main_color = slimes[input.instance_id].damage_color_timer <= 0.0f
                         ? MainTex.Sample(smp, input.uv)
                         : damage_color;
    else if(slimes[input.instance_id].slime_type == SLIME_TYPE::SPEED)
        main_color = slimes[input.instance_id].damage_color_timer <= 0.0f
                         ? float4(1.0f, 1.0f, 1.0f, 1.0f)
                         : damage_color;
    else if(slimes[input.instance_id].slime_type == SLIME_TYPE::TANK)
        main_color = slimes[input.instance_id].damage_color_timer <= 0.0f
                                 ? float4(0.5f, 0.0f, 0.5f, 1.0f)
                                 : damage_color;

    return float4(main_color.rgb * brightness, main_color.a);
}
