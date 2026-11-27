#pragma once
#include "ability_status_card.h"

namespace SlimeBoom
{
// オーラの攻撃速度倍率を強化するカード。
class AuraAttackSpeedCard : public AbilityStatusCard
{
protected:
    [[nodiscard]] kAbilityTypes GetAbilityType() const override;
    [[nodiscard]] kAbilityStatus GetStatusType() const override;
    bool HasTargetAbility() override;

public:
    AuraAttackSpeedCard();

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<CardBase>(this),
            CEREAL_NVP(m_ability_processer_),
            CEREAL_NVP(m_player_data_),
            CEREAL_NVP(m_value_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AuraAttackSpeedCard, 1)
