#include "Shaders/crs_ui.hlsli"

// UI(Image)用のゲージ。rotationの向きに、ratioの割合までtexを描く

cbuffer Ratio : register(b0)
{
    float ratio;
}

cbuffer Rotation : register(b1)
{
    float rotation;
}

cbuffer Alpha : register(b2)
{
    float alpha;
}

Texture2D tex : register(t0);
SamplerState smp : register(s0);

UiVSOutput VSMain(UiVertexInput input)
{
    return UiTransform(input);
}

float4 PSMain(UiVSOutput input) : SV_TARGET
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