#include "engine.hlsli"
#include "Light.hlsli"

TextureCube texture_cube : register(t3);

cbuffer Params : register (b2)
{
    float intensity;
}

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 local_pos : TEXCOORD1;
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

    float4x4 no_trans_view = View;
    no_trans_view._m03_m13_m23 = float3(0.0f, 0.0f, 0.0f);
    
    float4 projPos = mul(Proj, mul(no_trans_view, localPos));

    float3 worldNormal = mul((float3x3)World, localNormal);

    output.svpos = projPos;
    output.normal = normalize(worldNormal);
    output.color = input.color;
    output.uv = input.uv;
    output.local_pos = input.pos;
    return output;
}

float4 pix(VSOutput input) : SV_Target
{
    float3 dir = normalize(input.local_pos);
    
    float3 sky_color = texture_cube.Sample(smp, dir * float3(1.0f, -1.0f, 1.0f)).rgb * intensity;

    return float4(sky_color, 1.0f);
}