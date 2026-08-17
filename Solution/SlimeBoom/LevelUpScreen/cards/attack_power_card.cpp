#include "pch.h"
#include "attack_power_card.h"

#include "gui.h"

namespace SlimeBoom
{
void AttackPowerCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

void AttackPowerCard::OnSelect()
{
    auto attack_power = m_player_data_->GetAttackPower();
    attack_power += m_value_;
    m_player_data_->SetAttackPower(attack_power);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AttackPowerCard)