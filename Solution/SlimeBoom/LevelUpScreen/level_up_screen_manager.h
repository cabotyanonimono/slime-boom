#pragma once
#include "card_controller.h"
#include "../UI/ui_button.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
class LevelUpScreenManager final : public engine::Component
{
    engine::AssetPtr<CardController> m_card_controller_;
    engine::AssetPtr<LevelUpController> m_level_up_controller_;
    engine::AssetPtr<engine::GameObject> m_level_up_ui_object_;
    std::array<engine::AssetPtr<ui::UIButton>, CardController::kCardCount> m_buttons_;
    
    void ScreenStart() const;
    void ScreenEnd() const;
    
public:
    void OnInspectorGui() override;
    void OnStart() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_card_controller_),
            CEREAL_NVP(m_level_up_controller_),
            CEREAL_NVP(m_level_up_ui_object_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_buttons_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::LevelUpScreenManager, 2)
