#include "pch.h"
#include "max_hp_card.h"

#include "gui.h"

void SlimeBoom::MaxHpCard::OnInspectorGui()
{
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Player", m_player_data_);
    m_card_data_.OnInspectorGui();
}

void SlimeBoom::MaxHpCard::OnSelect()
{
    auto hp = m_player_data_->GetHp();
    auto max_hp = m_player_data_->GetMaxHp();
    hp += m_value_;
    max_hp += m_value_;
    m_player_data_->SetHp(hp);
    m_player_data_->SetMaxHp(max_hp);
}

CEREAL_REGISTER_TYPE(SlimeBoom::MaxHpCard)