#pragma once
#include "parameter.h"
#include "state.h"

class StateMachine : public enable_shared_from_base<StateMachine>
{
    std::unordered_map<std::string, Parameter> m_parameters_;
    std::vector<std::shared_ptr<State>> m_states_;
    std::shared_ptr<State> m_default_state_;
    std::shared_ptr<State> m_current_state_;
    
public:
    
    void ResetState();
    void SetCurrentState(const std::shared_ptr<State> &state);
    void SetDefaultState(const std::shared_ptr<State> &state);
    void CreateTransition(const std::shared_ptr<State> &base_state, const std::shared_ptr<ConditionBase> &condition, const std::shared_ptr<State>& next_state);
    void CreateTransition(const std::shared_ptr<State> &base_state, const Transition &transition, std::shared_ptr<State> next_state);
    void Update();
    void FixedUpdate() const;

    template<typename T>
    void SetParameter(const std::string &name, T value);
    template<typename T>
    T GetParameter(const std::string &name);
};

template <typename T>
void StateMachine::SetParameter(const std::string& name, T value)
{

    auto it = m_parameters_.find(name);
    if (it != m_parameters_.end()) {
        it->second.value = value;
    }
    
    Parameter param;

    if constexpr (std::is_same_v<T, int>)
    {
        param.SetInt(value);
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        param.SetFloat(value);
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        param.SetBool(value);
    }
    else if constexpr (std::is_same_v<T, Vector3>)
    {
        
    }
    else
    {
        static_assert(!std::is_same_v<T, T>, "Unsupported parameter type.");
    }

    m_parameters_.emplace(name, std::move(param));
}

template <typename T>
T StateMachine::GetParameter(const std::string &name)
{
    auto it = m_parameters_.find(name);
    if (it == m_parameters_.end()) {
        throw std::runtime_error("Parameter not found: " + name);
    }
    
    if constexpr (std::is_same_v<T, int>) {
        return it->second.GetInt();
    }
    else if constexpr (std::is_same_v<T, float>) {
        return it->second.GetFloat();
    }
    else if constexpr (std::is_same_v<T, bool>) {
        return it->second.GetBool();
    }
    else if constexpr (std::is_same_v<T, Vector3>) {
        return it->second.GetVector3();
    }
    else {
        throw std::runtime_error("Unsupported parameter type");
    }
}