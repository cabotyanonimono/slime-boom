#pragma once
#include "player_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/Cinema/cinema_brain_component.h"
#include "Coroutine/task.h"

namespace SlimeBoom
{
class AvoidEffect : public engine::Component
{
    float m_in_time_ = 0.0f;
    float m_out_time_ = 0.0f;
    float m_base_fov_ = 0.0f;
    float m_avoid_fov_ = 0.0f;
    engine::AssetPtr<PlayerComponent> m_player_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    engine::AssetPtr<engine::CameraComponent> m_camera_component_;

    engine::Task FovEffect();

public:
    void OnInspectorGui() override;
    void OnStart() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_in_time_),
            CEREAL_NVP(m_out_time_),
            CEREAL_NVP(m_base_fov_),
            CEREAL_NVP(m_avoid_fov_),
            CEREAL_NVP(m_camera_component_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_player_),
                CEREAL_NVP(m_player_data_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AvoidEffect, 2)
