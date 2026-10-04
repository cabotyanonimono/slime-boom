#include "pch.h"
#include "player_hp_gauge.h"

#include "gui.h"

namespace SlimeBoom
{
void PlayerHpGauge::OnInspectorGui()
{
    engine::Gui::PropertyField("Gauge", m_gauge_);
    engine::Gui::PropertyField("Hp Text", m_hp_text_);
    engine::Gui::PropertyField("UI Data Provider", m_ui_data_provider_);
}

void PlayerHpGauge::OnUpdate()
{
    const auto player_data = m_ui_data_provider_->GetPlayerData();

    m_hp_text_->string = std::format("{:.0f}", player_data.hp) + " / " + std::format("{}", player_data.max_hp);
    
    const auto hp_ratio = player_data.hp / player_data.max_hp;
    m_gauge_->SetRatio(hp_ratio);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerHpGauge)