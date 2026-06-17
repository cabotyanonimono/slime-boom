#pragma once
#include "transition.h"

class State
{
    friend class StateMachine;
    std::unordered_map<std::shared_ptr<State>, Transition> m_transitions_;

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