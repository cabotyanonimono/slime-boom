#include "pch.h"
#include "ability_data.h"

#include "gui.h"

void SlimeBoom::AbilityData::OnInspectorGui()
{
    if (ImGui::CollapsingHeader("Slash Data"))
    {
        engine::Gui::PropertyField("Arc", slash_ability_data.arc);
        engine::Gui::PropertyField("Size", slash_ability_data.size);
        engine::Gui::PropertyField("Damage", slash_ability_data.damage_multiplier);
        engine::Gui::PropertyField("Power", slash_ability_data.power_multiplier);
        engine::Gui::PropertyField("Cooldown", slash_ability_data.cooldown);
    }
}

CEREAL_REGISTER_TYPE(SlimeBoom::AbilityData)