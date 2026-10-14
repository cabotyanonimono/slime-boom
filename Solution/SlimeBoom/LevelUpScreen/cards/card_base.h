#pragma once
#include "../card_data.h"
#include "Components/component.h"

namespace SlimeBoom
{
// レベルアップカードの基底クラス。見た目(CardData)と、選択時の効果を定義する。
class CardBase : public engine::Inspectable
{
protected:
    CardData m_card_data_;
    
public:
    void OnInspectorGui() override;
    /// カードが選択されたときに呼ばれ、効果を適用する。
    virtual void OnSelect() = 0;
    /// 抽選の候補にできるかを返す。上限に達しているカードや取得済みのカードはfalseを返す。
    /// デフォルトはtrue。
    virtual bool CanSelect();
    /// カードの見た目のデータを返す。
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