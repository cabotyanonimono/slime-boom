#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
// スライムが死亡したときのパーティクル1個分のGPU用データ。
struct SlimeDeathParticleData
{
    float position[3];
    float age;
    float velocity[3];
    float random;
    int32_t slime_type;
    uint32_t state;
    float floor_y;
    float life_ratio;
};

// スライム死亡時のパーティクルを、GPU(ComputeShader)で更新するComponent。
class SlimeDeathEffectComponent : public engine::Component
{
    static constexpr uint32_t kMaxParticleCount = 4096;
    static constexpr uint32_t kThreadGroupSize = 64;

    std::shared_ptr<engine::StructuredBuffer> m_particle_buffer_;
    std::shared_ptr<engine::StructuredBuffer> m_particle_counter_buffer_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_update_compute_;

public:
    void OnStart() override;
    void OnInspectorGui() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_update_compute_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeDeathEffectComponent, 1)
