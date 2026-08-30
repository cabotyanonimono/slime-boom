#ifndef __ENGINE_HLSLI__
#define __ENGINE_HLSLI__

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

cbuffer WorldMatrix : register (b0)
{
    float4x4 World;
}
cbuffer ViewProjMatrix : register (b1)
{
    float4x4 View;
    float4x4 Proj;
}

cbuffer SceneData : register(b2)
{
    float2 screen_size;
    float2 shadow_map_size;
    float time;
    float deltatime;
    float3 camera_pos;
    float3 camera_dir;
}

StructuredBuffer<float4x4> BoneMatrices : register (t0);
StructuredBuffer<float4x4> LightViewProj : register(t1);
Texture2DArray ShadowMaps : register (t2);
SamplerState smp : register (s0);
SamplerComparisonState shadowSampler : register (s1);
SamplerState SmpClamp : register (s2);

#endif