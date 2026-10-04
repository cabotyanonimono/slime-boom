#include "pch.h"
#include "exp_multiplier_card.h"

#include "gui.h"

namespace SlimeBoom
{
void ExpMultiplierCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

void ExpMultiplierCard::OnSelect()
{
    auto exp_multiplier = m_player_data_->GetExpMultiplier();
    exp_multiplier += m_value_;
    m_player_data_->SetExpMultiplier(exp_multiplier);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExpMultiplierCard)