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
    //ジャンプ直後に地面との接触が残っていても、接地とみなさない時間(秒)
    static constexpr float kJumpGroundIgnoreTime = 0.1f;
    //移動前の当たり判定で、壁の手前に残す隙間
    static constexpr float kSweepSkin = 0.05f;
    //移動前の当たり判定でCapsuleの下側を削る高さ。これより低い段差や坂の継ぎ目には引っかからない
    static constexpr float kStepHeight = 0.3f;
    //壁に当たったあと、壁に沿って滑らせ直す最大回数
    static constexpr int kMaxSlideIterations = 3;

    std::weak_ptr<engine::RigidbodyComponent> m_rigidbody_;
    std::weak_ptr<RootMotionController> m_root_motion_;
    std::weak_ptr<engine::CapsuleCollider> m_capsule_;
    bool m_is_set_up_ = false;

    Vector3 m_pending_move_ = Vector3::Zero;
    bool m_is_grounded_ = false;
    //直前のステップで接地していたか(外から参照する用)
    bool m_was_grounded_ = false;
    Vector3 m_ground_normal_ = Vector3::Up;
    //ジャンプの初速
    float m_jump_speed_ = 17.0f;
    bool m_jump_requested_ = false;
    //ジャンプで上昇中か。上向きの速度はジャンプ中だけ許す(坂の頂上で飛び出さないように)
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
    void OnFixedUpdate() override;
    void OnCollisionEnter(const engine::Collision& collision) override;
    void OnCollisionStay(const engine::Collision& collision) override;

    /// 次の物理ステップで動かす移動量(World空間)を足す。Y成分は無視する。
    void AddMove(const Vector3& move);
    /// 次の物理ステップで接地していればジャンプする。空中で呼んだ場合は無視される。
    void Jump();
    /// 直前の物理ステップで接地していたかを返す。
    bool IsGrounded() const;

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
