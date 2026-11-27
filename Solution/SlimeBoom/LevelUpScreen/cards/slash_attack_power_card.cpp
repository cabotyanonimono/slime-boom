#include "pch.h"
#include "slash_attack_power_card.h"

#include "../../Player/ability/slash_ability.h"

namespace SlimeBoom
{
SlashAttackPowerCard::SlashAttackPowerCard()
{
    m_value_ = 0.2f;
}

kAbilityTypes SlashAttackPowerCard::GetAbilityType() const
{
    return kAbilityTypesSlash;
}

kAbilityStatus SlashAttackPowerCard::GetStatusType() const
{
    return kAbilityStatusAttackPower;
}

bool SlashAttackPowerCard::HasTargetAbility()
{
    return m_ability_processer_->HasAbility<SlashAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlashAttackPowerCard)
