#include "pch.h"
#include "exp_sound_emitter.h"

#include "gui.h"
#include "../Sound/sound_manager_component.h"

namespace SlimeBoom
{
void ExpSoundEmitter::OnInspectorGui()
{
    engine::Gui::PropertyField("Exp Simulation Component", m_exp_simulation_component_);
    engine::Gui::PropertyField("Sound Manager Component", m_sound_manager_component_);
    engine::Gui::PropertyField("Emmit Delay Time", m_emmit_delay_time_);
    engine::Gui::PropertyField("Sound Queue Many Threshold", m_sound_queue_many_threshold_);
    engine::Gui::PropertyField("Max Sound Queue Count", m_max_sound_queue_count_);
}

void ExpSoundEmitter::OnUpdate()
{
    m_sound_queue_count_ += m_exp_simulation_component_->GetDeltaExpCount();
    m_sound_queue_count_ = std::min(m_sound_queue_count_, m_max_sound_queue_count_);
    m_emmit_timer_ += engine::Time::GetDeltaTime();

    if (m_emmit_timer_ >= m_emmit_delay_time_ && m_sound_queue_count_ > 0)
    {
        m_emmit_timer_ = 0;
        if (m_sound_queue_count_ > m_sound_queue_many_threshold_)
        {
            m_sound_queue_count_ -= m_sound_queue_many_threshold_;
            m_sound_manager_component_->Play(kSoundTypes::kGetExpMany);
        }
        else
        {
            --m_sound_queue_count_;
            m_sound_manager_component_->Play(kSoundTypes::kGetExp);
        }
    }
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExpSoundEmitter)
