#include "pch.h"
#include "ui_data_provider.h"

namespace SlimeBoom
{
void UiDataProvider::OnInspectorGui()
{
    engine::Gui::PropertyField("Player Component", m_player_component_);
    engine::Gui::PropertyField("Level Up Component", m_level_up_controller_);
}

const PlayerData& UiDataProvider::GetPlayerData() const
{
    return m_player_component_->GetPlayerData();
}

int UiDataProvider::GetCurrentLevelExp() const
{
    return m_level_up_controller_->GetCurrentLevelExp();
}

int UiDataProvider::GetRequiredExp() const
{
    return m_level_up_controller_->GetRequiredExp();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::UiDataProvider)