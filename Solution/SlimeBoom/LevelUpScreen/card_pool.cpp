#include "pch.h"
#include "card_pool.h"

#include "cards/attack_power_card.h"
#include "cards/attack_speed_card.h"
#include "cards/aura_ability_card.h"
#include "cards/exp_multiplier_card.h"
#include "cards/max_hp_card.h"
#include "cards/slash_ability_card.h"
#include "cards/speed_card.h"

namespace SlimeBoom
{
void CardPool::RegisterCards()
{
    RegisterCard<MaxHpCard>(kCardType_MaxHp);
    RegisterCard<AuraAbilityCard>(kCardType_AuraAbility);
    RegisterCard<SlashAbilityCard>(kCardType_Slash);
    RegisterCard<SpeedCard>(kCardType_Speed);
    RegisterCard<AttackPowerCard>(kCardType_AttackPower);
    RegisterCard<AttackSpeedCard>(kCardType_AttackSpeed);
    RegisterCard<ExpMultiplierCard>(kCardType_ExpMultiplier);
}

void CardPool::OnInspectorGui()
{
    for (const auto [card_type, card] : m_cards_)
    {
        ImGui::PushID(card_type);
        if (ImGui::CollapsingHeader(ToString(card_type)))
            card->OnInspectorGui();
        ImGui::PopID();
    }
}

void CardPool::OnConstructed()
{
    RegisterCards();
}

void CardPool::OnDeserialized()
{
    RegisterCards();
}

std::shared_ptr<CardBase> CardPool::GetCard(const kCardType type)
{
    return m_cards_.find(type)->second;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::CardPool)
