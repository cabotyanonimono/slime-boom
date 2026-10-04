#include "pch.h"
#include "avoid_percent_card.h"

#include "gui.h"

namespace SlimeBoom
{
void AvoidPercentCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

void AvoidPercentCard::OnSelect()
{
    auto avoid_percent = m_player_data_->GetAvoidPercent();
    avoid_percent += m_value_;
    m_player_data_->SetAvoidPercent(avoid_percent);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AvoidPercentCard)
