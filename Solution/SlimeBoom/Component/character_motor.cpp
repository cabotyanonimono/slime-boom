#include "pch.h"
#include "character_motor.h"

#include "engine_time.h"
#include "game_object.h"
#include "gui.h"
#include "Physics/collision.h"
#include "Physics/physics.h"

namespace SlimeBoom
{
void CharacterMotor::SetUp()
{
    m_is_set_up_ = true;

    const auto rigidbody = GameObject()->GetComponent<engine::RigidbodyComponent>();
    m_rigidbody_ = rigidbody;
    m_root_motion_ = GameObject()->GetComponent<RootMotionController>();
    m_capsule_ = GameObject()->GetComponent<engine::CapsuleCollider>();
    if (rigidbody == nullptr)
    {
        engine::Logger::Warn<CharacterMotor>("[%s]: Rigidbody is not found.", GameObject()->Name().c_str());
        return;
    }

    //LockAxisはシリアライズされないので、ここで回転を固定する
    rigidbody->SetLockAxis(engine::kAxisAll);
    //速度は毎ステップこちらで決めるので摩擦は使わない。壁に押し付けたときに摩擦で落下が止まるのを防ぐ
    rigidbody->SetFriction(0.0f);
}

void CharacterMotor::UpdateGround(const engine::Collision& collision)
{
    for (const auto& contact : collision.GetContacts())
    {
        const auto normal = -contact.Normal();
        if (normal.y < kGroundNormalMinY)
            continue;

        if (!m_is_grounded_ || normal.y > m_ground_normal_.y)
            m_ground_normal_ = normal;
        m_is_grounded_ = true;
    }
}

void CharacterMotor::ApplyMovement()
{
    auto move = m_pending_move_;
    m_pending_move_ = Vector3::Zero;
    if (const auto root_motion = m_root_motion_.lock())
        move += root_motion->ConsumePendingDelta();

    const auto rigidbody = m_rigidbody_.lock();
    if (rigidbody == nullptr || !rigidbody->IsDynamic())
        return;

    const float dt = engine::Time::Get()->FixedDeltaTime();
    if (dt <= 0.0f)
        return;

    move.y = 0.0f;
    const auto move_velocity = move / dt;

    const auto velocity = rigidbody->Velocity();
    const auto gravity = engine::Physics::Gravity();

    bool is_grounded = m_is_grounded_;
    if (m_jump_ground_ignore_timer_ > 0.0f)
    {
        m_jump_ground_ignore_timer_ -= dt;
        is_grounded = false;
    }
    if (is_grounded)
        m_is_jumping_ = false;
    m_was_grounded_ = is_grounded;

    const bool jump = is_grounded && m_jump_requested_;
    //空中で押されたジャンプは持ち越さない
    m_jump_requested_ = false;

    //水平方向の移動は、動かす前に当たりを調べて壁の手前で止める(壁はY成分を持たないので空中では縦方向を足さない)
    auto planar_velocity = SlideMove(move_velocity * dt) / dt;
    planar_velocity.y = 0.0f;

    //SetVelocityするとそのステップのBulletの重力が消えるので、重力はこちらで足す
    Vector3 next_velocity;
    if (jump)
    {
        next_velocity = Vector3(planar_velocity.x, m_jump_speed_, planar_velocity.z);
        next_velocity += gravity * dt;
        m_is_jumping_ = true;
        m_jump_ground_ignore_timer_ = kJumpGroundIgnoreTime;
    }
    else if (is_grounded)
    {
        auto normal = m_ground_normal_;
        normal.Normalize();

        //地面から離れる向きの速度は残さない(ぶつかった反動で浮かないように)
        next_velocity = normal * std::min(velocity.Dot(normal), 0.0f);
        //重力は地面に垂直な分だけ足す(坂に沿う分を足さないので滑り落ちない)
        next_velocity += normal * gravity.Dot(normal) * dt;
        //接地中は坂の面に沿わせてから当たりを調べる
        const auto slope_velocity = move_velocity - normal * move_velocity.Dot(normal);
        next_velocity += SlideMove(slope_velocity * dt) / dt;
    }
    else
    {
        //ジャンプ以外の上向きの速度(坂を登っていた勢いなど)は、地面から離れたら消す
        const float vertical = m_is_jumping_ ? velocity.y : std::min(velocity.y, 0.0f);
        next_velocity = Vector3(planar_velocity.x, vertical, planar_velocity.z);
        next_velocity += gravity * dt;
    }

    rigidbody->SetVelocity(next_velocity);
    rigidbody->SetAngularVelocity(Vector3::Zero);
}

Vector3 CharacterMotor::SlideMove(const Vector3& displacement) const
{
    const auto capsule = m_capsule_.lock();
    if (capsule == nullptr)
        return displacement;

    const float radius = capsule->Radius();
    //CapsuleColliderは高さ0.5の円柱をHeight倍している
    const float cylinder_height = capsule->Height() * 0.5f;

    //下側をkStepHeightだけ削り、足元の地面や小さな段差に引っかからないようにする
    const float lift = std::min(kStepHeight, cylinder_height);
    auto position = GameObject()->Transform()->Position() + capsule->Offset() + Vector3(0.0f, lift * 0.5f, 0.0f);
    const float sweep_cylinder_height = cylinder_height - lift;

    Vector3 result = Vector3::Zero;
    Vector3 remaining = displacement;
    for (int i = 0; i < kMaxSlideIterations; ++i)
    {
        const float length = remaining.Length();
        if (length <= 1e-5f)
            break;

        engine::SweepHit hit;
        if (!engine::Physics::CapsuleSweep(position, position + remaining, radius, sweep_cylinder_height, hit))
        {
            result += remaining;
            break;
        }

        const float distance = std::max(hit.fraction * length - kSweepSkin, 0.0f);
        const auto step = remaining / length * distance;
        result += step;
        position += step;

        //急な面は壁として扱い、法線を水平にする(壁を登らないように)
        auto normal = hit.normal;
        if (normal.y < kGroundNormalMinY)
        {
            normal.y = 0.0f;
            if (normal.LengthSquared() <= 1e-6f)
                break;
            normal.Normalize();
        }
        const auto leftover = remaining - step;
        remaining = leftover - normal * leftover.Dot(normal);
    }

    return result;
}

void CharacterMotor::OnInspectorGui()
{
    engine::Gui::PropertyField("Jump Speed", m_jump_speed_);
}

void CharacterMotor::OnStart()
{
    SetUp();
}

void CharacterMotor::OnFixedUpdate()
{
    //実行中に追加されてOnStartより先にFixedUpdateが来た場合に備える
    if (!m_is_set_up_)
        SetUp();

    ApplyMovement();

    //接地状態は物理ステップごとの衝突通知で立て直す
    m_is_grounded_ = false;
}

void CharacterMotor::OnCollisionEnter(const engine::Collision& collision)
{
    UpdateGround(collision);
}

void CharacterMotor::OnCollisionStay(const engine::Collision& collision)
{
    UpdateGround(collision);
}

void CharacterMotor::AddMove(const Vector3& move)
{
    m_pending_move_ += move;
}

void CharacterMotor::Jump()
{
    m_jump_requested_ = true;
}

bool CharacterMotor::IsGrounded() const
{
    return m_was_grounded_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::CharacterMotor)
