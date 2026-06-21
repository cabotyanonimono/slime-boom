#pragma once
#include "player_data.h"
#include "player_data_presenter.h"
#include "player_sword_controller.h"
#include "../compute_result.h"
#include "../Component/animator.h"
#include "../Slime/slime_simulation_component.h"
#include "../StateMachine/state_machine.h"
#include "ability/ability_processer.h"
#include "ability/iability.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/transform.h"

namespace SlimeBoom
{
class PlayerComponent final : public engine::Component
{
    std::shared_ptr<StateMachine> m_state_machine_;
    AbilityProcesser m_ability_processer_;

    engine::AssetPtr<ComputeResult> m_compute_result_;
    engine::AssetPtr<PlayerDataPresenter> m_player_data_presenter_;
    engine::AssetPtr<PlayerSwordController> m_player_sword_controller_;
    engine::AssetPtr<engine::Transform> m_rotation_transform_;
    engine::AssetPtr<engine::Transform> m_camera_transform_;
    engine::AssetPtr<Animator> m_animator_;

    void UpdateParameter();

public:
    std::shared_ptr<PlayerData> player_data;

    void OnInspectorGui() override;
    void OnConstructed() override;
    void OnStart() override;
    void OnUpdate() override;
    void OnFixedUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(player_data),
                CEREAL_NVP(m_animator_),
                CEREAL_NVP(m_camera_transform_),
                CEREAL_NVP(m_rotation_transform_)
            );
        }
        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_player_data_presenter_),
                CEREAL_NVP(m_player_sword_controller_)
            );
        }
        if (version >= 5)
        {
            ar(
                CEREAL_NVP(m_compute_result_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerComponent, 5)