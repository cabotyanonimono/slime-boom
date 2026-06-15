#include "pch.h"
#include "state.h"

namespace spring_bloom
{
void State::AddTransition(std::shared_ptr<Condition> condition, std::shared_ptr<State> state)
{
    m_states_.emplace(condition, state);
}

void State::OnEnter()
{}
void State::FixedUpdate()
{}
void State::OnExit()
{}
}