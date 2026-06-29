#pragma once
#include "../box.h"
#include "Asset/asset_ptr.h"
#include "Components/image.h"

namespace SlimeBoom
{
class Gauge : public engine::Component
{
    float m_ratio_;
    
    Box m_box_;
    engine::AssetPtr<engine::Image> m_bar_image_;
    engine::AssetPtr<engine::Image> m_bar_frame_image_;

    void ApplyImageSize() const;

public:
    Gauge();
    
    void OnInspectorGui() override;
    void OnUpdate() override;


    float Ratio() const;
    void SetRatio(float ratio);
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this),
           CEREAL_NVP(m_box_),
           CEREAL_NVP(m_bar_image_),
           CEREAL_NVP(m_bar_frame_image_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::Gauge, 1)