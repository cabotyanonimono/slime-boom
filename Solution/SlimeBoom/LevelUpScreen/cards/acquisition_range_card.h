#pragma once
#include "card_base.h"
#include "../../Exp/exp_simulation_component.h"

namespace SlimeBoom
{
// 経験値の獲得範囲を広げるカード。
class AcquisitionRangeCard : public CardBase
{
    float m_value_;
    engine::AssetPtr<ExpSimulationComponent> m_exp_simulation_;

public:
    void OnInspectorGui() override;
    /// 経験値の獲得範囲を強化する。
    void OnSelect() override;
    /// 選んだときの変化を「現在値  ->  選択後の値」で返す（能力の習得カードは"NEW!"）。
    std::string GetEffectText() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<CardBase>(this),
            CEREAL_NVP(m_exp_simulation_),
            CEREAL_NVP(m_value_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AcquisitionRangeCard, 1)