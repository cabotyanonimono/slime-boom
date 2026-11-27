#include "pch.h"
#include "aura_attack_speed_card.h"

#include "../../Player/ability/aura_ability.h"

namespace SlimeBoom
{
AuraAttackSpeedCard::AuraAttackSpeedCard()
{
    m_value_ = 0.2f;
}

kAbilityTypes AuraAttackSpeedCard::GetAbilityType() const
{
    return kAbilityTypesAura;
}

kAbilityStatus AuraAttackSpeedCard::GetStatusType() const
{
    return kAbilityStatusAttackSpeed;
}

bool AuraAttackSpeedCard::HasTargetAbility()
{
    return m_ability_processer_->HasAbility<AuraAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AuraAttackSpeedCard)
