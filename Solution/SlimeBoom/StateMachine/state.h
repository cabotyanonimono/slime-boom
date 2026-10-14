#pragma once
#include "transition.h"

// ステートマシンの状態の基底クラス。
class State
{
    friend class StateMachine;
    std::unordered_map<std::shared_ptr<State>, Transition> m_transitions_;

    /// 遷移先の状態と、その遷移条件を追加する。
    void AddTransition(const Transition &transition, const std::shared_ptr<State>& state);
    virtual void OnEnter();
    virtual void Update() = 0;
    virtual void FixedUpdate();
    virtual void OnExit();
    
protected:
    std::weak_ptr<StateMachine> m_state_machine_;
    
public:
    virtual ~State() = default;
};