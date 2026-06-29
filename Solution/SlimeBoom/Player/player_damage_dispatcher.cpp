#include "pch.h"
#include "player_damage_dispatcher.h"

void SlimeBoom::PlayerDamageDispatcher::OnInspectorGui()
{
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
}

void SlimeBoom::PlayerDamageDispatcher::OnUpdate()
{
    const auto damage = *static_cast<int*>(m_compute_result_->GetValue("taken_damage"));
    const auto delta_damage = damage - m_current_damage_;
    m_current_damage_ = damage;

    m_on_take_damage_.Invoke(delta_damage);
}

size_t SlimeBoom::PlayerDamageDispatcher::AddOnTakeDamageListener(const std::function<void(int)>& callback)
{
    return m_on_take_damage_.AddListener(callback);
}

void SlimeBoom::PlayerDamageDispatcher::RemoveOnTakeDamageListener(const size_t token)
{
    m_on_take_damage_.RemoveListener(token);
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerDamageDispatcher)
