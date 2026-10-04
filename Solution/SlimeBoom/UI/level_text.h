#pragma once
#include "ui_data_provider.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/text_renderer.h"

namespace SlimeBoom
{
class LevelText : public engine::Component
{
    engine::AssetPtr<engine::TextRenderer> m_level_text_;
    engine::AssetPtr<UiDataProvider> m_ui_data_provider_;
    
public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_level_text_),
            CEREAL_NVP(m_ui_data_provider_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::LevelText, 1)
