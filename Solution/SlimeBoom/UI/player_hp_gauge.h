#pragma once
#include "gauge.h"
#include "ui_data_provider.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/text_renderer.h"

namespace SlimeBoom
{
class PlayerHpGauge final : public engine::Component
{
    engine::AssetPtr<Gauge> m_gauge_;
    engine::AssetPtr<engine::TextRenderer> m_hp_text_;
    engine::AssetPtr<UiDataProvider> m_ui_data_provider_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_gauge_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_ui_data_provider_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_hp_text_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerHpGauge, 3)