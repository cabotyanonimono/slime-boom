#include "pch.h"
#include "speed_card.h"

#include "gui.h"

void SlimeBoom::SpeedCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

void SlimeBoom::SpeedCard::OnSelect()
{
    auto speed = m_player_data_->GetSpeed();
    speed += m_value_;
    m_player_data_->SetSpeed(speed);
}

CEREAL_REGISTER_TYPE(SlimeBoom::SpeedCard)