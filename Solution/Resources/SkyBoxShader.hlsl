#include "engine.hlsli"
#include "Light.hlsli"

Texture2D TextureLeft : register(t3);
Texture2D TextureRight : register(t4);
Texture2D TextureTop : register(t5);
Texture2D TextureBottom : register(t6);
Texture2D TextureFront : register(t7);
Texture2D TextureBack : register(t8);

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
    float3 p = input.local_pos;
    float3 absP = abs(p);

    float2 uv = float2(0.0f, 0.0f);
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);

    // Y軸が最大の面（上下）
    if (absP.y >= absP.x && absP.y >= absP.z)
    {
        if (p.y > 0.0f) // Top面
        {
            uv = float2(p.x, p.z) * 0.5f + 0.5f;
            color = TextureTop.Sample(SmpClamp, uv);
        }
        else // Bottom面
        {
            uv = float2(p.x, -p.z) * 0.5f + 0.5f;
            color = TextureBottom.Sample(SmpClamp, uv);
        }
    }
    // X軸が最大の面（左右）
    else if (absP.x >= absP.y && absP.x >= absP.z)
    {
        if (p.x > 0.0f) // Right面
        {
            uv = float2(-p.z, -p.y) * 0.5f + 0.5f; // UVを 0.0 ~ 1.0 に変換
            color = TextureRight.Sample(SmpClamp, uv);
        }
        else // Left面
        {
            uv = float2(p.z, -p.y) * 0.5f + 0.5f;
            color = TextureLeft.Sample(SmpClamp, uv);
        }
    }
    // Z軸が最大の面（前後）
    else
    {
        if (p.z > 0.0f) // Front面
        {
            uv = float2(p.x, -p.y) * 0.5f + 0.5f;
            color = TextureFront.Sample(SmpClamp, uv);
        }
        else // Back面
        {
            uv = float2(-p.x, -p.y) * 0.5f + 0.5f;
            color = TextureBack.Sample(SmpClamp, uv);
        }
    }

    return color;
}