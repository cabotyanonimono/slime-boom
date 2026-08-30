#include "pch.h"
#include "avoid_cooldown_gauge.h"

void SlimeBoom::AvoidCooldownGauge::OnInspectorGui()
{
    engine::Gui::PropertyField("Gauge", m_gauge_);
    engine::Gui::PropertyField("UI Provider", m_ui_data_provider_);
}

void SlimeBoom::AvoidCooldownGauge::OnUpdate()
{
    m_gauge_->SetRatio(m_ui_data_provider_->GetAvoidCooldownRatio());
}

CEREAL_REGISTER_TYPE(SlimeBoom::AvoidCooldownGauge)