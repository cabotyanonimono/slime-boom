#include "engine.hlsli"
#include "Light.hlsli"

Texture2D _MainTex : register (t4);

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

    float4 skinnedPos = 0;
    float3 skinnedNormal = 0;

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        float w = input.bone_weight[i];
        if (w > 0)
        {
            float4x4 boneMatrix = BoneMatrices[input.bone_id[i]];
            skinnedPos += mul(boneMatrix, localPos) * w;
            skinnedNormal += mul((float3x3)boneMatrix, localNormal) * w;
        }
    }

    if (input.bones_per_vertex != 0)
    {
        localPos = skinnedPos;
        localNormal = skinnedNormal;
    }

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
    float3 brightness = float3(0, 0, 0);
    if (light_count == 0)
    {
        float2 flippedUV = float2(input.uv.x, 1.0 - input.uv.y);
        float4 mainColor = _MainTex.Sample(smp, flippedUV);
        return float4(mainColor.rgb, mainColor.a);
    }

    float4 viewPos = mul(View, float4(input.worldpos, 1.0));
    float depth = abs(viewPos.z);
    brightness = CalcAllShadow(depth, normalize(input.normal), input.worldpos);

    float4 main_color = _MainTex.Sample(smp, input.uv);

    return float4(main_color.rgb * brightness, main_color.a);
}
