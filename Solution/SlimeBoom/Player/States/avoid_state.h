#pragma once
#include "event.h"
#include "../player_data_component.h"
#include "../../Component/animator.h"
#include "../../StateMachine/state.h"

namespace SlimeBoom
{
// プレイヤーの回避状態。入力した方向へ回避速度で移動する。
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

    /// 入力方向を取得して回避イベントと効果音を発火し、回避アニメーションへ切り替える。
    void OnEnter() override;
    /// 入力方向をカメラ基準に変換し、回避速度でプレイヤーを移動させながらモデルをその方向へ回転させる。
    void Update() override;

public:
    /// @param animation_name 回避時に再生するアニメーション名
    /// @param player_data プレイヤーのステータス
    /// @param animator アニメーションの制御に使うAnimator
    /// @param camera_transform 移動方向の基準にするカメラのTransform
    /// @param rotation_transform 向きを回転させるモデルのTransform
    /// @param player_transform 移動させるプレイヤーのTransform
    /// @param on_avoid 回避開始時に発火するイベント
    AvoidState(std::string animation_name, const std::shared_ptr<PlayerDataComponent> &player_data,
              const std::shared_ptr<Animator>& animator, const std::shared_ptr<engine::Transform>& camera_transform,
              const std::shared_ptr<engine::Transform>& rotation_transform,
              const std::shared_ptr<engine::Transform>& player_transform,
              engine::Event<>* on_avoid);
};
}
