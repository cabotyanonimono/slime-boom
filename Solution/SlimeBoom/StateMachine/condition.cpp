#include "pch.h"
#include "condition.h"

#include "state_machine.h"

bool spring_bloom::Condition::Evaluate() const
{
    return m_is_not_equal_ ? !m_state_machine_.lock()->GetParameter<bool>(m_param_name_) : m_state_machine_.lock()->GetParameter<bool>(m_param_name_);
}

spring_bloom::Condition::Condition(const std::string &param_name, bool is_not_equal)
{
    m_param_name_ = param_name;
    m_is_not_equal_ = is_not_equal;
}