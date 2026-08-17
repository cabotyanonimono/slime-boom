#pragma once
#include "card_base.h"
#include "../../Player/player_data_component.h"
#include "Components/component.h"

namespace SlimeBoom
{
class AttackSpeedCard : public CardBase
{
    float m_value_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;

public:
    void OnInspectorGui() override;
    void OnSelect() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<CardBase>(this),
            CEREAL_NVP(m_player_data_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_value_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AttackSpeedCard, 2)
