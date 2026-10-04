#pragma once

class StateMachine;

class ConditionBase
{
protected:
    std::weak_ptr<StateMachine> m_state_machine_;

public:
    virtual ~ConditionBase() = default;
    virtual bool Evaluate() const = 0;

    void SetStateMachine(const std::shared_ptr<StateMachine>& state_machine);
};
