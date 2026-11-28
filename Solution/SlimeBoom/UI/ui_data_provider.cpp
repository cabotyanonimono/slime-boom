#include "pch.h"
#include "ui_data_provider.h"

#include "gui.h"

namespace SlimeBoom
{
void UiDataProvider::OnInspectorGui()
{
    engine::Gui::PropertyField("Player Data", m_player_data_);
    engine::Gui::PropertyField("Level Up Component", m_level_up_controller_);
    engine::Gui::PropertyField("Card Controller", m_card_controller_);
    engine::Gui::PropertyField("Player", m_player_);   
    engine::Gui::PropertyField("Exp Simulation", m_exp_simulation_);
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
    return 1.0f - ((m_player_->GetAvoidTimer()) / m_player_->GetAvoidCooldown());
}

std::shared_ptr<CardPool> UiDataProvider::GetCardPool() const
{
    return m_card_controller_->GetPool();
}

std::shared_ptr<AbilityProcesser> UiDataProvider::GetAbilityProcesser() const
{
    return m_card_controller_->GetAbilityProcesser();
}

float UiDataProvider::GetAcquisitionRange() const
{
    const auto exp_simulation = m_exp_simulation_.CastedLock();
    return exp_simulation ? exp_simulation->GetAcquisitionRange() : 0.0f;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::UiDataProvider)