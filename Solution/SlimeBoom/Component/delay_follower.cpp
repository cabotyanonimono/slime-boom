#include "pch.h"
#include "delay_follower.h"

#include "gui.h"

namespace SlimeBoom
{
void DelayFollower::OnInspectorGui()
{
    engine::Gui::PropertyField("BaseSpeed", m_base_speed_);
    engine::Gui::PropertyField("SpeedUpDistance", m_max_speed_distance_);
    engine::Gui::PropertyField("Target", m_target_);
}
void DelayFollower::OnUpdate()
{
    auto target = m_target_.CastedLock();
    if (target == nullptr)
        return
    ;
    auto this_transform = GameObject()->Transform();
    auto this_pos = this_transform->Position();
    auto target_pos = target->Position();

    Vector3 direction;
    auto to_target = target_pos - this_pos;
    to_target.Normalize(direction);
    
    auto distance = to_target.Length();

    if (distance <= 0.01f)
    {
        this_transform->SetPosition(target_pos);
        return;
    }

    float speed_multiply = distance / m_max_speed_distance_;
    speed_multiply = std::max(speed_multiply, 0.01f);

    auto move_power = direction * m_base_speed_ * speed_multiply;
    this_transform->SetPosition(this_pos + move_power);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::DelayFollower)