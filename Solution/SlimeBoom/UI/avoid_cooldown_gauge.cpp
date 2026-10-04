#include "pch.h"
#include "avoid_cooldown_gauge.h"

#include "engine.h"
#include "gui.h"

engine::Task SlimeBoom::AvoidCooldownGauge::GaugeTransparentTask()
{
    //ratioが１以上になってたらゲージを少しずつ透明にしていく
    auto transparent_per_second = 1.0f / m_gauge_transparent_time_;
    while (m_current_alpha_ >= 0.0f)
    {
        m_gauge_->SetAlpha(m_current_alpha_);
        m_current_alpha_ -= transparent_per_second * engine::Time::GetDeltaTime();
        co_await engine::WaitForNextFrame();
    }
}

void SlimeBoom::AvoidCooldownGauge::OnInspectorGui()
{
    engine::Gui::PropertyField("Gauge", m_gauge_);
    engine::Gui::PropertyField("UI Provider", m_ui_data_provider_);
    engine::Gui::PropertyField("Target Transform", m_target_transform_);
    engine::Gui::PropertyField("Offset", m_offset_);
    engine::Gui::PropertyField("Bar RectTransform", m_bar_rect_transform_);
    engine::Gui::PropertyField("Bar Frame RectTransform", m_bar_frame_rect_transform_);
    engine::Gui::PropertyField("Gauge Transparent Time", m_gauge_transparent_time_);   
}

void SlimeBoom::AvoidCooldownGauge::OnUpdate()
{
    //ターゲットの３D座標にUIの位置を変更する
    const auto screen_target_pos = engine::CameraComponent::Main()->WorldPosToScreenPos(m_target_transform_->Position()) * 0.5f;
    const auto offset_pos = screen_target_pos + m_offset_;

    m_bar_rect_transform_->anchored_position = offset_pos;
    m_bar_frame_rect_transform_->anchored_position = offset_pos;

    //ratioをゲージに反映する
    const auto ratio = m_ui_data_provider_->GetAvoidCooldownRatio();
    m_gauge_->SetRatio(ratio);

    if (ratio < 1.0f && m_is_transparent_)
    {
        m_is_transparent_ = false;
        m_current_alpha_ = 1.0f;
        m_gauge_->SetAlpha(m_current_alpha_);
    }
    if (!m_is_transparent_ && ratio >= 1.0f)
    {
        m_is_transparent_ = true;
        engine::Engine::coroutine.Start(GaugeTransparentTask(), m_gauge_transparent_task_token_.GetToken());
    }
}

void SlimeBoom::AvoidCooldownGauge::OnDestroy()
{
    m_gauge_transparent_task_token_.Cancel();
}

CEREAL_REGISTER_TYPE(SlimeBoom::AvoidCooldownGauge)
