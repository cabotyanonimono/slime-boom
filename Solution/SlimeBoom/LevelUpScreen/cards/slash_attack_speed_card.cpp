#include "pch.h"
#include "slash_attack_speed_card.h"

#include "../../Player/ability/slash_ability.h"

namespace SlimeBoom
{
SlashAttackSpeedCard::SlashAttackSpeedCard()
{
    m_value_ = 0.2f;
}

kAbilityTypes SlashAttackSpeedCard::GetAbilityType() const
{
    return kAbilityTypesSlash;
}

kAbilityStatus SlashAttackSpeedCard::GetStatusType() const
{
    return kAbilityStatusAttackSpeed;
}

bool SlashAttackSpeedCard::HasTargetAbility()
{
    return m_ability_processer_->HasAbility<SlashAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlashAttackSpeedCard)
