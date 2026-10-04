#include "pch.h"
#include "attack_num_card.h"

#include "gui.h"

void SlimeBoom::AttackNumCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Max Value", m_max_value_);
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

void SlimeBoom::AttackNumCard::OnSelect()
{
    auto attack_num = m_player_data_->GetAttackNum();
    attack_num += m_value_;
    m_player_data_->SetAttackNum(attack_num);
}

bool SlimeBoom::AttackNumCard::CanSelect()
{
    return m_player_data_->GetAttackNum() <= m_max_value_;
}

CEREAL_REGISTER_TYPE(SlimeBoom::AttackNumCard)
