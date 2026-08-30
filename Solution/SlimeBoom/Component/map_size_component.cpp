#include "pch.h"
#include "map_size_component.h"
#include "gui.h"

namespace SlimeBoom
{
void MapSizeComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Min", m_min_);
    engine::Gui::PropertyField("Max", m_max_);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::MapSizeComponent)