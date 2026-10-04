#include "pch.h"
#include "condition_base.h"

void ConditionBase::SetStateMachine(const std::shared_ptr<StateMachine>& state_machine)
{
    m_state_machine_ = state_machine;
}
