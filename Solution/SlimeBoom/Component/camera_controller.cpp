#include "pch.h"
#include "camera_controller.h"

#include "application.h"
#include "gui.h"
#include "input.h"

namespace SlimeBoom
{
void CameraController::OnInspectorGui()
{
    engine::Gui::PropertyField("Sensitivity", m_sensitivity);
    engine::Gui::PropertyField("Target", m_target_);
}

void CameraController::OnUpdate()
{
    
    /*auto mouse_mode = engine::Input::MouseMode();
    if (engine::Input::GetKeyDown(DirectX::Keyboard::Space))
    {
        mouse_mode = mouse_mode == engine::kMouseMode::kNormal ? engine::kMouseMode::kLocked : engine::kMouseMode::kNormal;
        engine::Input::SetMouseMode(mouse_mode);
        engine::Input::SetCursorVisible(true);
    }

    if (mouse_mode == engine::kMouseMode::kNormal)
        return;*/
    
    const auto target = m_target_.CastedLock();
    if (target == nullptr)
        return;

    auto mouse_delta = engine::Input::MouseDelta();
    mouse_delta.x /= engine::Application::WindowWidth();
    mouse_delta.y /= engine::Application::WindowHeight();

    mouse_delta *= m_sensitivity;
    m_yaw_ -= mouse_delta.x;
    m_pitch_ -= mouse_delta.y;
    
    m_pitch_ = std::clamp(m_pitch_, -DirectX::XM_PIDIV2, DirectX::XM_PIDIV2);

    auto position_rotation = Quaternion::CreateFromYawPitchRoll(m_yaw_, m_pitch_, 0.0f);
    
    GameObject()->Transform()->SetLocalRotation(position_rotation);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::CameraController)