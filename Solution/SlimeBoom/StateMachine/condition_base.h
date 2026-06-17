#pragma once

class ConditionBase
{
    friend class StateMachine;

protected:
    std::weak_ptr<StateMachine> m_state_machine_;

public:
    virtual ~ConditionBase() = default;
    virtual bool Evaluate() const = 0;
};
