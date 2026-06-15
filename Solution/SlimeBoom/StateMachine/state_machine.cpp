#include "pch.h"
#include "state_machine.h"

namespace spring_bloom
{
void StateMachine::ResetState()
{
    SetCurrentState(m_default_state_);
}

void StateMachine::SetCurrentState(const std::shared_ptr<State> &state)
{
    if (m_current_state_ != nullptr)
    {
        m_current_state_->OnExit();
    }
    m_current_state_ = state;
     m_current_state_->OnEnter();
}

void StateMachine::SetDefaultState(const std::shared_ptr<State> &state)
{
    state->m_state_machine_ = shared_from_this();
    m_default_state_ = state;
    m_current_state_ = state;
    m_states_.emplace_back(state);
    
    state->OnEnter();
}

void StateMachine::CreateTransition(const std::shared_ptr<State> &base_state, const std::shared_ptr<Condition> &condition, std::shared_ptr<State> next_state)
{
    auto find = std::ranges::find_if(m_states_, [base_state](std::shared_ptr<State> a){return a == base_state;});
    if (find == m_states_.end())
    {
        engine::Logger::Log<StateMachine>("failed to add transition");
        return;
    }

    condition->m_state_machine_ = shared_from_this();
    next_state->m_state_machine_ = shared_from_this();
    m_states_.emplace_back(next_state);
    base_state->AddTransition(condition, next_state);
}

void StateMachine::Update()
{
    if (m_current_state_ == nullptr)
        return;
    
    for (auto& [condition, state] : m_current_state_->m_states_)
    {
        if (condition->Evaluate())
        {
            SetCurrentState(state);
            break;
        }
    }
    
    m_current_state_->Update();
}
void StateMachine::FixedUpdate() const
{
    if (m_current_state_ == nullptr)
        return;
    m_current_state_->FixedUpdate();
}
}