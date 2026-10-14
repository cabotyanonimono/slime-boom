#include "pch.h"
#include "cooldown_ability.h"

#include "cancellation_token_source.h"
#include "engine.h"

engine::Task CooldownAbility::UpdateAttack()
{
    for (int i = 0; i < m_player_data_->GetAttackNum(); ++i)
    {
        auto dt = engine::Time::GetDeltaTime();
        Execute(dt);
        co_await engine::WaitForSeconds(0.2f);
    }
}

CooldownAbility::CooldownAbility(const float cooldown, const std::shared_ptr<SlimeBoom::PlayerDataComponent> &player_data, const float cooldown_timer)
    :m_cooldown_(cooldown), m_cooldown_timer_(cooldown_timer), m_player_data_(player_data)
{
    m_update_attack_cancellation_token_ = std::make_shared<CancellationTokenSource>();
}

void CooldownAbility::Update(const float delta_time)
{
    m_cooldown_timer_ += delta_time;
    if (m_cooldown_timer_ >= m_cooldown_)
    {
        engine::Engine::coroutine.Start(UpdateAttack(), m_update_attack_cancellation_token_->GetToken());
        m_cooldown_timer_ -= m_cooldown_;
    }
}

void CooldownAbility::OnDetach()
{
    m_update_attack_cancellation_token_->Cancel();
}
