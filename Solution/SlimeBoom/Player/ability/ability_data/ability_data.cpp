#include "pch.h"
#include "ability_data.h"

#include "gui.h"

void SlimeBoom::AbilityData::OnInspectorGui()
{
    if (ImGui::CollapsingHeader("Slash Data"))
    {
        engine::Gui::PropertyField("Arc", m_slash_ability_data_.arc);
        engine::Gui::PropertyField("Size", m_slash_ability_data_.size);
        engine::Gui::PropertyField("Damage", m_slash_ability_data_.damage_multiplier);
        engine::Gui::PropertyField("Power", m_slash_ability_data_.power_multiplier);
        engine::Gui::PropertyField("Cooldown", m_slash_ability_data_.cooldown);
    }
}

SlimeBoom::SlashAbilityData SlimeBoom::AbilityData::GetSlashAbilityData()
{
    return m_slash_ability_data_;
}

void SlimeBoom::AbilityData::SetSlashAbilityData(const SlashAbilityData& ability_data)
{
    m_slash_ability_data_ = ability_data;
}

CEREAL_REGISTER_TYPE(SlimeBoom::AbilityData)
