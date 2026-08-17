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
class LevelUpCard : public engine::Component
{
    std::shared_ptr<CardBase> m_card_;
    engine::AssetPtr<engine::Image> m_card_icon_;
    engine::AssetPtr<engine::Image> m_name_;
    engine::AssetPtr<engine::Image> m_description_;
    engine::AssetPtr<engine::Image> m_background_;
    engine::AssetPtr<ui::UIButton> m_button_;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    
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
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::LevelUpCard, 5)
