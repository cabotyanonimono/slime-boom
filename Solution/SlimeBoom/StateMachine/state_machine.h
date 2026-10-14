#pragma once
#include "parameter.h"
#include "state.h"

// 状態(State)と遷移条件を管理し、条件を満たしたら状態を切り替えるステートマシン。
class StateMachine : public enable_shared_from_base<StateMachine>
{
    std::unordered_map<std::string, Parameter> m_parameters_;
    std::vector<std::shared_ptr<State>> m_states_;
    std::shared_ptr<State> m_default_state_;
    std::shared_ptr<State> m_current_state_;
    
public:
    
    /// 現在の状態を、初期状態に戻す。
    void ResetState();
    /// 現在の状態を切り替える。現在の状態のOnExit、新しい状態のOnEnterの順に呼ぶ。
    void SetCurrentState(const std::shared_ptr<State> &state);
    /// 初期状態を設定する。現在の状態にもなり、OnEnterが呼ばれる。
    void SetDefaultState(const std::shared_ptr<State> &state);
    /// 1つの条件で、base_stateからnext_stateへの遷移を作る。base_stateが登録されていない場合は失敗する。
    void CreateTransition(const std::shared_ptr<State> &base_state, const std::shared_ptr<ConditionBase> &condition, const std::shared_ptr<State>& next_state);
    /// 複数の条件(Transition)で、base_stateからnext_stateへの遷移を作る。base_stateが登録されていない場合は失敗する。
    void CreateTransition(const std::shared_ptr<State> &base_state, const Transition &transition, std::shared_ptr<State> next_state);
    /// 現在の状態からの遷移条件を評価して、満たしていれば状態を切り替える。その後、現在の状態のUpdateを呼ぶ。
    void Update();
    /// 現在の状態のFixedUpdateを呼ぶ。
    void FixedUpdate() const;

    /// パラメーターを設定する。対応している型はint/float/bool/Vector3。
    template<typename T>
    void SetParameter(const std::string &name, T value);
    /// パラメーターを取得する。パラメーターが存在しない場合はstd::runtime_errorを投げる。
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