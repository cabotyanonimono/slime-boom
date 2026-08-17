#pragma once
#include "card_pool.h"
#include "../Exp/level_up_controller.h"
#include "Asset/asset_ptr.h"
#include "cards/max_hp_card.h"
#include "Components/component.h"

namespace SlimeBoom
{
class CardController final : public engine::Component
{
public:
    static constexpr int kCardCount = 3;

private:
    engine::AssetPtr<CardPool> m_pool_;
    engine::AssetPtr<AbilityProcesser> m_ability_processer_;
    std::array<std::shared_ptr<CardBase>, kCardCount> m_cards_;

    std::array<std::shared_ptr<CardBase>, kCardCount> GetAbilityCards() const;

public:
    void OnInspectorGui() override;
    void OnStart() override;

    void GenerateRandomCards();
    std::shared_ptr<CardBase> GetSelectableCard();

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
