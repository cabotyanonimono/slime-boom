#include "pch.h"
#include "cooldown_ability.h"

CooldownAbility::CooldownAbility(const float cooldown, const float cooldown_timer)
    :m_cooldown_(cooldown), m_cooldown_timer_(cooldown_timer)
{
}

void CooldownAbility::Update(const float delta_time)
{
    m_cooldown_timer_ += delta_time;
    if (m_cooldown_timer_ >= m_cooldown_)
    {
        UpdateCooldown(delta_time);
        m_cooldown_timer_ -= m_cooldown_;
    }
}