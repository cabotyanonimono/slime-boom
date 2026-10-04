#pragma once
#include "slime_take_damage_event_component.h"
#include "slime_types.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Components/mesh_renderer.h"
#include "Rendering/CabotEngine/Graphics/ByteAddressBuffer.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
struct SlimeData
{
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Vector3 velocity;
    Vector3 angular_velocity;
    uint32_t hp;
    float damage_color_timer;
};

class SlimeSimulationComponent : public engine::Component
{
    static constexpr int kMaxSlimesCount = 8096;
    int m_current_slime_count_ = 512;
    std::shared_ptr<engine::StructuredBuffer> m_slime_buffer_;
    std::shared_ptr<engine::ConstantBuffer> m_slime_count_buffer_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_slime_generator_compute_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_slime_physics_compute_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_closest_slime_compute_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_player_attack_compute_;
    engine::AssetPtr<engine::MeshRenderer> m_slime_renderer_;
    engine::AssetPtr<SlimeTakeDamageEventComponent> m_slime_take_damage_event_component_;

public:
    void OnStart() override;
    void OnInspectorGui() override;
    void OnUpdate() override;

    int GetCurrentSlimesCount() const;
    void SetCurrentSlimesCount(int count);
    void SetSpawnRate(std::array<float, SlimeTypes::Count> rates) const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_slime_generator_compute_),
                CEREAL_NVP(m_slime_physics_compute_),
                CEREAL_NVP(m_closest_slime_compute_),
                CEREAL_NVP(m_player_attack_compute_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_slime_renderer_)
            );
        }

        if (version >= 5)
        {
            ar(
                CEREAL_NVP(m_slime_take_damage_event_component_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeSimulationComponent, 5)