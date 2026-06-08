#include "pch.h"
#include "player_component.h"

#include "gui.h"

namespace SlimeBoom
{
void PlayerComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Speed", speed);
    engine::Gui::PropertyField("Health", health);
    engine::Gui::PropertyField("Attack Speed", attack_speed);
    engine::Gui::PropertyField("Attack Damage", attack_damage);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerComponent)