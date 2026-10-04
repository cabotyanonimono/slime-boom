#include "pch.h"
#include "level_text.h"

#include "gui.h"

namespace SlimeBoom
{
void LevelText::OnInspectorGui()
{
    engine::Gui::PropertyField("Level Text", m_level_text_);
    engine::Gui::PropertyField("UI Data", m_ui_data_provider_);
}

void LevelText::OnUpdate()
{
    auto player_data = m_ui_data_provider_->GetPlayerData();
    m_level_text_->string = "Lv." + std::format("{:02}", player_data.level);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::LevelText)