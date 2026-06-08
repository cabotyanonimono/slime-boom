#include "pch.h"
#include "player_controller.h"

#include "engine_time.h"
#include "input.h"
#include "../Utils/math_util.h"

namespace SlimeBoom
{
void PlayerController::OnInspectorGui()
{
    engine::Gui::PropertyField("Rotation Speed", m_rotation_speed_);
    engine::Gui::PropertyField("Camera", m_camera_);
    engine::Gui::PropertyField("Player", m_player_);
    engine::Gui::PropertyField("Rigidbody", m_rigidbody_);
    engine::Gui::PropertyField("Rotation", m_rotation_object_);
}

void PlayerController::OnUpdate()
{
    Vector3 move_dir = Vector3::Zero;

    if (engine::Input::GetKey(DirectX::Keyboard::A))
        move_dir.x -= 1;
    if (engine::Input::GetKey(DirectX::Keyboard::D))
        move_dir.x += 1;
    if (engine::Input::GetKey(DirectX::Keyboard::W))
        move_dir.z -= 1;
    if (engine::Input::GetKey(DirectX::Keyboard::S))
        move_dir.z += 1;

    move_dir = Vector3::Transform(move_dir, m_camera_->GameObject()->Transform()->Rotation());
    move_dir.y = 0;
    move_dir.Normalize();
    
    m_rotation_object_->Transform()->SetRotation(MathUtils::LookRotationSmooth(move_dir, Vector3::Up, m_rotation_object_->Transform()->LocalRotation(), m_rotation_speed_, engine::Time::GetDeltaTime()));

    const auto transform = GameObject()->Transform();

    const auto velocity = move_dir * m_player_->speed * engine::Time::GetDeltaTime();
    m_rigidbody_->AddForce(velocity);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerController)