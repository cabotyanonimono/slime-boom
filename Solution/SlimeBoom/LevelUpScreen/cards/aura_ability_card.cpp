#include "pch.h"
#include "aura_ability_card.h"

#include "gui.h"
#include "../../Player/ability/aura_ability.h"

namespace SlimeBoom
{
void AuraAbilityCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Ability Processer", m_ability_processer_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
    engine::Gui::PropertyField("Player Data Presenter", m_player_data_presenter_);
    engine::Gui::PropertyField("Aura Renderer", m_aura_renderer_);
}

void AuraAbilityCard::OnSelect()
{
    const auto aura_ability = std::make_shared<AuraAbility>(m_player_data_, m_aura_renderer_, m_player_data_presenter_);
    m_ability_processer_->Attach(aura_ability);
}

bool AuraAbilityCard::CanSelect()
{
    return !m_ability_processer_->HasAbility<AuraAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AuraAbilityCard)