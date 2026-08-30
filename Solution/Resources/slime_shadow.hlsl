#include "engine.hlsli"
#include "slime.hlsli"

StructuredBuffer<SlimeData> slimes : register(t3);

struct VSOutput
{
    float3 world_pos : WORLDPOS;
};

VSOutput vrt(VSInput input, uint instance_id : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    float4 world_pos = mul(World, input.pos);
    world_pos.xyz += slimes[instance_id].position;

    output.world_pos = world_pos.xyz;

    return output;
}

struct GSOutput
{
    float4 pos : SV_POSITION;
    float2 near_far : NEARFAR;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

float LinearizeDepth(float depth, float nearZ, float farZ)
{
    return (nearZ * farZ) / (farZ - depth * (farZ - nearZ));
}

float pix(GSOutput input) : SV_TARGET
{
    float linearDepth = LinearizeDepth(input.pos.w, input.near_far.x, input.near_far.y);
    
    return linearDepth / input.near_far.y;
}

#define MAX_SHADOWMAP_COUNT 10

[maxvertexcount(3 * MAX_SHADOWMAP_COUNT)]
void geo(triangle VSOutput input[3], inout TriangleStream<GSOutput> tri_stream)
{
    for (int i = 0; i < MAX_SHADOWMAP_COUNT; ++i)
    {
        float4x4 viewproj = LightViewProj[i];
        for (uint j = 0; j < 3; j++)
        {
            GSOutput element;
            element.pos = mul(viewproj, float4(input[j].world_pos, 1));
            element.near_far = float2(0.1f, 10.0f);
            element.RTIndex = i;
            tri_stream.Append(element);
        }
        tri_stream.RestartStrip();
    }
}