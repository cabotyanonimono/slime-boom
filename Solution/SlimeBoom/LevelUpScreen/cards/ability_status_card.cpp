#include "pch.h"
#include "ability_status_card.h"

#include "gui.h"

namespace SlimeBoom
{
void AbilityStatusCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Ability Processer", m_ability_processer_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

void AbilityStatusCard::OnSelect()
{
    const auto status = GetStatusType();
    const auto ability_type = GetAbilityType();
    const auto value = m_player_data_->GetAbilityStatus(status, ability_type);
    m_player_data_->SetAbilityStatus(status, ability_type, value + m_value_);
}

std::string AbilityStatusCard::GetEffectText()
{
    const float value = m_player_data_->GetAbilityStatus(GetStatusType(), GetAbilityType());
    return FormatChange(value, value + m_value_, "x");
}

bool AbilityStatusCard::CanSelect()
{
    return HasTargetAbility();
}
}
