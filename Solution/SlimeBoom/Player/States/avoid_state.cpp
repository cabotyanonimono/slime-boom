#include "pch.h"
#include "avoid_state.h"

#include "engine_time.h"
#include "event.h"
#include "input.h"
#include "../../Sound/sound_manager_component.h"
#include "../../Utils/math_util.h"

void SlimeBoom::AvoidState::OnEnter()
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

    m_on_avoid_->Invoke();
    SoundManagerComponent::Play(kSoundTypes::kPlayerAvoid);

    auto current_anim_name = m_animator_->GetCurrentAnimName();
    if (current_anim_name != "")
    {
        m_animator_->BlendAnim(current_anim_name, m_anim_name_, 8.0f);
        return;
    }

    m_animator_->PlayAnim(m_anim_name_);
}

void SlimeBoom::AvoidState::Update()
{
    if (m_move_direction_.Length() == 0)
    {
        m_move_direction_ = -Vector3::Forward;
    }

    m_move_direction_.Normalize();
    auto target_dir = Vector3::Transform(m_move_direction_, m_camera_transform_->Rotation());
    target_dir.y = 0;
    target_dir.Normalize();

    float angle_radians = DirectX::XMConvertToRadians(kAnimationRotationOffset);
    Quaternion rotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle_radians);
    Vector3 converted_target_dir = Vector3::Transform(target_dir, rotation);
    m_rotation_transform_->SetRotation(MathUtils::LookRotationSmooth(converted_target_dir, Vector3::Up,
                                                                     m_rotation_transform_->LocalRotation(),
                                                                     kRotationSpeed, engine::Time::GetDeltaTime()));
    auto position = m_player_transform_->Position();
    auto velocity = (-target_dir * m_player_data_->GetAvoidSpeed()) * engine::Time::GetDeltaTime();
    m_player_transform_->SetPosition(position + velocity);
}

SlimeBoom::AvoidState::AvoidState(std::string animation_name, const std::shared_ptr<PlayerDataComponent>& player_data,
                                  const std::shared_ptr<Animator>& animator,
                                  const std::shared_ptr<engine::Transform>& camera_transform,
                                  const std::shared_ptr<engine::Transform>& rotation_transform,
                                  const std::shared_ptr<engine::Transform>& player_transform, engine::Event<>* on_avoid)
    : m_anim_name_(std::move(animation_name)),
      m_animator_(animator),
      m_player_data_(player_data),
      m_camera_transform_(camera_transform),
      m_rotation_transform_(rotation_transform),
      m_player_transform_(player_transform), m_animation_name_(std::move(animation_name)),
      m_on_avoid_(on_avoid)
{
}
