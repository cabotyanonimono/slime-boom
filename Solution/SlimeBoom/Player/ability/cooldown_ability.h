#pragma once
#include "iability.h"
#include "../player_data_component.h"
#include "Coroutine/task.h"

class CooldownAbility : public IAbility
{
protected:
    float m_cooldown_;
    float m_cooldown_timer_;
    std::shared_ptr<SlimeBoom::PlayerDataComponent> m_player_data_;

    engine::Task UpdateAttack();
    virtual void Execute(float delta_time) = 0;

public:
    CooldownAbility(float cooldown, const std::shared_ptr<SlimeBoom::PlayerDataComponent> &player_data, float cooldown_timer = 0.0f);
    
    void Update(float delta_time) override;
};
