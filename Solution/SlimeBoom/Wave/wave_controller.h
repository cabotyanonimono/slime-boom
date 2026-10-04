#pragma once
#include "wave_data.h"
#include "../Slime/slime_simulation_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
class WaveController : public engine::Component
{
    uint32_t m_current_wave_index_ = 0;
    float m_current_time_ = 0.0f;
    std::vector<WaveData> m_wave_data_;
    engine::AssetPtr<SlimeSimulationComponent> m_slime_simulation_component_;
    
public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;

    [[nodiscard]] uint32_t GetCurrentNumSlimes() const;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_wave_data_),
            CEREAL_NVP(m_slime_simulation_component_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::WaveController, 1)