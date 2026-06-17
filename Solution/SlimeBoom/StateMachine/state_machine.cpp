#include "pch.h"
#include "state_machine.h"

void StateMachine::ResetState()
{
    SetCurrentState(m_default_state_);
}

void StateMachine::SetCurrentState(const std::shared_ptr<State>& state)
{
    if (m_current_state_ != nullptr)
    {
        m_current_state_->OnExit();
    }
    m_current_state_ = state;
    m_current_state_->OnEnter();
}

void StateMachine::SetDefaultState(const std::shared_ptr<State>& state)
{
    state->m_state_machine_ = shared_from_this();
    m_default_state_ = state;
    m_current_state_ = state;
    m_states_.emplace_back(state);

    state->OnEnter();
}

void StateMachine::CreateTransition(const std::shared_ptr<State>& base_state,
    const std::shared_ptr<ConditionBase>& condition, const std::shared_ptr<State>& next_state)
{
    Transition transition;
    transition.conditions.emplace_back(condition);

    CreateTransition(base_state, transition, next_state);
}

void StateMachine::CreateTransition(const std::shared_ptr<State>& base_state, const Transition& transition,
                                    std::shared_ptr<State> next_state)
{
    if (auto find = std::ranges::find_if(m_states_, [base_state](std::shared_ptr<State> a) { return a == base_state; });
        find == m_states_.end())
    {
        engine::Logger::Log<StateMachine>("failed to add transition");
        return;
    }

    for (const auto& condition : transition.conditions)
    {
        condition->m_state_machine_ = shared_from_this();
    }
    next_state->m_state_machine_ = shared_from_this();
    m_states_.emplace_back(next_state);
    base_state->AddTransition(std::move(transition), next_state);
}

void StateMachine::Update()
{
    if (m_current_state_ == nullptr)
        return;

    for (auto& [state, transition] : m_current_state_->m_transitions_)
    {
        if (transition.Evaluate())
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
