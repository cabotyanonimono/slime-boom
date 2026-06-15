#pragma once

namespace spring_bloom
{
class Condition;
class State
{
    friend class StateMachine;
    std::map<std::shared_ptr<Condition>,std::shared_ptr<State>> m_states_;

    void AddTransition(std::shared_ptr<Condition> condition, std::shared_ptr<State> state);
    virtual void OnEnter();
    virtual void Update() = 0;
    virtual void FixedUpdate();
    virtual void OnExit();
protected:
    std::weak_ptr<StateMachine> m_state_machine_;
    
public:
    virtual ~State() = default;
};
}