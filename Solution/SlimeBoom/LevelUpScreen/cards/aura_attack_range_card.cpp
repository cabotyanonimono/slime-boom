#include "pch.h"
#include "aura_attack_range_card.h"

#include "../../Player/ability/aura_ability.h"

namespace SlimeBoom
{
AuraAttackRangeCard::AuraAttackRangeCard()
{
    m_value_ = 0.1f;
}

kAbilityTypes AuraAttackRangeCard::GetAbilityType() const
{
    return kAbilityTypesAura;
}

kAbilityStatus AuraAttackRangeCard::GetStatusType() const
{
    return kAbilityStatusAttackRange;
}

bool AuraAttackRangeCard::HasTargetAbility()
{
    return m_ability_processer_->HasAbility<AuraAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AuraAttackRangeCard)
