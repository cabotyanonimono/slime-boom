#include "pch.h"
#include "player_data_component.h"

#include "gui.h"

namespace SlimeBoom
{
void PlayerDataComponent::OnInspectorGui()
{
    if (engine::Gui::PropertyField("MaxHp", m_player_data_.max_hp))
        SetMaxHp(m_player_data_.max_hp);
    
    if (engine::Gui::PropertyField("Hp", m_player_data_.hp))
        SetHp(m_player_data_.hp);

    if (engine::Gui::PropertyField("Speed", m_player_data_.speed))
        SetSpeed(m_player_data_.speed);

    if (engine::Gui::PropertyField("Attack Power", m_player_data_.attack_power))
        SetAttackPower(m_player_data_.attack_power);

    if (engine::Gui::PropertyField("Attack Speed", m_player_data_.attack_speed))
        SetAttackSpeed(m_player_data_.attack_speed);

    if (engine::Gui::PropertyField("Avoid Time", m_player_data_.avoid_time))
        SetAvoidTime(m_player_data_.avoid_time);

    if (engine::Gui::PropertyField("Avoid Speed", m_player_data_.avoid_speed))
        SetAvoidSpeed(m_player_data_.avoid_speed);

    if (engine::Gui::PropertyField("Level", m_player_data_.level))
        SetLevel(m_player_data_.level);

    if (engine::Gui::PropertyField("Exp Multiplier", m_player_data_.exp_multiplier))
        SetExpMultiplier(m_player_data_.exp_multiplier);
}

void PlayerDataComponent::SetMaxHp(const float max_hp)
{
    m_player_data_.max_hp = max_hp;
}

void PlayerDataComponent::SetHp(const float hp)
{
    m_player_data_.hp = hp;
}

void PlayerDataComponent::SetSpeed(const float speed)
{
    m_player_data_.speed = speed;
}

void PlayerDataComponent::SetAttackPower(const float attack_power)
{
    m_player_data_.attack_power = attack_power;
}

void PlayerDataComponent::SetAttackSpeed(const float attack_speed)
{
    m_player_data_.attack_speed = attack_speed;
}

void PlayerDataComponent::SetAvoidTime(const float avoid_time)
{
    m_player_data_.avoid_time = avoid_time;
}

void PlayerDataComponent::SetAvoidSpeed(const float avoid_speed)
{
    m_player_data_.avoid_speed = avoid_speed;
}

void PlayerDataComponent::SetExpMultiplier(const float exp_multiplier)
{
    m_player_data_.exp_multiplier = exp_multiplier;
}

void PlayerDataComponent::SetLevel(const uint32_t level)
{
    m_player_data_.level = level;
}

float PlayerDataComponent::GetMaxHp() const
{
    return m_player_data_.max_hp;
}

float PlayerDataComponent::GetHp() const
{
    return m_player_data_.hp;
}

float PlayerDataComponent::GetSpeed() const
{
    return m_player_data_.speed;
}

float PlayerDataComponent::GetAttackPower() const
{
    return m_player_data_.attack_power;
}

float PlayerDataComponent::GetAttackSpeed() const
{
    return m_player_data_.attack_speed;
}

float PlayerDataComponent::GetAvoidTime() const
{
    return m_player_data_.avoid_time;
}

float PlayerDataComponent::GetAvoidSpeed() const
{
    return m_player_data_.avoid_speed;
}

float PlayerDataComponent::GetExpMultiplier() const
{
    return m_player_data_.exp_multiplier;
}

uint32_t PlayerDataComponent::GetLevel() const
{
    return m_player_data_.level;
}

const PlayerData& PlayerDataComponent::GetPlayerData() const
{
    return m_player_data_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerDataComponent)