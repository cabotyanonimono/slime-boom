#pragma once
#include "card_type.h"
#include "cards/card_base.h"
#include "Components/component.h"

namespace SlimeBoom
{
// すべての種類のカードを、kCardTypeをキーに保持するComponent。
class CardPool : public engine::Component
{
    std::unordered_map<kCardType, std::shared_ptr<CardBase>> m_cards_;

    /// すべての種類のカードを生成して登録する。
    void RegisterCards();
    
    /// カードTのインスタンスを生成し、card_typeに紐づけて登録する。
    template<typename T> requires std::is_base_of_v<CardBase, T>
    void RegisterCard(kCardType card_type);
    
public:
    void OnInspectorGui() override;
    /// 生成時にすべてのカードを登録する。
    void OnConstructed() override;
    /// デシリアライズ後に、未登録のカードを補って登録する。
    void OnDeserialized() override;

    /// 指定した種類のカードを返す。
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