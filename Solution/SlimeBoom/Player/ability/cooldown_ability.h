#pragma once
#include "iability.h"

class CooldownAbility : public IAbility
{
protected:
    float m_cooldown_;
    float m_cooldown_timer_;
    
    virtual void UpdateCooldown(float delta_time) = 0;

public:
    CooldownAbility(float cooldown, float cooldown_timer = 0.0f);
    
    void Update(float delta_time) override;
};
