#pragma once
#include "exp_simulation_component.h"
#include "../Sound/sound_manager_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Coroutine/task.h"

namespace SlimeBoom
{
// Exp関連のサウンドを鳴らすComponentです。
class ExpSoundEmitter : public engine::Component
{
    float m_emmit_timer_ = 0.0f;
    float m_emmit_delay_time_ = 0.0f;
    uint32_t m_max_sound_queue_count_ = 0;
    uint32_t m_sound_queue_many_threshold_ = 0;
    uint32_t m_sound_queue_count_ = 0;
    engine::AssetPtr<ExpSimulationComponent> m_exp_simulation_component_;
    engine::AssetPtr<SoundManagerComponent> m_sound_manager_component_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_exp_simulation_component_),
            CEREAL_NVP(m_sound_manager_component_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_emmit_delay_time_),
                CEREAL_NVP(m_max_sound_queue_count_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_sound_queue_many_threshold_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ExpSoundEmitter, 3)
