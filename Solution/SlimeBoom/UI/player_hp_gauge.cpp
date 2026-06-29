#include "pch.h"
#include "player_hp_gauge.h"

namespace SlimeBoom
{
void PlayerHpGauge::OnInspectorGui()
{
    engine::Gui::PropertyField("Gauge", m_gauge_);
    engine::Gui::PropertyField("UI Data Provider", m_ui_data_provider_);
}

void PlayerHpGauge::OnUpdate()
{
    const auto player_data = m_ui_data_provider_->GetPlayerData();
    const auto hp_ratio = player_data.hp / player_data.max_hp;
    
    m_gauge_->SetRatio(hp_ratio);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerHpGauge)