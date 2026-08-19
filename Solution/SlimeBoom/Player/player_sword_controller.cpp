#include "pch.h"
#include "player_sword_controller.h"

#include "engine.h"
#include "engine_time.h"
#include "gui.h"
#include "Rendering/render_pipeline.h"

namespace SlimeBoom
{
void PlayerSwordController::OnInspectorGui()
{
    engine::Gui::PropertyField("Sword Transform", m_sword_transform_);
    engine::Gui::PropertyField("Sword Parent", m_sword_parent_transform_);
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
    engine::Gui::PropertyField("Angle", m_angle_);
    engine::Gui::PropertyField("Offset", m_offset_);
    engine::Gui::PropertyField("Duration", m_duration_);
}

void PlayerSwordController::OnStart()
{
}

void PlayerSwordController::OnUpdate()
{
    m_current_time_ += engine::Time::GetDeltaTime();
    
    const float progress = m_current_time_ / m_duration_;

    const Quaternion current_angle = Quaternion::Slerp(m_start_angle_, m_end_angle_, progress);
    m_sword_parent_transform_->SetRotation(current_angle);

    if (m_current_time_ > m_duration_)
    {
        m_is_playing_ = false;
        m_current_time_ = 0.0f;
        GameObject()->SetActive(false);
    }
}

void PlayerSwordController::Play(const Vector3 attack_pos, const float offset)
{
    const auto forward = attack_pos - m_player_transform_->Position();
    const float angle = std::atan2(forward.x, forward.z);

    const float half_sweep_rad = (m_angle_ / 2.0f) / 180.0f * DirectX::XM_PI;
    m_start_angle_ = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), angle - half_sweep_rad);;
    m_end_angle_ = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), angle + half_sweep_rad);

    m_offset_ = offset;
    m_current_time_ = 0.0f;
    
    m_sword_transform_->SetLocalPosition(Vector3(0,0, m_offset_));
    m_sword_parent_transform_->SetPosition(m_player_transform_->Position());

    //m_sword_transform_->GameObject()->SetActive(true);
    GameObject()->SetActive(true);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerSwordController)