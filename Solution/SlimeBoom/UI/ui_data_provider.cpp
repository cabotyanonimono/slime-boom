#include "pch.h"
#include "ui_data_provider.h"

namespace SlimeBoom
{
void UiDataProvider::OnInspectorGui()
{
    engine::Gui::PropertyField("Player Data", m_player_data_);
    engine::Gui::PropertyField("Level Up Component", m_level_up_controller_);
    engine::Gui::PropertyField("Card Controller", m_card_controller_);
    engine::Gui::PropertyField("Player", m_player_);   
}

const PlayerData& UiDataProvider::GetPlayerData() const
{
    return m_player_data_->GetPlayerData();
}

int UiDataProvider::GetCurrentLevelExp() const
{
    return m_level_up_controller_->GetCurrentLevelExp();
}

int UiDataProvider::GetRequiredExp() const
{
    return m_level_up_controller_->GetRequiredExp();
}

std::array<std::shared_ptr<CardBase>, CardController::kCardCount> UiDataProvider::GetCards() const
{
    return m_card_controller_->GetCards();
}

float UiDataProvider::GetAvoidCooldownRatio() const
{
    return (1.0f - m_player_->GetAvoidTimer()) / m_player_->GetAvoidCooldown();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::UiDataProvider)