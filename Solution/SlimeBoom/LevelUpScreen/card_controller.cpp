#include "pch.h"
#include "card_controller.h"
#include "../Utils/random.h"

namespace SlimeBoom
{
void CardController::GenerateRandomCards()
{
    if (m_ability_processer_->GetAbilities().empty())
    {
        m_cards_ = GetAbilityCards();
        return;
    }
    
    for (auto &card : m_cards_)
    {
        card = GetSelectableCard();
    }
}

std::shared_ptr<CardBase> CardController::GetSelectableCard()
{
    auto random_card = m_pool_->GetCard(static_cast<kCardType>(Random(0, kCardType_Count - 1)));
    if (!random_card->CanSelect())
        random_card = GetSelectableCard();
    return random_card;
}

std::array<std::shared_ptr<CardBase>, CardController::kCardCount> CardController::GetAbilityCards() const
{
    std::array<std::shared_ptr<CardBase>, kCardCount> result;
    result[0] = m_pool_->GetCard(kCardType_AuraAbility);
    result[1] = m_pool_->GetCard(kCardType_Slash);
    result[2] = m_pool_->GetCard(kCardType_AuraAbility);
    return result;
}

void CardController::OnInspectorGui()
{
    engine::Gui::PropertyField("Card Pool", m_pool_);
    engine::Gui::PropertyField("Ability Processer", m_ability_processer_);
}

void CardController::OnStart()
{
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::CardController)