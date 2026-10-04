#pragma once
#include "gauge.h"
#include "ui_data_provider.h"
#include "../Exp/level_up_controller.h"
#include "Asset/asset_ptr.h"

namespace SlimeBoom
{
class ExpGauge : public engine::Component
{
    engine::AssetPtr<UiDataProvider> m_ui_data_provider_;
    engine::AssetPtr<Gauge> m_gauge_;
    
public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this),
           CEREAL_NVP(m_gauge_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_ui_data_provider_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ExpGauge, 2)