#pragma once
#include "player_component.h"
#include "Asset/asset_ptr.h"
#include "Components/camera_component.h"
#include "Components/component.h"
#include "Physics/rigidbody_component.h"

namespace SlimeBoom
{
class PlayerController : public engine::Component
{
    float m_rotation_speed_;
    engine::AssetPtr<engine::CameraComponent> m_camera_;
    engine::AssetPtr<PlayerComponent> m_player_;
    engine::AssetPtr<engine::RigidbodyComponent> m_rigidbody_;
    engine::AssetPtr<engine::GameObject> m_rotation_object_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_rotation_speed_),
            CEREAL_NVP(m_camera_),
            CEREAL_NVP(m_player_),
            CEREAL_NVP(m_rigidbody_),
            CEREAL_NVP(m_rotation_object_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerController, 1)