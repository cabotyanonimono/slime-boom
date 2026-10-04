#pragma once
#include "card_type.h"
#include "cards/card_base.h"
#include "Components/component.h"

namespace SlimeBoom
{
class CardPool : public engine::Component
{
    std::unordered_map<kCardType, std::shared_ptr<CardBase>> m_cards_;

    void RegisterCards();
    
    template<typename T> requires std::is_base_of_v<CardBase, T>
    void RegisterCard(kCardType card_type);
    
public:
    void OnInspectorGui() override;
    void OnConstructed() override;
    void OnDeserialized() override;

    std::shared_ptr<CardBase> GetCard(kCardType type);
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 3)
        {
            ar(CEREAL_NVP(m_cards_));
        }
    }
};

template <typename T> requires std::is_base_of_v<CardBase, T>
void CardPool::RegisterCard(kCardType card_type)
{
    m_cards_.emplace(card_type, std::make_shared<T>());
}
}

CEREAL_CLASS_VERSION(SlimeBoom::CardPool, 3)