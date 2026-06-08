#pragma once
#include "Components/component.h"
#include "Components/compute_shader_component.h"
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
};

class SlimeSimulationComponent : public engine::Component
{
    int m_num_slimes_ = 512;
    std::shared_ptr<engine::StructuredBuffer> m_slime_buffer_;
    std::shared_ptr<engine::ByteAddressBuffer> m_damage_buffer_;
    std::shared_ptr<engine::ConstantBuffer> m_num_slime_buffer_;
    
public:
    void OnStart() override;
    void OnInspectorGui() override;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeSimulationComponent, 1)
