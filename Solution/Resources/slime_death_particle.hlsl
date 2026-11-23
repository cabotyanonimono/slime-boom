// 旧エンジンの設定（加算、両面、深度は書かない）
// @Cull Off
// @ZWrite Off
// @Blend Additive
#define SLIME_TYPE_PARAMS_REGISTER t1
#include "engine.hlsli"
#include "slime.hlsli"
#include "slime_death_particle.hlsli"

// 種類ごとの色は種類の表(slime_type_params)の death_color を使う。
// 0のときは既定値(どちらも1)を使う。
cbuffer DeathParticleStyle : register(b0)
{
    float particle_size;
    float glow;
}

StructuredBuffer<SlimeDeathParticle> slime_death_particles : register(t0);

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
};

static const float2 kCorners[6] =
{
    float2(-1.0f, -1.0f), float2(-1.0f, 1.0f), float2(1.0f, 1.0f),
    float2(-1.0f, -1.0f), float2(1.0f, 1.0f), float2(1.0f, -1.0f)
};

VSOutput VSMain(uint vertex_id : SV_VertexID)
{
    VSOutput output = (VSOutput)0;

    uint index = vertex_id / 6;
    SlimeDeathParticle p = slime_death_particles[index];

    if (p.state != DEATH_PARTICLE_STATE_ALIVE)
    {
        output.svpos = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return output;
    }

    float2 corner = kCorners[vertex_id % 6];
    float t = saturate(p.life_ratio);

    SlimeTypeParams type_params = GetSlimeTypeParams(p.slime_type);
    float size = particle_size > 0.0f ? particle_size : 1.0f;
    size *= type_params.size_scale;
    size *= lerp(0.6f, 1.4f, p.random) * (1.0f - t * t);

    float4 view_pos = mul(View, float4(p.position, 1.0f));
    view_pos.xy += corner * size;

    output.svpos = mul(Proj, view_pos);
    output.uv = corner;
    output.color = type_params.death_color;
    output.color.a *= 1.0f - t;
    return output;
}

float4 PSMain(VSOutput input) : SV_Target
{
    float d = length(input.uv);
    if (d > 1.0f)
        discard;

    float falloff = saturate(1.0f - d);
    float core = falloff * falloff;
    float glow_power = glow > 0.0f ? glow : 1.0f;

    float3 rgb = input.color.rgb * (1.0f + core * glow_power);
    return float4(rgb, input.color.a * falloff);
}
