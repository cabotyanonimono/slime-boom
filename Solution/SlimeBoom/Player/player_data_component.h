#pragma once
#include "player_data.h"
#include "Components/component.h"

namespace SlimeBoom
{
class PlayerDataComponent : public engine::Component
{
    PlayerData m_player_data_;

public:
    void OnInspectorGui() override;

    void SetMaxHp(float max_hp);
    void SetHp(float hp);
    void SetSpeed(float speed);
    void SetAttackPower(float attack_power);
    void SetAttackSpeed(float attack_speed);
    void SetAvoidTime(float avoid_time);
    void SetAvoidSpeed(float avoid_speed);
    void SetExpMultiplier(float exp_multiplier);
    void SetLevel(uint32_t level);

    [[nodiscard]] float GetMaxHp() const;
    [[nodiscard]] float GetHp() const;
    [[nodiscard]] float GetSpeed() const;
    [[nodiscard]] float GetAttackPower() const;
    [[nodiscard]] float GetAttackSpeed() const;
    [[nodiscard]] float GetAvoidTime() const;
    [[nodiscard]] float GetAvoidSpeed() const;
    [[nodiscard]] float GetExpMultiplier() const;
    [[nodiscard]] uint32_t GetLevel() const;
    [[nodiscard]] const PlayerData &GetPlayerData() const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_player_data_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerDataComponent, 1)
