#include "engine.hlsli"
#include "slime.hlsli"
#include "slime_death_particle.hlsli"

cbuffer DeathParticleLook : register(b2)
{
    float4 normal_color;
    float4 speed_color;
    float4 tank_color;
    float particle_size;
    float glow;
}

StructuredBuffer<SlimeDeathParticle> slime_death_particles : register(t3);

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

float4 ColorOrDefault(float4 color, float4 default_color)
{
    return any(color) ? color : default_color;
}

float4 GetTypeColor(int slime_type)
{
    if (slime_type == SLIME_TYPE::SPEED)
        return ColorOrDefault(speed_color, float4(1.0f, 1.0f, 1.0f, 1.0f));
    if (slime_type == SLIME_TYPE::TANK)
        return ColorOrDefault(tank_color, float4(0.75f, 0.25f, 1.0f, 1.0f));
    return ColorOrDefault(normal_color, float4(0.35f, 0.85f, 1.0f, 1.0f));
}

VSOutput vrt(uint vertex_id : SV_VertexID)
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

    float size = particle_size > 0.0f ? particle_size : 0.35f;
    size *= p.slime_type == SLIME_TYPE::TANK ? 1.5f : 1.0f;
    size *= lerp(0.6f, 1.4f, p.random) * (1.0f - t * t);

    float4 view_pos = mul(View, float4(p.position, 1.0f));
    view_pos.xy += corner * size;

    output.svpos = mul(Proj, view_pos);
    output.uv = corner;
    output.color = GetTypeColor(p.slime_type);
    output.color.a *= 1.0f - t;
    return output;
}

float4 pix(VSOutput input) : SV_Target
{
    float d = length(input.uv);
    if (d > 1.0f)
        discard;

    float falloff = saturate(1.0f - d);
    float core = falloff * falloff;
    float glow_power = glow > 0.0f ? glow : 1.5f;

    float3 rgb = input.color.rgb * (1.0f + core * glow_power);
    return float4(rgb, input.color.a * falloff);
}
