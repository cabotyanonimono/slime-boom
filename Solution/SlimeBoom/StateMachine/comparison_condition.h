#pragma once
#include "condition_base.h"
#include "state_machine.h"

enum class kOpType
{
    kGreaterThan,
    kLessThan,
    kGreaterEqual,
    kLessEqual,
    kEqual,
    kNotEqual,
};

template <typename T>
class ComparisonCondition : public ConditionBase
{
    kOpType m_op_type_;
    bool m_is_comparision_other_params_;
    T m_value_;
    std::string m_param_name_;
    std::string m_other_param_name_;

    T GetComparisonValue() const;

public:
    explicit ComparisonCondition(const std::string &param_name, T value, kOpType op_type);
    explicit ComparisonCondition(const std::string &param_name, const std::string& other_param_name, kOpType op_type);

    bool Evaluate() const override;
};

template <typename T>
T ComparisonCondition<T>::GetComparisonValue() const
{
    return m_is_comparision_other_params_ ? m_state_machine_.lock()->GetParameter<T>(m_other_param_name_) : m_value_;
}

template <typename T>
ComparisonCondition<T>::ComparisonCondition(const std::string &param_name, T value, const kOpType op_type) :
    m_op_type_(op_type),
    m_is_comparision_other_params_(false),
    m_value_(value),
    m_param_name_(std::move(param_name))
{
}

template <typename T>
ComparisonCondition<T>::ComparisonCondition(const std::string &param_name, const std::string &other_param_name, const kOpType op_type) :
    m_op_type_(op_type),
    m_is_comparision_other_params_(true),
    m_param_name_(std::move(param_name)),
    m_other_param_name_(other_param_name)
{
}

template <typename T>
bool ComparisonCondition<T>::Evaluate() const
{
    switch (m_op_type_)
    {
    case kOpType::kGreaterThan:
        return m_state_machine_.lock()->GetParameter<T>(m_param_name_) > GetComparisonValue();
    case kOpType::kLessThan:
        return m_state_machine_.lock()->GetParameter<T>(m_param_name_) < GetComparisonValue();
    case kOpType::kGreaterEqual:
        return m_state_machine_.lock()->GetParameter<T>(m_param_name_) >= GetComparisonValue();
    case kOpType::kLessEqual:
        return m_state_machine_.lock()->GetParameter<T>(m_param_name_) <= GetComparisonValue();
    case kOpType::kEqual:
        return m_state_machine_.lock()->GetParameter<T>(m_param_name_) == GetComparisonValue();
    case kOpType::kNotEqual:
        return m_state_machine_.lock()->GetParameter<T>(m_param_name_) != GetComparisonValue();
    default:
        return false;
    }
}
