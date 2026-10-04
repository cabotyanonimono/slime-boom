#include "pch.h"
#include "ability_data.h"

#include "gui.h"

namespace SlimeBoom
{
void AbilityData::OnInspectorGui()
{
    engine::Gui::PropertyField("Value", value);
    engine::Gui::PropertyField("Count", count);
    engine::Gui::PropertyField("Arc", arc);
    engine::Gui::PropertyField("Size", size);
    engine::Gui::PropertyField("Duration", duration);
    engine::Gui::PropertyField("Attack Speed", cooldown);
    engine::Gui::PropertyField("Knockback", knockback);
}
}
