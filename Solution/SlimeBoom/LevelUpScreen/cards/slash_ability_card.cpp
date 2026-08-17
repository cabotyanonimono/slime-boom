#include "pch.h"
#include "slash_ability_card.h"

#include "../../Player/ability/slash_ability.h"

namespace SlimeBoom
{
void SlashAbilityCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Ability Processer", m_ability_processer_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
    engine::Gui::PropertyField("Player Data Presenter", m_player_data_presenter_);
    engine::Gui::PropertyField("Sword Controller", m_sword_controller_);
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
}

void SlashAbilityCard::OnSelect()
{
    const auto aura_ability = std::make_shared<SlashAbility>(m_compute_result_, m_player_data_, m_player_transform_, m_player_data_presenter_, m_sword_controller_);
    m_ability_processer_->Attach(aura_ability);
}

bool SlashAbilityCard::CanSelect()
{
    return !m_ability_processer_->HasAbility<SlashAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlashAbilityCard)