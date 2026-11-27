#include "pch.h"
#include "aura_attack_power_card.h"

#include "../../Player/ability/aura_ability.h"

namespace SlimeBoom
{
AuraAttackPowerCard::AuraAttackPowerCard()
{
    m_value_ = 0.2f;
}

kAbilityTypes AuraAttackPowerCard::GetAbilityType() const
{
    return kAbilityTypesAura;
}

kAbilityStatus AuraAttackPowerCard::GetStatusType() const
{
    return kAbilityStatusAttackPower;
}

bool AuraAttackPowerCard::HasTargetAbility()
{
    return m_ability_processer_->HasAbility<AuraAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AuraAttackPowerCard)
