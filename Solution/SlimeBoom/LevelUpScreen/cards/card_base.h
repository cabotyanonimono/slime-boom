#pragma once
#include "../card_data.h"
#include "Components/component.h"

namespace SlimeBoom
{
class CardBase : public engine::Inspectable
{
protected:
    CardData m_card_data_;
    
public:
    void OnInspectorGui() override;
    virtual void OnSelect() = 0;
    virtual bool CanSelect();
    CardData& GetCardData();
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(m_card_data_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::CardBase, 1)