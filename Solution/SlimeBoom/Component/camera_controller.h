#pragma once
#include "map_size_component.h"
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
    Vector3 m_camera_position_offset_;
    engine::AssetPtr<engine::Transform> m_target_;
    engine::AssetPtr<engine::Transform> m_camera_transform_;
    engine::AssetPtr<MapSizeComponent> m_map_size_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_sensitivity),
            CEREAL_NVP(m_target_)
        );

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_camera_transform_),
                CEREAL_NVP(m_map_size_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_camera_position_offset_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::CameraController, 4)
