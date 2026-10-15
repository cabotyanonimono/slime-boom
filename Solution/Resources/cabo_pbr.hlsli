#ifndef __CABO_PBR_HLSLI__
#define __CABO_PBR_HLSLI__

#include "util.hlsli"

float3 FresnelSchlick(float costheta, float3 f0)
{
    return f0 +(1.0f - f0) * pow(saturate(1.0f - costheta), 5.0f);
}

float DistributionGGX(float n_dot_h, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float n_dot_h2 = n_dot_h * n_dot_h;

    float nom = a2;
    float denom = (n_dot_h2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
    return nom / max(denom, 0.0001f);
}

float GeometrySchlickGGX(float n_dot_v, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;

    float nom = n_dot_v;
    float denom = n_dot_v * (1.0f - k) + k;
    
    return nom / denom;   
}

float GeometrySmith(float n_dot_v, float n_dot_l, float roughness)
{
    float ggx2 = GeometrySchlickGGX(n_dot_v, roughness);
    float ggx1 = GeometrySchlickGGX(n_dot_l, roughness);
    return ggx1 * ggx2;
}

float3 CalcLighting(float3 albedo, float metallic, float roughness, float3 normal, float3 light_dir, float3 view_dir)
{
    float3 n = normalize(normal);
    float3 v = normalize(view_dir);
    float3 l = normalize(light_dir);
    float3 h = normalize(v + l);

    float n_dot_l = saturate(dot(n, l));
    float n_dot_v = saturate(dot(n, v));
    float n_dot_h = saturate(dot(n, h));
    float h_dot_v = saturate(dot(h, v));
    
    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metallic);

    float3 f = FresnelSchlick(h_dot_v, f0);
    float d = DistributionGGX(n_dot_h, max(roughness, 0.0001f));
    float g = GeometrySmith(n_dot_v, n_dot_l, roughness);

    float3 direct_specular = (f * d * g) / (4.0f * n_dot_l * n_dot_v + 0.0001f);

    float3 kS = f;
    float3 kD = (float3(1.0f, 1.0f, 1.0f) - kS) * (1.0f - metallic);
    float3 direct_diffuse = kD * albedo / PI;

    return (direct_diffuse + direct_specular) * n_dot_l;
}
#endif
