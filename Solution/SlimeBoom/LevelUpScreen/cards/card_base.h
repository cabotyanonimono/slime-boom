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
    /// カードを選んだときの変化を表す文字列を返す（例: "100  ->  110"）。
    /// 表示に使うフォントはASCIIのみなので、英数字と記号だけで組み立てる。デフォルトは空文字列。
    virtual std::string GetEffectText();

    /// 数値を小数第2位までの短い文字列にする（末尾の0は落とす）。
    static std::string FormatNumber(float value);
    /// 「変化前  ->  変化後」の文字列を作る。prefix/suffixは両方の数値に付ける（"x"や"%"など）。
    static std::string FormatChange(float before, float after, const std::string& prefix = "",
                                    const std::string& suffix = "");
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