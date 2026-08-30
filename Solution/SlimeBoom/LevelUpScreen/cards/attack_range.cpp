#include "pch.h"
#include "attack_range_card.h"
#include "gui.h"

namespace SlimeBoom
{
void AttackRangeCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Player Data", m_player_data_);   
}

void AttackRangeCard::OnSelect()
{
    auto attack_range= m_player_data_->GetAttackRange();
    attack_range += m_value_;
    m_player_data_->SetAttackRange(attack_range);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AttackRangeCard)