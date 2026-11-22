#pragma once
#include "root_motion_controller.h"
#include "Components/component.h"
#include "Physics/capsule_collider.h"
#include "Physics/rigidbody_component.h"

namespace SlimeBoom
{
// RigidbodyとCapsuleColliderを持つGameObjectを、キャラクターとして動かすComponent。
// AddMoveで渡された移動量とルートモーションの移動量を、毎ステップRigidbodyの速度にする。
// 接地判定、坂で滑らない処理、ジャンプ、移動前の当たり判定(壁抜け防止)を行う。
// Rigidbody / CapsuleCollider / RootMotionControllerと同じGameObjectに付けること。
class CharacterMotor final : public engine::Component
{
    static constexpr float kGroundNormalMinY = 0.5f;
    static constexpr float kJumpGroundIgnoreTime = 0.1f;
    static constexpr float kSweepSkin = 0.05f;
    static constexpr float kStepHeight = 0.3f;
    static constexpr int kMaxSlideIterations = 3;
    static constexpr float kGroundProbeDistance = 50.0f;

    std::weak_ptr<engine::RigidbodyComponent> m_rigidbody_;
    std::weak_ptr<RootMotionController> m_root_motion_;
    std::weak_ptr<engine::CapsuleCollider> m_capsule_;
    bool m_is_set_up_ = false;

    Vector3 m_pending_move_ = Vector3::Zero;
    float m_pending_frame_time_ = 0.0f;
    Vector3 m_move_velocity_ = Vector3::Zero;
    bool m_is_grounded_ = false;
    //直前のステップで接地していたか(外から参照する用)
    bool m_was_grounded_ = false;
    Vector3 m_ground_normal_ = Vector3::Up;
    Vector3 m_intended_planar_velocity_ = Vector3::Zero;
    //ジャンプの初速
    float m_jump_speed_ = 17.0f;
    bool m_jump_requested_ = false;
    bool m_is_jumping_ = false;
    float m_jump_ground_ignore_timer_ = 0.0f;

    /// 同じGameObjectのRigidbodyなどを取得し、回転の固定と摩擦0を設定する。
    void SetUp();
    /// 衝突の接触点から地面(上向きの法線)を探し、接地状態を更新する。
    void UpdateGround(const engine::Collision& collision);
    /// 溜まった移動量をRigidbodyの速度にする。
    /// 接地中は坂に沿った重力を打ち消して滑り落ちないようにし、移動は坂の面に沿わせる。
    void ApplyMovement();
    /// Capsuleをdisplacementだけ動かす前に当たりを調べ、壁があれば手前で止めて壁に沿って滑らせた移動量を返す。
    /// 速い移動(回避など)でも壁をすり抜けないようにするため。
    Vector3 SlideMove(const Vector3& displacement) const;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;
    void OnFixedUpdate() override;
    void OnCollisionEnter(const engine::Collision& collision) override;
    void OnCollisionStay(const engine::Collision& collision) override;

    /// 次の物理ステップで動かす移動量(World空間)を足す。Y成分は無視する。
    void AddMove(const Vector3& move);
    /// 次の物理ステップで接地していればジャンプする。空中で呼んだ場合は無視される。
    void Jump();
    /// 直前の物理ステップで接地していたかを返す。
    bool IsGrounded() const;
    /// Rigidbodyの現在の速度(World空間)を返す。
    Vector3 Velocity() const;
    /// 直前の物理ステップでこのComponentが狙った水平速度(Y=0)を返す。
    /// Velocity()と違い、地形の継ぎ目などに当たってBulletが削った分を含まないので、慣性の引き継ぎに使う。
    /// 壁に当たって止められた分(SlideMove)は反映される。
    Vector3 IntendedPlanarVelocity() const;
    /// 今の縦速度と重力から、着地するまでの予測時間(秒)を返す。接地中は0、下に地面が見つからなければ非常に大きな値。
    float EstimateTimeToGround() const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_jump_speed_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::CharacterMotor, 1)
