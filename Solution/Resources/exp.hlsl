#include "Light.hlsli"
#include "engine.hlsli"
#include "slime.hlsli"
#include "math.hlsli"
#include "exp.hlsli"

Texture2D MainTex : register (t4);
StructuredBuffer<Exp> exps : register(t5);

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 worldpos : TEXCOORD1;
};

VSOutput vrt(VSInput input, uint instance_id : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    float4 local_pos = float4(input.pos, 1.0f);
    float3 local_normal = input.normal;

    if (exps[instance_id].is_active == false)
    {
        output.svpos = float4(-100000000.0f, 0.0f, 0.0f, 0.0f);
        return output;
    }
    
    local_pos.xyz += exps[instance_id].pos;
    
    float4 world_pos = mul(World, local_pos);
    float4 proj_pos = mul(Proj, mul(View, world_pos));

    float3 world_normal = mul((float3x3)World, local_normal);

    output.svpos = proj_pos;
    output.normal = normalize(world_normal);
    output.color = input.color;
    output.uv = input.uv;
    output.worldpos = world_pos.xyz;
    return output;
}

float4 pix(VSOutput input) : SV_Target
{
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

    float4 main_color = MainTex.Sample(smp, input.uv);

    return float4(main_color.rgb * brightness, main_color.a);
}