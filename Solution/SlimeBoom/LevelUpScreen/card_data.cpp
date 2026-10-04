#include "pch.h"
#include "card_data.h"
#include "gui.h"

void SlimeBoom::CardData::OnInspectorGui()
{
    engine::Gui::PropertyField("Icon", icon);
    engine::Gui::PropertyField("Name", name);
    engine::Gui::PropertyField("Description", description);
    engine::Gui::PropertyField("Background", background);
}

CEREAL_REGISTER_TYPE(SlimeBoom::CardData)