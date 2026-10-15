#include "engine.hlsli"

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv    : TEXCOORD0;
};

cbuffer Ratio : register(b2)
{
    float ratio;
}

cbuffer Rotation : register(b3)
{
    float rotation;
}

cbuffer Alpha : register(b4)
{
    float alpha;
}

Texture2D tex : register(t3);

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
    float2 flippedUV = float2(input.uv.x, 1.0 - input.uv.y);
    float4 mainColor = tex.Sample(smp, flippedUV);

    float2 p = input.uv - float2(0.5f, 0.5f);

    float2 dir;

    float rad = rotation / 180.0f * 3.14159f;

    dir.x = cos(rad);
    dir.y = sin(rad);

    float proj = dot(p, dir);

    float proj01 = proj + 0.5f;

    if(proj01 < ratio)
        return float4(mainColor.rgb, mainColor.a * alpha);
    
    return float4(0,0,0,0);
}