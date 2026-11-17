#include "pch.h"
#include "attack_speed_card.h"

#include "gui.h"

namespace SlimeBoom
{
void AttackSpeedCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

std::string AttackSpeedCard::GetEffectText()
{
    const float value = m_player_data_->GetAttackSpeed();
    return FormatChange(value, value + m_value_, "x");
}

void AttackSpeedCard::OnSelect()
{
    auto attack_speed = m_player_data_->GetAttackSpeed();
    attack_speed += m_value_;
    m_player_data_->SetAttackSpeed(attack_speed);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AttackSpeedCard)