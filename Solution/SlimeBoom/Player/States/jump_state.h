#pragma once
#include "../../Component/animator.h"
#include "../../Component/character_motor.h"
#include "../../Component/root_motion_controller.h"
#include "../../StateMachine/state.h"
#include "Components/transform.h"

namespace SlimeBoom
{
// プレイヤーのジャンプ(空中)状態。1本のジャンプアニメーションを
// 踏み切り → 空中ループ → 着地 の3区間に分けて再生し、入力した方向へ向きを変える。
// 縦方向の移動と接地判定はCharacterMotorが行う。
// 空中ではルートモーションを止め、状態に入ったときの水平速度のまま進む。
class JumpState : public State
{
    enum class kPhase
    {
        kTakeoff,
        kAir,
        kLand,
    };

    static constexpr float kRotationSpeed = 30.0f;
    static constexpr float kBlendSpeed = 10.0f;
    static constexpr float kFrameRate = 30.0f;
    static constexpr float kMinLandSpeed = 0.5f;
    static constexpr float kMaxLandSpeed = 3.0f;

    // Jump.fbx(Mixamo: Forward Running Jump をBlenderで再構成, 30fps, 61フレーム)の区間
    //  0-11: 元の踏み切り / 11-15: 空中ポーズへ移行 / 15-39: 空中ループ(始点と終点が同じポーズ)
    //  39-43: 落下姿勢へ移行 / 43-60: 元の着地
    static constexpr float kTakeoffEnd = 15.0f / kFrameRate;
    static constexpr float kAirLoopEnd = 39.0f / kFrameRate;
    static constexpr float kAirLoopLength = kAirLoopEnd - kTakeoffEnd;
    static constexpr float kLandStart = 39.0f / kFrameRate;
    static constexpr float kContact = 51.0f / kFrameRate;

    std::string m_anim_name_;
    std::shared_ptr<Animator> m_animator_;
    std::shared_ptr<engine::Transform> m_camera_transform_;
    std::shared_ptr<engine::Transform> m_rotation_transform_;
    std::function<std::shared_ptr<CharacterMotor>()> m_get_motor_;
    std::function<void()> m_on_finished_;

    kPhase m_phase_ = kPhase::kTakeoff;
    Vector3 m_carry_velocity_ = Vector3::Zero;

    /// 踏み切り区間(空中で入った場合は空中ループ)から再生を始め、ルートモーションを止める。
    void OnEnter() override;
    /// 区間ごとの再生位置を制御し、水平方向に進ませ、移動入力の方向へモデルを回転させる。
    void Update() override;
    /// 再生速度とルートモーションを元に戻す。
    void OnExit() override;

    /// アニメーションの区間を進める。アニメーションが未登録の場合は着地したら終了する。
    void UpdatePhase(const std::shared_ptr<CharacterMotor>& motor);
    /// WASDの移動入力(カメラ基準のローカル方向、未正規化)を返す。
    static Vector3 ReadMoveInput();
    /// 移動入力の方向へモデルを回転させる。
    void UpdateRotation() const;
    /// 着地区間へ移る。
    void StartLand(const std::shared_ptr<engine::AnimationState>& state);
    /// ルートモーションの有効/無効を切り替える。
    void SetRootMotionEnabled(bool enabled) const;

public:
    /// @param animation_name ジャンプ時に再生するアニメーション名
    /// @param animator アニメーションの制御に使うAnimator
    /// @param camera_transform 移動方向の基準にするカメラのTransform
    /// @param rotation_transform 向きを回転させるモデルのTransform
    /// @param get_motor プレイヤーを動かすCharacterMotorを返す関数
    /// @param on_finished 着地したときに呼ばれる関数
    JumpState(std::string animation_name, const std::shared_ptr<Animator>& animator,
              const std::shared_ptr<engine::Transform>& camera_transform,
              const std::shared_ptr<engine::Transform>& rotation_transform,
              std::function<std::shared_ptr<CharacterMotor>()> get_motor,
              std::function<void()> on_finished);
};
}
