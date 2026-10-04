#include "engine.hlsli"

cbuffer Color : register(b2)
{
    float4 main_color;    
  	float  effect_progress;
}

Texture2D _MainTex : register (t3);

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 worldpos : TEXCOORD1;
};

float3x3 ExtractRotation(float4x4 m)
{
    return float3x3(
        m[0].xyz,
        m[1].xyz,
        m[2].xyz
    );
}

static const float4x4 identityMatrix = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

VSOutput vrt(VSInput input)
{
    VSOutput output = (VSOutput)0;

    float4 localPos = float4(input.pos, 1.0f);
    float3 localNormal = input.normal;

    float4 worldPos = mul(World, localPos);
    float4 projPos = mul(Proj, mul(View, worldPos));

    float3 worldNormal = mul((float3x3)World, localNormal);

    output.svpos = projPos;
    output.normal = normalize(worldNormal);
    
    output.color = input.color; 
    output.uv = input.uv;
    output.worldpos = worldPos.xyz;
    return output;
}

float4 pix(VSOutput input) : SV_TARGET
{
    float2 scrolledUV = input.uv;
    scrolledUV.x -= effect_progress * 1.5f - 1.0f; 

    float4 texColor = _MainTex.Sample(SmpClamp, scrolledUV);
    float alpha = texColor.r;

    alpha = saturate(alpha - effect_progress);

    alpha *= input.color.a;

    if (alpha <= 0.0f) discard;

    return float4(main_color.rgb, alpha);
}