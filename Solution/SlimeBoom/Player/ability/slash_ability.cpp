#include "pch.h"
#include "slash_ability.h"

#include "engine.h"
#include "ability_data/ability_data_store.h"

namespace SlimeBoom
{
engine::Task SlashAbility::DelaySetAttackDataTask()
{
    PlayerAttackData *attack_data = new PlayerAttackData;
    const auto slash_data = AbilityDataStore::GetAbilityData(kAbilityTypesSlash);
    m_cooldown_ = slash_data.cooldown / m_player_data_->GetAttackSpeed();

    attack_data->arc = slash_data.arc;
    attack_data->angle = m_closest_slime_pos_ - m_player_transform_->Position();
    attack_data->size = slash_data.size;
    attack_data->damage = m_player_data_->GetAttackPower() + slash_data.value;
    attack_data->power = slash_data.knockback;
    
    co_await engine::WaitForSeconds(m_attack_delay_time_);

    if (this == nullptr)
        co_return;   
    
    m_player_data_presenter_->SetAttackData(*attack_data);
    delete attack_data;
}

void SlashAbility::Execute(const float delta_time)
{
    m_closest_slime_pos_ = *static_cast<Vector3*>(m_compute_result_->GetValue("closest_slime_pos"));
    engine::Logger::Log("X : %f, Y : %f, Z : %f", m_closest_slime_pos_.x, m_closest_slime_pos_.y,
                        m_closest_slime_pos_.z);
    
    const auto slash_data = AbilityDataStore::GetAbilityData(kAbilityTypesSlash);
    engine::Engine::coroutine.Start(DelaySetAttackDataTask());
    m_sword_controller_->Play(m_closest_slime_pos_, slash_data.size * 0.5f);

    m_effekseer_renderer_->Play();    
}

SlashAbility::SlashAbility(const std::shared_ptr<ComputeResult>& compute_result,
                           const std::shared_ptr<PlayerDataComponent>& player_data,
                           const std::shared_ptr<engine::Transform>& player_transform,
                           const std::shared_ptr<PlayerDataPresenter>& player_data_presenter,
                           const std::shared_ptr<PlayerSwordController>& swc_controller,
                           const std::shared_ptr<engine::EffekseerRenderer>& effekseer_renderer,
                           float attack_delay_time)
    : CooldownAbility(AbilityDataStore::GetAbilityData(kAbilityTypesSlash).cooldown / player_data->GetAttackSpeed(), player_data),
      m_compute_result_(compute_result), m_player_data_(player_data),
      m_player_transform_(player_transform), m_player_data_presenter_(player_data_presenter),
      m_sword_controller_(swc_controller),
      m_effekseer_renderer_(effekseer_renderer),
      m_attack_delay_time_(attack_delay_time)
{
}
}
