#include "pch.h"
#include "exp_gauge.h"

#include "gui.h"

void SlimeBoom::ExpGauge::OnInspectorGui()
{
    engine::Gui::PropertyField("Ui Data Provider", m_ui_data_provider_);
    engine::Gui::PropertyField("Gauge", m_gauge_);
}

void SlimeBoom::ExpGauge::OnUpdate()
{
    const auto current_exp = m_ui_data_provider_->GetCurrentLevelExp();
    const auto required_exp = m_ui_data_provider_->GetRequiredExp();

    const auto exp_ratio = static_cast<float>(current_exp) / static_cast<float>(required_exp);
    m_gauge_->SetRatio(exp_ratio);
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExpGauge)