#include "pch.h"
#include "ui_hover_scale_effect.h"

#include "engine_time.h"
#include "gui.h"

void UiHoverScaleEffect::OnInspectorGui()
{
    ImGui::Checkbox("Unscaled", &m_use_unscaled_delta_time_);
    engine::Gui::PropertyField("UI Button", m_ui_button_);
    engine::Gui::PropertyField("Image", m_rect_transform_);
    engine::Gui::PropertyField("Target Scale", m_target_scale_);
    engine::Gui::PropertyField("Transition Time", m_transition_time_);
}

void UiHoverScaleEffect::OnStart()
{
    m_base_size_ = m_rect_transform_->size_delta;
}

void UiHoverScaleEffect::OnUpdate()
{
    const auto delta_time = m_use_unscaled_delta_time_
        ? engine::Time::Get()->UnscaledDeltaTime()
        : engine::Time::GetDeltaTime();
    
    m_time_ += m_ui_button_->IsSelected()
        ? delta_time
        : -delta_time;
    
    m_time_ = std::clamp(m_time_, 0.0f, m_transition_time_);

    m_rate_ = m_time_ / m_transition_time_;
    const auto current_scale = engine::Mathf::Lerp(1.0f, m_target_scale_, m_rate_);
    m_rect_transform_->size_delta = m_base_size_ * current_scale;
}

CEREAL_REGISTER_TYPE(UiHoverScaleEffect)
