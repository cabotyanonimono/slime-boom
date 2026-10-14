#pragma once
#include "card_base.h"
#include "../../Player/player_data_component.h"

namespace SlimeBoom
{
// 攻撃回数を増やすカード。
class AttackNumCard : public CardBase
{
    uint32_t m_max_value_;
    uint32_t m_value_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;

public:
    void OnInspectorGui() override;
    void OnSelect() override;
    /// 攻撃回数が上限以下の間だけ選択可能。
    bool CanSelect() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<CardBase>(this),
            CEREAL_NVP(m_player_data_),
            CEREAL_NVP(m_value_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_max_value_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AttackNumCard, 1)