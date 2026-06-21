#include "pch.h"
#include "slash_ability.h"
#include "ability_data.h"

namespace SlimeBoom
{
void SlashAbility::UpdateCooldown(const float delta_time)
{
    m_closest_slime_pos_ = *static_cast<Vector3*>(m_compute_result_->GetValue("closest_slime_pos"));
    engine::Logger::Log("X : %f, Y : %f, Z : %f", m_closest_slime_pos_.x, m_closest_slime_pos_.y, m_closest_slime_pos_.z);
    
    PlayerAttackData attack_data;
    const auto slash_data = AbilityData::slash_ability_data;
    m_cooldown_ = slash_data.cooldown / m_player_data_->attack_speed;
    
    attack_data.arc = slash_data.arc;
    attack_data.angle = m_closest_slime_pos_ - m_player_transform_->Position();
    attack_data.size = slash_data.size;
    attack_data.damage = m_player_data_->attack_power * slash_data.damage_multiplier;
    attack_data.power = m_player_data_->attack_power * slash_data.power_multiplier;
    m_player_data_presenter_->SetAttackData(attack_data);
    m_sword_controller_->Play(m_closest_slime_pos_, attack_data.size * 0.5f);
}

SlashAbility::SlashAbility(const std::shared_ptr<ComputeResult>& compute_result, const std::shared_ptr<PlayerData>& player_data,
                           const std::shared_ptr<engine::Transform>& player_transform,
                           const std::shared_ptr<PlayerDataPresenter>& player_data_presenter,
                           const std::shared_ptr<PlayerSwordController>& swc_controller)
    : CooldownAbility(AbilityData::slash_ability_data.cooldown / player_data->attack_speed), m_compute_result_(compute_result), m_player_data_(player_data),
      m_player_transform_(player_transform), m_player_data_presenter_(player_data_presenter),
      m_sword_controller_(swc_controller)
{
}
}
