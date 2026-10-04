#include "pch.h"
#include "transition.h"

void Transition::SetStateMachine(const std::shared_ptr<StateMachine>& state_machine) const
{
    for (const auto& condition : m_conditions_)
    {
        condition->SetStateMachine(state_machine);
    }
}

void Transition::AddCondition(std::shared_ptr<ConditionBase> condition)
{
    m_conditions_.emplace(condition);
}

void Transition::RemoveCondition(const std::shared_ptr<ConditionBase>& condition)
{
    m_conditions_.erase(condition);
}

bool Transition::Evaluate() const
{
    for (const auto &condition : m_conditions_)
        if (!condition->Evaluate())
            return false;

    return true;
}
