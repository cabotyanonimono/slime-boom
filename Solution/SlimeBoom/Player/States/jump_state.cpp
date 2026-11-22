#include "pch.h"
#include "jump_state.h"
#include "engine_time.h"
#include "game_object.h"
#include "input.h"
#include "../../Utils/math_util.h"

void SlimeBoom::JumpState::OnEnter()
{
    const auto motor = m_get_motor_();

    const bool in_air = motor != nullptr && !motor->IsGrounded();
    m_phase_ = in_air ? kPhase::kAir : kPhase::kTakeoff;

    m_carry_velocity_ = motor != nullptr ? motor->IntendedPlanarVelocity() : Vector3::Zero;
    SetRootMotionEnabled(false);

    const auto state = m_animator_->GetState(m_anim_name_);
    if (state == nullptr)
        return;

    if (const auto clip = state->clip.CastedLock())
        state->length = clip->Length();

    state->speed = 1.0f;

    const auto current_anim_name = m_animator_->GetCurrentAnimName();
    if (current_anim_name != "" && current_anim_name != m_anim_name_)
        m_animator_->BlendAnim(current_anim_name, m_anim_name_, kBlendSpeed);
    else
        m_animator_->PlayAnim(m_anim_name_);

    state->time = in_air ? kTakeoffEnd : 0.0f;
    state->ReleaseCurvesCache();
}

void SlimeBoom::JumpState::Update()
{
    const auto motor = m_get_motor_();
    if (motor != nullptr)
        motor->AddMove(m_carry_velocity_ * engine::Time::GetDeltaTime());

    UpdatePhase(motor);
    UpdateRotation();
}

void SlimeBoom::JumpState::OnExit()
{
    if (const auto state = m_animator_->GetState(m_anim_name_))
        state->speed = 1.0f;
    SetRootMotionEnabled(true);
}

void SlimeBoom::JumpState::SetRootMotionEnabled(const bool enabled) const
{
    const auto motor = m_get_motor_();
    if (motor == nullptr)
        return;
    if (const auto root_motion = motor->GameObject()->GetComponent<RootMotionController>())
        root_motion->SetEnabledMotion(enabled);
}

void SlimeBoom::JumpState::UpdatePhase(const std::shared_ptr<CharacterMotor>& motor)
{
    const bool is_grounded = motor == nullptr || motor->IsGrounded();
    const auto time_to_ground = motor != nullptr ? motor->EstimateTimeToGround() : 0.0f;

    const auto state = m_animator_->GetState(m_anim_name_);
    if (state == nullptr)
    {
        if (is_grounded && m_phase_ != kPhase::kTakeoff)
            m_on_finished_();
        if (!is_grounded)
            m_phase_ = kPhase::kAir;
        return;
    }

    switch (m_phase_)
    {
    case kPhase::kTakeoff:
    case kPhase::kAir:
    {
        if (is_grounded)
        {
            // 踏み切り中はまだ地面を離れていない(ジャンプ直後は物理の更新前で接地扱いのまま)
            if (m_phase_ == kPhase::kAir)
                StartLand(state);
            else if (state->time >= kTakeoffEnd)
                m_phase_ = kPhase::kAir;
            break;
        }

        // ループ終点からそのまま着地区間へ続くので、今から順再生して足が着くまでに残っている時間で判断する
        const auto remaining_to_contact = std::max(kAirLoopEnd - state->time, 0.0f) + (kContact - kLandStart);
        if (time_to_ground <= remaining_to_contact)
        {
            m_phase_ = kPhase::kLand;
            break;
        }

        if (state->time >= kTakeoffEnd)
            m_phase_ = kPhase::kAir;

        if (m_phase_ == kPhase::kAir && state->time >= kAirLoopEnd)
        {
            state->time -= kAirLoopLength;
            state->ReleaseCurvesCache();
        }
        break;
    }

    case kPhase::kLand:
        if (!is_grounded)
        {
            if (state->time >= kContact)
            {
                state->time = kContact;
                state->speed = 0.0f;
                break;
            }

            const auto safe_time_to_ground = std::max(time_to_ground, 0.001f);
            state->speed = std::clamp((kContact - state->time) / safe_time_to_ground, kMinLandSpeed, kMaxLandSpeed);
            break;
        }

        m_on_finished_();
        break;
    }
}

void SlimeBoom::JumpState::StartLand(const std::shared_ptr<engine::AnimationState>& state)
{
    m_phase_ = kPhase::kLand;

    if (state->time < kLandStart)
        state->time = kLandStart;
    state->speed = 1.0f;
    state->ReleaseCurvesCache();
}

Vector3 SlimeBoom::JumpState::ReadMoveInput()
{
    Vector3 move_direction = Vector3::Zero;
    if (engine::Input::GetKey(DirectX::Keyboard::W))
        move_direction.z += 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::S))
        move_direction.z -= 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::A))
        move_direction.x += 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::D))
        move_direction.x -= 1.0f;
    return move_direction;
}

void SlimeBoom::JumpState::UpdateRotation() const
{
    auto move_direction = ReadMoveInput();
    if (move_direction.Length() == 0)
        return;

    move_direction.Normalize();
    auto target_dir = Vector3::Transform(move_direction, m_camera_transform_->Rotation());
    target_dir.y = 0;
    target_dir.Normalize();

    m_rotation_transform_->SetRotation(MathUtils::LookRotationSmooth(target_dir, Vector3::Up,
                                                                     m_rotation_transform_->LocalRotation(),
                                                                     kRotationSpeed, engine::Time::GetDeltaTime()));
}

SlimeBoom::JumpState::JumpState(std::string animation_name, const std::shared_ptr<Animator>& animator,
                                const std::shared_ptr<engine::Transform>& camera_transform,
                                const std::shared_ptr<engine::Transform>& rotation_transform,
                                std::function<std::shared_ptr<CharacterMotor>()> get_motor,
                                std::function<void()> on_finished)
    : m_anim_name_(std::move(animation_name)),
      m_animator_(animator),
      m_camera_transform_(camera_transform),
      m_rotation_transform_(rotation_transform),
      m_get_motor_(std::move(get_motor)),
      m_on_finished_(std::move(on_finished))
{
}
