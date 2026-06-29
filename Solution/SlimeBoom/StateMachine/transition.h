#pragma once
#include "condition_base.h"

class Transition
{
    std::unordered_set<std::shared_ptr<ConditionBase>> m_conditions_;

public:
    void SetStateMachine(const std::shared_ptr<StateMachine>& state_machine) const;
    void AddCondition(std::shared_ptr<ConditionBase> condition);
    void RemoveCondition(const std::shared_ptr<ConditionBase>& condition);
    bool Evaluate() const;
};
