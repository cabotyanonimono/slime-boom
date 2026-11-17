#include "pch.h"
#include "level_up_card.h"

#include "engine_time.h"
#include "gui.h"
#include "Asset/asset_database.h"
#include "Rendering/font_data.h"

namespace SlimeBoom
{
void LevelUpCard::OnInspectorGui()
{
    engine::Gui::PropertyField("Card Icon", m_card_icon_);
    engine::Gui::PropertyField("Name", m_name_);
    engine::Gui::PropertyField("Description", m_description_);
    engine::Gui::PropertyField("Button", m_button_);
    engine::Gui::PropertyField("Background", m_background_);
    engine::Gui::PropertyField("Effect Text Offset", m_effect_text_offset_);
    engine::Gui::PropertyField("Effect Text Scale", m_effect_text_scale_);
    engine::Gui::PropertyField("Effect Text Color", m_effect_text_color_);
}

void LevelUpCard::OnStart()
{
    m_button_->AddEventListener([this]
    {
        m_card_->OnSelect();
    });

    // 効果の文字はASCIIしか持たないNoto Serif JPのフォントで描く（数値と記号だけなので足りる）
    m_effect_text_ = GameObject()->AddComponent<engine::TextRenderer>();
    m_effect_text_->font_data = engine::AssetDatabase::GetAsset<engine::FontData>("Font/Noto Serif JP.spritefont");
    UpdateEffectText();
}

void LevelUpCard::OnUpdate()
{
    UpdateEffectText();
}

void LevelUpCard::UpdateEffectText() const
{
    if (m_effect_text_ == nullptr)
        return;

    // 同じカードが続けて出ても数値は変わっているので、毎回作り直す
    m_effect_text_->string = m_card_ != nullptr ? m_card_->GetEffectText() : std::string();
    m_effect_text_->color = m_effect_text_color_;
    m_effect_text_->scale = m_effect_text_scale_;

    const auto font_data = m_effect_text_->font_data.CastedLock();
    const auto button = m_button_.CastedLock();
    if (font_data == nullptr || button == nullptr)
        return;

    m_effect_text_->origin = font_data->MeasureString(m_effect_text_->string) * 0.5f;
    // UIButtonのRectはposがボタンの中心なので、offsetの(0.5, 0.5)が中心になるようにずらす
    const auto rect = button->GetRect();
    m_effect_text_->position = rect.pos + rect.size * (m_effect_text_offset_ - Vector2(0.5f, 0.5f));
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
