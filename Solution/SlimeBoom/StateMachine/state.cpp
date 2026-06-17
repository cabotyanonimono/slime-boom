#include "pch.h"
#include "state.h"

void State::AddTransition(const Transition& transition, const std::shared_ptr<State>& state)
{
    m_transitions_.try_emplace(state, std::move(transition));
}

void State::OnEnter()
{}
void State::FixedUpdate()
{}
void State::OnExit()
{}