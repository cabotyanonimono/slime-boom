#include "pch.h"
#include "explosion_ability_card.h"

#include "gui.h"
#include "../../Player/ability/explosion_ability.h"

namespace SlimeBoom
{
void ExplosionAbilityCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Ability Processer", m_ability_processer_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
    engine::Gui::PropertyField("Explosion Component", m_explosion_component_);
}

void ExplosionAbilityCard::OnSelect()
{
    const auto explosion_ability = std::make_shared<ExplosionAbility>(m_player_data_, m_explosion_component_);
    m_ability_processer_->Attach(explosion_ability);
}

bool ExplosionAbilityCard::CanSelect()
{
    return !m_ability_processer_->HasAbility<ExplosionAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExplosionAbilityCard)
