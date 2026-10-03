#include "pch.h"
#include "camera_controller.h"

#include "application.h"
#include "engine_time.h"
#include "gui.h"
#include "input.h"

namespace SlimeBoom
{
void CameraController::OnInspectorGui()
{
    engine::Gui::PropertyField("Sensitivity", m_sensitivity);
    engine::Gui::PropertyField("Target", m_target_);
    engine::Gui::PropertyField("Camera Transform", m_camera_transform_);
    engine::Gui::PropertyField("Map Size", m_map_size_);
    engine::Gui::PropertyField("Camera Position Offset", m_camera_position_offset_);
}

void CameraController::OnUpdate()
{
    if (const auto target = m_target_.CastedLock(); target == nullptr)
        return;

    auto mouse_delta = engine::Input::MouseDelta();
    mouse_delta.x /= engine::Application::WindowWidth();
    mouse_delta.y /= engine::Application::WindowHeight();

    mouse_delta *= m_sensitivity;
    m_yaw_ -= mouse_delta.x * engine::Time::GetDeltaTime();
    m_pitch_ -= mouse_delta.y * engine::Time::GetDeltaTime();
    
    m_pitch_ = std::clamp(m_pitch_, -DirectX::XM_PIDIV2, DirectX::XM_PIDIV2);

    auto position_rotation = Quaternion::CreateFromYawPitchRoll(m_yaw_, m_pitch_, 0.0f);
    GameObject()->Transform()->SetLocalRotation(position_rotation);

    m_camera_transform_->SetLocalPosition(m_camera_position_offset_);
    
    auto min = m_map_size_->GetMin();
    auto max = m_map_size_->GetMax();
    auto camera_pos = m_camera_transform_->Position();

    camera_pos.x = std::clamp(camera_pos.x, min.x, max.x);
    camera_pos.y = std::clamp(camera_pos.y, min.y, max.y);
    camera_pos.z = std::clamp(camera_pos.z, min.z, max.z);
    m_camera_transform_->SetPosition(camera_pos);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::CameraController)