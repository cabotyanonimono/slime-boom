#pragma once
#include "event.h"
#include "../player_data_component.h"
#include "../../Component/animator.h"
#include "../../StateMachine/state.h"

namespace SlimeBoom
{
class AvoidState : public State
{
    static constexpr float kRotationSpeed = 60.0f;
    static constexpr float kAnimationRotationOffset = -40.0f;

    engine::Event<>* m_on_avoid_;
    std::string m_anim_name_;
    std::shared_ptr<Animator> m_animator_;
    std::shared_ptr<PlayerDataComponent> m_player_data_;
    std::shared_ptr<engine::Transform> m_camera_transform_;
    std::shared_ptr<engine::Transform> m_rotation_transform_;
    std::shared_ptr<engine::Transform> m_player_transform_;

    Vector3 m_move_direction_;
    std::string m_animation_name_;

    void OnEnter() override;
    void Update() override;

public:
    AvoidState(std::string animation_name, const std::shared_ptr<PlayerDataComponent> &player_data,
              const std::shared_ptr<Animator>& animator, const std::shared_ptr<engine::Transform>& camera_transform,
              const std::shared_ptr<engine::Transform>& rotation_transform,
              const std::shared_ptr<engine::Transform>& player_transform,
              engine::Event<>* on_avoid);
};
}
