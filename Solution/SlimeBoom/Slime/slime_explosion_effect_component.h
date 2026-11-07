#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Rendering/CabotEngine/Graphics/ConstantBuffer.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
// 連鎖爆発のパーティクル1個分のGPU用データ。slime_explosion_particle.hlsliのSlimeExplosionParticleと合わせること。
struct SlimeExplosionParticleData
{
    float position[3];
    float age;
    float velocity[3];
    float random;
    uint32_t state;
    float floor_y;
    float life_ratio;
    uint32_t spawned_frame;
};

// パーティクルの動き。slime_explosion_particle_update.computeのcbuffer ExplosionParticleParamsと合わせること。
struct SlimeExplosionParticleParams
{
    float life_time;
    float speed_min;
    float speed_max;
    float gravity;
    float drag;
    float bounce;
    float up_min;
    float up_max;
    float bounce_friction;
    uint32_t particle_count;
    float padding[2];
};

// パーティクルの見た目。slime_explosion_particle.hlslのcbuffer ExplosionParticleLookと合わせること。
struct SlimeExplosionParticleLook
{
    Color start_color;
    Color end_color;
    float particle_size;
    float glow;
    float size_random_min;
    float size_random_max;
};

// 連鎖爆発のパーティクルを、GPU(ComputeShader)で生成・更新するComponent。
// 爆発の登録はSlimeExplosionComponentが行い、このComponentは爆発バッファを見てパーティクルを出す。
class SlimeExplosionEffectComponent : public engine::Component
{
    static constexpr uint32_t kParticlePerExplosion = 24;
    static constexpr uint32_t kMaxExplosionCount = 128;
    static constexpr uint32_t kMaxParticleCount = kParticlePerExplosion * kMaxExplosionCount;
    static constexpr uint32_t kThreadGroupSize = 64;

    std::shared_ptr<engine::StructuredBuffer> m_particle_buffer_;
    std::shared_ptr<engine::ConstantBuffer> m_params_buffer_;
    std::shared_ptr<engine::ConstantBuffer> m_look_buffer_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_update_compute_;

    // 動き
    float m_life_time_ = 0.5f;
    float m_speed_min_ = 6.0f;
    float m_speed_max_ = 14.0f;
    float m_gravity_ = 6.0f;
    float m_drag_ = 2.0f;
    float m_bounce_ = 0.35f;
    float m_up_min_ = 0.1f;
    float m_up_max_ = 0.6f;
    float m_bounce_friction_ = 0.7f;
    uint32_t m_particle_count_ = kParticlePerExplosion;

    // 見た目
    Color m_start_color_ = Color(1.0f, 0.9f, 0.5f, 1.0f);
    Color m_end_color_ = Color(0.9f, 0.2f, 0.05f, 1.0f);
    float m_particle_size_ = 0.75f;
    float m_glow_ = 1.5f;
    float m_size_random_min_ = 0.6f;
    float m_size_random_max_ = 1.4f;

public:
    void OnStart() override;
    void OnInspectorGui() override;
    /// パラメーターをGPUに転送し、パーティクル更新用のComputeShaderを実行する。
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_update_compute_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_life_time_),
                CEREAL_NVP(m_speed_min_),
                CEREAL_NVP(m_speed_max_),
                CEREAL_NVP(m_gravity_),
                CEREAL_NVP(m_drag_),
                CEREAL_NVP(m_bounce_),
                CEREAL_NVP(m_up_min_),
                CEREAL_NVP(m_up_max_),
                CEREAL_NVP(m_bounce_friction_),
                CEREAL_NVP(m_particle_count_),
                CEREAL_NVP(m_start_color_),
                CEREAL_NVP(m_end_color_),
                CEREAL_NVP(m_particle_size_),
                CEREAL_NVP(m_glow_),
                CEREAL_NVP(m_size_random_min_),
                CEREAL_NVP(m_size_random_max_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeExplosionEffectComponent, 2)
