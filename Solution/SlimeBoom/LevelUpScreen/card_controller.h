#pragma once
#include "card_pool.h"
#include "../Exp/level_up_controller.h"
#include "Asset/asset_ptr.h"
#include "cards/max_hp_card.h"
#include "Components/component.h"

namespace SlimeBoom
{
// レベルアップ時に提示するカード(kCardCount枚)を抽選して保持するComponent。
class CardController final : public engine::Component
{
public:
    static constexpr int kCardCount = 3;

private:
    engine::AssetPtr<CardPool> m_pool_;
    engine::AssetPtr<AbilityProcesser> m_ability_processer_;
    std::array<std::shared_ptr<CardBase>, kCardCount> m_cards_;

    /// アビリティを1つも持っていないときに提示する、アビリティ取得用のカードを返す。
    std::array<std::shared_ptr<CardBase>, kCardCount> GetAbilityCards() const;

public:
    void OnInspectorGui() override;
    void OnStart() override;

    /// 提示するカードを抽選し直す。
    /// アビリティを持っていない場合は取得用のカードを提示し、持っている場合は重複しないランダムなカードを選ぶ。
    void GenerateRandomCards();
    /// 選択可能なカードをランダムに選び、cardsと重複した場合は選び直す。
    /// @param cards 重複を避ける対象のカード
    /// @param card 結果で上書きされる
    /// @return cardsと重複しない、選択可能なカード
    std::shared_ptr<CardBase> GetOriginalCard(std::array<std::shared_ptr<CardBase>, kCardCount> cards, std::shared_ptr<CardBase> card);
    /// CanSelectがtrueのカードが選ばれるまで、ランダムにカードを選び直して返す。
    std::shared_ptr<CardBase> GetSelectableCard();

    /// 現在提示しているカードを返す。
    std::array<std::shared_ptr<CardBase>, kCardCount> GetCards();

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_pool_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_ability_processer_)
            );
        }
    }
};

inline std::array<std::shared_ptr<CardBase>, CardController::kCardCount> CardController::GetCards()
{
    return m_cards_;
}
}

CEREAL_CLASS_VERSION(SlimeBoom::CardController, 2)
