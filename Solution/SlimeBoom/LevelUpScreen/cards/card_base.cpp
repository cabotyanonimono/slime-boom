#include "pch.h"
#include "card_base.h"

void SlimeBoom::CardBase::OnInspectorGui()
{
    m_card_data_.OnInspectorGui();
}

bool SlimeBoom::CardBase::CanSelect()
{
    return true;
}

SlimeBoom::CardData& SlimeBoom::CardBase::GetCardData()
{
    return m_card_data_;
}

CEREAL_REGISTER_TYPE(SlimeBoom::CardBase)
