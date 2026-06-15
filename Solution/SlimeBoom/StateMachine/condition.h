#pragma once

namespace spring_bloom
{
class Condition
{
    friend class StateMachine;

    bool m_is_not_equal_;
    std::string m_param_name_;
    
    bool Evaluate() const;
    
protected:
    std::weak_ptr<StateMachine> m_state_machine_;

public:
    explicit Condition(const std::string &param_name, bool is_not_equal = false);
};
}