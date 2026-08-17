#include "pch.h"
#include "slime_take_damage_event_component.h"

void SlimeBoom::SlimeTakeDamageEventComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
}

void SlimeBoom::SlimeTakeDamageEventComponent::OnUpdate()
{
    const auto deal_damage_count = *static_cast<int*>(m_compute_result_->GetValue("deal_damage_count"));
    const auto delta_deal_damage_count = deal_damage_count - m_before_deal_damage_count_;
    m_before_deal_damage_count_ = deal_damage_count;

    if (delta_deal_damage_count > 0)
        m_on_damage_event_.Invoke(delta_deal_damage_count);
}

size_t SlimeBoom::SlimeTakeDamageEventComponent::AddOnDamageEventListener(const std::function<void(int)>& callback)
{
    return m_on_damage_event_.AddListener(callback);
}

void SlimeBoom::SlimeTakeDamageEventComponent::RemoveOnDamageEventListener(const size_t token)
{
    m_on_damage_event_.RemoveListener(token);
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeTakeDamageEventComponent)
