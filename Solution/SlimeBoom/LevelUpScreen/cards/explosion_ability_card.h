#pragma once
#include "card_base.h"
#include "../../Player/player_data_component.h"
#include "../../Player/ability/ability_processer.h"
#include "../../Slime/slime_explosion_component.h"

namespace SlimeBoom
{
// 倒したスライムが連鎖爆発するアビリティを習得するカード。
class ExplosionAbilityCard : public CardBase
{
    engine::AssetPtr<AbilityProcesser> m_ability_processer_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    engine::AssetPtr<SlimeExplosionComponent> m_explosion_component_;

public:
    void OnInspectorGui() override;
    /// ExplosionAbilityを生成して、AbilityProcesserに追加する。
    void OnSelect() override;
    /// 連鎖爆発アビリティを未習得の場合のみ選択可能。
    bool CanSelect() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<CardBase>(this),
            CEREAL_NVP(m_ability_processer_),
            CEREAL_NVP(m_player_data_),
            CEREAL_NVP(m_explosion_component_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ExplosionAbilityCard, 1)
