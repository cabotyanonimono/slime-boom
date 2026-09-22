#include "pch.h"
#include "gui.h"
#include "player_status_ui.h"

namespace SlimeBoom
{
void PlayerStatusUi::OnInspectorGui()
{
    engine::Gui::PropertyField("UI Data Provider", m_ui_data_provider_);
    engine::Gui::PropertyField("Max Hp Text", m_max_hp_text_);
    engine::Gui::PropertyField("Speed Text", m_speed_text_);
    engine::Gui::PropertyField("Attack Power Text", m_attack_power_text_);
    engine::Gui::PropertyField("Attack Speed Text", m_attack_speed_text_);
    engine::Gui::PropertyField("Avoid Time Text", m_avoid_time_text_);
    engine::Gui::PropertyField("Avoid Speed Text", m_avoid_speed_text_);
    engine::Gui::PropertyField("Exp Multiplier", m_exp_multiplier_text_);
}

void PlayerStatusUi::OnUpdate()
{
    auto player_data = m_ui_data_provider_->GetPlayerData();
    m_max_hp_text_->string = player_data.max_hp;
    m_speed_text_->string = player_data.speed;
    m_attack_power_text_->string = player_data.attack_power;
    m_attack_speed_text_->string = player_data.attack_speed;
    m_avoid_time_text_->string = player_data.avoid_time;
    m_avoid_speed_text_->string = player_data.avoid_speed;
    m_exp_multiplier_text_->string = player_data.exp_multiplier;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerStatusUi)