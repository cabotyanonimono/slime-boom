#include "engine.hlsli"
#include "Light.hlsli"

cbuffer z : register(b2)
{
    float z;
}

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput vrt(VSInput input)
{
    VSOutput output;
    
    float4 worldPos = mul(World, float4(input.pos, 1.0f));
    
    output.svpos = worldPos;
    output.uv = input.uv;

    return output;
}

float4 pix(VSOutput input) : SV_TARGET
{
    float4 mainColor = ShadowMaps.Sample(smp, float3(input.uv, z));
    return mainColor;
}