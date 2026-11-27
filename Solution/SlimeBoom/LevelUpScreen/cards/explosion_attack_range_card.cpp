#include "pch.h"
#include "explosion_attack_range_card.h"

#include "../../Player/ability/explosion_ability.h"

namespace SlimeBoom
{
ExplosionAttackRangeCard::ExplosionAttackRangeCard()
{
    m_value_ = 0.1f;
}

kAbilityTypes ExplosionAttackRangeCard::GetAbilityType() const
{
    return kAbilityTypesExplosion;
}

kAbilityStatus ExplosionAttackRangeCard::GetStatusType() const
{
    return kAbilityStatusAttackRange;
}

bool ExplosionAttackRangeCard::HasTargetAbility()
{
    return m_ability_processer_->HasAbility<ExplosionAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExplosionAttackRangeCard)
