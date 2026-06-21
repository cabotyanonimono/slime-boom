#pragma once
#include "iability.h"

class AbilityProcesser
{
    std::unordered_set<std::shared_ptr<IAbility>> m_abilities_;
    
public:
    void Attach(const std::shared_ptr<IAbility>& ability);
    void Detach(const std::shared_ptr<IAbility>& ability);
    void Update(float delta_time) const;
    void FixedUpdate() const;

    [[nodiscard]] std::unordered_set<std::shared_ptr<IAbility>> GetAbilities();
};
