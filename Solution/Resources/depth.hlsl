#define SHADOW_CASCADE_COUNT 3

cbuffer Transform : register (b0)
{
    float4x4 World;
}
cbuffer LightCount : register (b4)
{
    int light_count;
}

StructuredBuffer<float4x4> LightViewProj : register(t1);

StructuredBuffer<float4x4> BoneMatrices : register (t0);

struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 uv : TEXCOORD0;
    float2 uv2 : TEXCOORD1;
    float2 uv3 : TEXCOORD2;
    float2 uv4 : TEXCOORD3;
    float2 uv5 : TEXCOORD4;
    float2 uv6 : TEXCOORD5;
    float2 uv7 : TEXCOORD6;
    float2 uv8 : TEXCOORD7;
    uint bones_per_vertex : BONESPERVERTEX;
    uint4 bone_id : BONEINDEX;
    float4 bone_weight : BONEWEIGHT;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    uint instance_id : SV_InstanceID;
};

VSOutput vrt(VSInput input, uint instance_id : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    float4 localPos = float4(input.pos, 1.0f);
    float4 skinnedPos = 0;

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        float w = input.bone_weight[i];
        if (w > 0)
        {
            float4x4 boneMatrix = BoneMatrices[input.bone_id[i]];
            skinnedPos += mul(boneMatrix, localPos) * w;
        }
    }

    if (input.bones_per_vertex != 0)
        localPos = skinnedPos;

    float4 worldPos = mul(World, localPos);

    output.pos = mul(LightViewProj[instance_id], worldPos);
    output.instance_id = instance_id;

    return output;
}

struct GSOutput
{
    float4 pos : SV_POSITION;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(3)]
void geo(triangle VSOutput input[3], inout TriangleStream<GSOutput> tri_stream)
{
    for (uint j = 0; j < 3; ++j)
    {
        GSOutput element;
        element.pos = input[j].pos;
        element.RTIndex = input[j].instance_id;
        tri_stream.Append(element);
    }
    tri_stream.RestartStrip();
}