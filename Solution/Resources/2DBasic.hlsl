#include "engine.hlsli"
#include "Light.hlsli"

Texture2D _MainTex : register (t3);

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
    float4 mainColor = _MainTex.Sample(smp, input.uv);
    return mainColor;
}