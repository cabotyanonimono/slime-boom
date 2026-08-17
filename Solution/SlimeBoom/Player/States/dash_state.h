#pragma once
#include "../player_data.h"
#include "../player_data_component.h"
#include "../../Component/animator.h"
#include "../../StateMachine/state.h"
#include "Animation/animation_component.h"

namespace SlimeBoom
{
class DashState : public State
{
    static constexpr float kRotationSpeed = 15.0f;

    std::string m_anim_name_;
    std::shared_ptr<PlayerDataComponent> m_player_data_;
    std::shared_ptr<Animator> m_animator_;
    std::shared_ptr<engine::Transform> m_camera_transform_;
    std::shared_ptr<engine::Transform> m_rotation_transform_;

    Vector3 m_move_direction_;

    void OnEnter() override;
    void Update() override;
    void FixedUpdate() override;
    void OnExit() override;

public:
    DashState(std::string animation_name, const std::shared_ptr<PlayerDataComponent> &player_data,
              const std::shared_ptr<Animator>& animator, const std::shared_ptr<engine::Transform>& camera_transform,
              const std::shared_ptr<engine::Transform>& rotation_transform);
};
}
