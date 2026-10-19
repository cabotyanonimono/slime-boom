#include "engine.hlsli"
#include "Light.hlsli"

Texture2D _MainTex : register (t3);

// モザイク1マスの大きさ(ピクセル)
static const float kMosaicSize = 24.0f;
// 背景の明るさ(1で元の明るさ)
static const float kBrightness = 0.4f;

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
    float width, height;
    _MainTex.GetDimensions(width, height);
    const float2 block = kMosaicSize / float2(width, height);
    const float2 uv = (floor(input.uv / block) + 0.5f) * block;
    float4 color = _MainTex.Sample(smp, uv);
    color.rgb *= kBrightness;
    return color;
}