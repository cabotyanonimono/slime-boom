#pragma once
#include "gauge.h"
#include "ui_data_provider.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/rect_transform.h"

namespace SlimeBoom
{
class AvoidCooldownGauge : public engine::Component
{
    bool m_is_transparent_ = false;
    float m_current_alpha_ = 1.0f;
    float m_gauge_transparent_time_ = 0.0f;
    engine::AssetPtr<Gauge> m_gauge_;
    engine::AssetPtr<UiDataProvider> m_ui_data_provider_;
    engine::AssetPtr<engine::RectTransform> m_bar_rect_transform_;
    engine::AssetPtr<engine::RectTransform> m_bar_frame_rect_transform_;
    engine::AssetPtr<engine::Transform> m_target_transform_;
    Vector2 m_offset_ = Vector2(0.0f, 0.0f);

    //Gaugeを少しずつ透明にしていく
    engine::Task GaugeTransparentTask();
    
public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_gauge_),
            CEREAL_NVP(m_ui_data_provider_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_target_transform_),
                CEREAL_NVP(m_offset_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_bar_rect_transform_),
                CEREAL_NVP(m_bar_frame_rect_transform_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_gauge_transparent_time_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AvoidCooldownGauge, 4)
