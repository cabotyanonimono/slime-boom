#pragma once
#include "event.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Components/transform.h"

namespace SlimeBoom
{
class SlimeDamageCounter : public engine::Component
{
    bool m_is_first_frame = true;
    engine::AssetPtr<engine::ComputeShaderComponent> m_compute_shader_;
    std::shared_ptr<engine::BufferBase> m_damage_buffer_;
    size_t m_lisner_token_ = -1;
    uint32_t m_damage_counter_ = 0;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this),
            CEREAL_NVP(m_compute_shader_));
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeDamageCounter, 1)