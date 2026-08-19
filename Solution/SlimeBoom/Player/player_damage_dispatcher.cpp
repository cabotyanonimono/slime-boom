#include "pch.h"
#include "player_damage_dispatcher.h"

#include "engine_time.h"

void SlimeBoom::PlayerDamageDispatcher::OnInspectorGui()
{
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
    engine::Gui::PropertyField("Receive Damage Multiplier", m_receive_damage_multiplier_);
}

void SlimeBoom::PlayerDamageDispatcher::OnUpdate()
{
    const auto damage = *static_cast<int*>(m_compute_result_->GetValue("taken_damage"));
    const auto delta_damage = static_cast<float>(damage - m_current_damage_) * engine::Time::GetDeltaTime() * m_receive_damage_multiplier_;
    m_current_damage_ = damage;

    if (engine::Time::Get()->TimeScale() <= 0.0f)
        return;
    
    m_on_take_damage_.Invoke(delta_damage);
}

size_t SlimeBoom::PlayerDamageDispatcher::AddOnTakeDamageListener(const std::function<void(float)>& callback)
{
    return m_on_take_damage_.AddListener(callback);
}

void SlimeBoom::PlayerDamageDispatcher::RemoveOnTakeDamageListener(const size_t token)
{
    m_on_take_damage_.RemoveListener(token);
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerDamageDispatcher)
