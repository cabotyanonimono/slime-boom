#include "engine.hlsli"
#include "slime_explosion_particle.hlsli"

cbuffer ExplosionParticleLook : register(b2)
{
    float4 start_color; // 生成直後の色
    float4 end_color;   // 消える直前の色
    float particle_size;
    float glow;
    float size_random_min; // 大きさのばらつき(particle_sizeに掛ける倍率)
    float size_random_max;
}

StructuredBuffer<SlimeExplosionParticle> slime_explosion_particles : register(t3);

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

VSOutput vrt(uint vertex_id : SV_VertexID)
{
    VSOutput output = (VSOutput)0;

    uint index = vertex_id / 6;
    SlimeExplosionParticle p = slime_explosion_particles[index];

    if (p.state != EXPLOSION_PARTICLE_STATE_ALIVE)
    {
        output.svpos = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return output;
    }

    float2 corner = kCorners[vertex_id % 6];
    float t = saturate(p.life_ratio);

    float size = (particle_size > 0.0f ? particle_size : 0.75f) * lerp(size_random_min > 0.0f ? size_random_min : 0.6f, size_random_max > 0.0f ? size_random_max : 1.4f, p.random) * (1.0f - t * t);

    float4 view_pos = mul(View, float4(p.position, 1.0f));
    view_pos.xy += corner * size;

    output.svpos = mul(Proj, view_pos);
    output.uv = corner;
    float4 start = ColorOrDefault(start_color, float4(1.0f, 0.9f, 0.5f, 1.0f));
    float4 end = ColorOrDefault(end_color, float4(0.9f, 0.2f, 0.05f, 1.0f));
    output.color = lerp(start, end, t);
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

    float3 rgb = input.color.rgb * (1.0f + core * (glow > 0.0f ? glow : 1.5f));
    return float4(rgb, input.color.a * falloff);
}
