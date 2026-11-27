#include "pch.h"
#include "explosion_attack_power_card.h"

#include "../../Player/ability/explosion_ability.h"

namespace SlimeBoom
{
ExplosionAttackPowerCard::ExplosionAttackPowerCard()
{
    m_value_ = 0.2f;
}

kAbilityTypes ExplosionAttackPowerCard::GetAbilityType() const
{
    return kAbilityTypesExplosion;
}

kAbilityStatus ExplosionAttackPowerCard::GetStatusType() const
{
    return kAbilityStatusAttackPower;
}

bool ExplosionAttackPowerCard::HasTargetAbility()
{
    return m_ability_processer_->HasAbility<ExplosionAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExplosionAttackPowerCard)
