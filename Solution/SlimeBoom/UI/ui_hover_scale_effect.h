#pragma once
#include "ui_button.h"
#include "Asset/asset_ptr.h"
#include "Engine/Components/rect_transform.h"
#include "Components/component.h"

class UiHoverScaleEffect : public engine::Component
{
    engine::AssetPtr<ui::UIButton> m_ui_button_;
    engine::AssetPtr<engine::RectTransform> m_rect_transform_;
    float m_transition_time_ = 1.0f;
    Vector2 m_base_size_;
    float m_time_ = 0.0f;
    float m_rate_ = 0.0f;
    float m_target_scale_ = 1.0f;
    bool m_use_unscaled_delta_time_ = true;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_ui_button_),
                CEREAL_NVP(m_rect_transform_),
                CEREAL_NVP(m_transition_time_),
                CEREAL_NVP(m_target_scale_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_use_unscaled_delta_time_)
            );
        }
    }
};

CEREAL_CLASS_VERSION(UiHoverScaleEffect, 3)
