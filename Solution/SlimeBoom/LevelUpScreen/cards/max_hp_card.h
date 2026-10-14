#pragma once
#include "card_base.h"
#include "../../Player/player_component.h"

namespace SlimeBoom
{
// 最大HPを増やすカード。
class MaxHpCard : public CardBase
{
    float m_value_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;

public:
    void OnInspectorGui() override;
    /// 最大HPと現在のHPを、同じ量だけ増やす。
    void OnSelect() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<CardBase>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_player_data_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_value_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::MaxHpCard, 3)
