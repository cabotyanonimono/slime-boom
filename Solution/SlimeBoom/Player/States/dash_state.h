#pragma once
#include "../player_data.h"
#include "../player_data_component.h"
#include "../../Component/animator.h"
#include "../../StateMachine/state.h"
#include "Animation/animation_component.h"

namespace SlimeBoom
{
// プレイヤーの移動状態。入力した方向へ向きを変える。
class DashState : public State
{
    static constexpr float kRotationSpeed = 30.0f;

    std::string m_anim_name_;
    std::shared_ptr<PlayerDataComponent> m_player_data_;
    std::shared_ptr<Animator> m_animator_;
    std::shared_ptr<engine::Transform> m_camera_transform_;
    std::shared_ptr<engine::Transform> m_rotation_transform_;

    Vector3 m_move_direction_;

    /// 移動の効果音を再生し、移動アニメーションへ切り替える。
    void OnEnter() override;
    /// 移動入力から移動方向を取得する。
    void Update() override;
    /// 移動方向へモデルを回転させ、向きに応じてアニメーションのブレンド率を変える。
    /// また、アニメーションの再生速度をプレイヤーの移動速度に合わせる。
    void FixedUpdate() override;
    /// 移動の効果音を停止する。
    void OnExit() override;

public:
    /// @param animation_name 移動時に再生するアニメーション名
    /// @param player_data プレイヤーのステータス
    /// @param animator アニメーションの制御に使うAnimator
    /// @param camera_transform 移動方向の基準にするカメラのTransform
    /// @param rotation_transform 向きを回転させるモデルのTransform
    DashState(std::string animation_name, const std::shared_ptr<PlayerDataComponent> &player_data,
              const std::shared_ptr<Animator>& animator, const std::shared_ptr<engine::Transform>& camera_transform,
              const std::shared_ptr<engine::Transform>& rotation_transform);
};
}
