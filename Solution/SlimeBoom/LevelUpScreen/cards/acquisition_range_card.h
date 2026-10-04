#pragma once
#include "card_base.h"
#include "../../Exp/exp_simulation_component.h"

namespace SlimeBoom
{
class AcquisitionRangeCard : public CardBase
{
    float m_value_;
    engine::AssetPtr<ExpSimulationComponent> m_exp_simulation_;

public:
    void OnInspectorGui() override;
    void OnSelect() override;

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