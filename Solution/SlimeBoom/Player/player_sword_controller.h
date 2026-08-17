#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/transform.h"
#include "Coroutine/task.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
class PlayerSwordController : public engine::Component
{
    bool m_is_playing_ = false;
    float m_duration_ = 0.0f;
    float m_current_time_ = 0.0f;
    Quaternion m_start_angle_;
    Quaternion m_end_angle_;
    float m_angle_ = 0.0f;
    float m_offset_ = 0.0f;

    engine::AssetPtr<engine::Transform> m_sword_transform_;
    engine::AssetPtr<engine::Transform> m_sword_parent_transform_;
    engine::AssetPtr<engine::Transform> m_player_transform_;
    
public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;

    void Play(Vector3 attack_pos, float offset);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_sword_transform_),
            CEREAL_NVP(m_sword_parent_transform_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_player_transform_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_angle_),
                CEREAL_NVP(m_offset_),
                CEREAL_NVP(m_duration_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerSwordController, 3)