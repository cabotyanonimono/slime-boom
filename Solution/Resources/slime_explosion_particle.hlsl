#include "engine.hlsli"
#include "slime_explosion_particle.hlsli"

cbuffer ExplosionParticleLook : register(b0)
{
    float4 explosion_particle_start_color;
    float4 explosion_particle_end_color;
    float explosion_particle_size;
    float explosion_particle_glow;
    float explosion_particle_size_random_min;
    float explosion_particle_size_random_max;
}

StructuredBuffer<SlimeExplosionParticle> slime_explosion_particles : register(t0);

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
};

float4 ColorOrDefault(float4 color, float4 default_color)
{
    return any(color) ? color : default_color;
}

VSOutput VSMain(VSInput input, uint instance_id : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    uint index = instance_id;
    SlimeExplosionParticle p = slime_explosion_particles[index];

    if (p.state != EXPLOSION_PARTICLE_STATE_ALIVE)
    {
        output.svpos = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return output;
    }

    float2 corner = input.pos.xy;
    float t = saturate(p.life_ratio);

    float size = (explosion_particle_size > 0.0f ? explosion_particle_size : 0.75f) * lerp(explosion_particle_size_random_min > 0.0f ? explosion_particle_size_random_min : 0.6f, explosion_particle_size_random_max > 0.0f ? explosion_particle_size_random_max : 1.4f, p.random) * (1.0f - t * t);

    float4 view_pos = mul(View, float4(p.position, 1.0f));
    view_pos.xy += corner * size;

    output.svpos = mul(Proj, view_pos);
    output.uv = corner;
    float4 start = ColorOrDefault(explosion_particle_start_color, float4(1.0f, 0.9f, 0.5f, 1.0f));
    float4 end = ColorOrDefault(explosion_particle_end_color, float4(0.9f, 0.2f, 0.05f, 1.0f));
    output.color = lerp(start, end, t);
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

    float3 rgb = input.color.rgb * (1.0f + core * (explosion_particle_glow > 0.0f ? explosion_particle_glow : 1.5f));
    return float4(rgb, input.color.a * falloff);
}
