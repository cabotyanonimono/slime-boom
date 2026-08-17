#include "pch.h"
#include "level_up_card.h"

#include "engine_time.h"

namespace SlimeBoom
{
void LevelUpCard::OnInspectorGui()
{
    engine::Gui::PropertyField("Card Icon", m_card_icon_);
    engine::Gui::PropertyField("Name", m_name_);
    engine::Gui::PropertyField("Description", m_description_);
    engine::Gui::PropertyField("Button", m_button_);
    engine::Gui::PropertyField("Background", m_background_);
}

void LevelUpCard::OnStart()
{
    m_button_->AddEventListener([this]
    {
        m_card_->OnSelect();
    });
}

void LevelUpCard::SetCard(const std::shared_ptr<CardBase>& card)
{
    if (card == nullptr || card == m_card_)
        return;

    m_card_ = card;

    auto card_data = m_card_->GetCardData();
    auto texture_data = m_card_icon_->shared_material->shared_material_block->GetTextureBufferData("_MainTex");
    texture_data->SetTexture(card_data.icon);

    texture_data = m_name_->shared_material->shared_material_block->GetTextureBufferData("_MainTex");
    texture_data->SetTexture(card_data.name);

    texture_data = m_description_->shared_material->shared_material_block->GetTextureBufferData("_MainTex");
    texture_data->SetTexture(card_data.description);

    texture_data = m_background_->shared_material->shared_material_block->GetTextureBufferData("_MainTex");
    texture_data->SetTexture(card_data.background);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::LevelUpCard)
