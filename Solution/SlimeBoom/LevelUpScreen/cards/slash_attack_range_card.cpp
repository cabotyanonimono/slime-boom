#include "pch.h"
#include "slash_attack_range_card.h"

#include "../../Player/ability/slash_ability.h"

namespace SlimeBoom
{
SlashAttackRangeCard::SlashAttackRangeCard()
{
    m_value_ = 0.1f;
}

kAbilityTypes SlashAttackRangeCard::GetAbilityType() const
{
    return kAbilityTypesSlash;
}

kAbilityStatus SlashAttackRangeCard::GetStatusType() const
{
    return kAbilityStatusAttackRange;
}

bool SlashAttackRangeCard::HasTargetAbility()
{
    return m_ability_processer_->HasAbility<SlashAbility>();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlashAttackRangeCard)
