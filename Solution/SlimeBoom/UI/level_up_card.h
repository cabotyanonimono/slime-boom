#pragma once
#include "ui_button.h"
#include "../LevelUpScreen/card_data.h"
#include "../LevelUpScreen/cards/card_base.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/image.h"
#include "Components/text_renderer.h"

namespace SlimeBoom
{
// レベルアップ画面のカード1枚分のUI。カードの見た目を表示し、ボタンが押されたらカードの効果を適用する。
class LevelUpCard : public engine::Component
{
    std::shared_ptr<CardBase> m_card_;
    engine::AssetPtr<engine::Image> m_card_icon_;
    engine::AssetPtr<engine::Image> m_name_;
    engine::AssetPtr<engine::Image> m_description_;
    engine::AssetPtr<engine::Image> m_background_;
    engine::AssetPtr<ui::UIButton> m_button_;

    // カードを選んだときの変化（"100  ->  110"など）を出す文字。シーンに置かず、OnStartで自分のGameObjectに追加する
    std::shared_ptr<engine::TextRenderer> m_effect_text_;
    // ボタンの矩形に対する、効果の文字の中心位置（左上が(0, 0)、右下が(1, 1)）
    Vector2 m_effect_text_offset_ = Vector2(0.5f, 0.8f);
    float m_effect_text_scale_ = 0.36f;
    Color m_effect_text_color_ = Color(1.0f, 0.78f, 0.3f, 1.0f);

    /// 効果の文字を、現在のカードの内容に更新する。
    void UpdateEffectText() const;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    /// 効果の文字をボタンの位置に合わせる。
    void OnUpdate() override;

    /// カードを設定し、カードのテクスチャ(アイコン・名前・説明・背景)を反映する。
    /// @param card 表示するカード。nullptrまたは設定済みと同じカードの場合は何もしない。
    void SetCard(const std::shared_ptr<CardBase>& card);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_card_icon_),
                CEREAL_NVP(m_name_),
                CEREAL_NVP(m_description_),
                CEREAL_NVP(m_button_)
            );
        }

        if (version >= 5)
        {
            ar(
                CEREAL_NVP(m_background_)
            );
        }

        if (version >= 6)
        {
            ar(
                CEREAL_NVP(m_effect_text_offset_),
                CEREAL_NVP(m_effect_text_scale_),
                CEREAL_NVP(m_effect_text_color_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::LevelUpCard, 6)
