#include "pch.h"
#include "survival_timer.h"

#include "engine_time.h"

namespace SlimeBoom
{
void SurvivalTimer::OnInspectorGui()
{
    engine::Gui::PropertyField("Text Renderer", m_text_renderer_);
}

void SurvivalTimer::OnUpdate()
{
    m_seconds_ += engine::Time::GetDeltaTime();
    
    if (m_seconds_ >= kSecondsPerMinute)
    {
        m_seconds_ -= kSecondsPerMinute;
        ++m_minutes_;
    }
    
    if (m_text_renderer_ == nullptr)
        return;

    m_text_renderer_->string = std::format("{:02d}:{:02d}", static_cast<int>(m_minutes_), static_cast<int>(m_seconds_));
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SurvivalTimer)