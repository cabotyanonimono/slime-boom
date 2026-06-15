#pragma once
#include "Asset/asset_ptr.h"
#include "Components/camera_component.h"
#include "Components/component.h"

namespace SlimeBoom
{
class CameraController : public engine::Component
{
    float m_yaw_;
    float m_pitch_;
    float m_sensitivity = 2.0f;
    engine::AssetPtr<engine::Transform> m_target_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this), CEREAL_NVP(m_sensitivity), CEREAL_NVP(m_target_));
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::CameraController, 1)