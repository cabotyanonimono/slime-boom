#include "pch.h"
#include "dash_state.h"
#include "engine_time.h"
#include "input.h"
#include "../../Utils/math_util.h"

void SlimeBoom::DashState::OnEnter()
{
    auto current_anim_name = m_animator_->GetCurrentAnimName();
    if (current_anim_name != "")
    {
        m_animator_->BlendAnim(current_anim_name, m_anim_name_, 1.0f);
        return;
    }

    m_animator_->PlayAnim(m_anim_name_);
}

void SlimeBoom::DashState::Update()
{
    m_move_direction_ = Vector3::Zero;
    if (engine::Input::GetKey(DirectX::Keyboard::W))
        m_move_direction_.z += 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::S))
        m_move_direction_.z -= 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::A))
        m_move_direction_.x += 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::D))
        m_move_direction_.x -= 1.0f;
}

void SlimeBoom::DashState::FixedUpdate()
{
    if (m_move_direction_.Length() == 0)
        return;

    m_move_direction_.Normalize();
    auto target_dir = Vector3::Transform(m_move_direction_, m_camera_transform_->Rotation());
    target_dir.y = 0;
    target_dir.Normalize();

    m_rotation_transform_->SetRotation(MathUtils::LookRotationSmooth(target_dir, Vector3::Up,
                                                                     m_rotation_transform_->LocalRotation(),
                                                                     kRotationSpeed, engine::Time::GetDeltaTime()));

    const auto current_dir = m_rotation_transform_->Forward();

    const auto dot = target_dir.Dot(current_dir);
    const auto angle_normalized = (dot + 1.0f) * 0.5f;

    m_animator_->blend_weight = angle_normalized;
    m_animator_->GetState(m_anim_name_)->speed = m_player_data_->speed;
}

SlimeBoom::DashState::DashState(std::string animation_name, const std::shared_ptr<PlayerData>& player_data,
                                const std::shared_ptr<Animator>& animator, const std::shared_ptr<engine::Transform>& camera_transform,
                                const std::shared_ptr<engine::Transform>& rotation_transform)
    : m_anim_name_(std::move(animation_name)),
      m_player_data_(player_data),
      m_animator_(animator),
      m_camera_transform_(camera_transform),
      m_rotation_transform_(rotation_transform)
{
}