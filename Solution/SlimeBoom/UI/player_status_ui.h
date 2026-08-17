#pragma once
#include "ui_data_provider.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/text_renderer.h"

namespace SlimeBoom
{
class PlayerStatusUi : public engine::Component
{
    engine::AssetPtr<UiDataProvider> m_ui_data_provider_;
    engine::AssetPtr<engine::TextRenderer> m_max_hp_text_;
    engine::AssetPtr<engine::TextRenderer> m_speed_text_;
    engine::AssetPtr<engine::TextRenderer> m_attack_power_text_;
    engine::AssetPtr<engine::TextRenderer> m_attack_speed_text_;
    engine::AssetPtr<engine::TextRenderer> m_avoid_time_text_;
    engine::AssetPtr<engine::TextRenderer> m_avoid_speed_text_;
    engine::AssetPtr<engine::TextRenderer> m_exp_multiplier_text_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_ui_data_provider_),
            CEREAL_NVP(m_max_hp_text_),
            CEREAL_NVP(m_speed_text_),
            CEREAL_NVP(m_attack_power_text_),
            CEREAL_NVP(m_attack_speed_text_),
            CEREAL_NVP(m_avoid_time_text_),
            CEREAL_NVP(m_avoid_speed_text_),
            CEREAL_NVP(m_exp_multiplier_text_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerStatusUi, 1)