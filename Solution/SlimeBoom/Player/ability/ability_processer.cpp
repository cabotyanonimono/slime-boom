#include "pch.h"
#include "ability_processer.h"

void AbilityProcesser::Attach(const std::shared_ptr<IAbility>& ability)
{
    ability->OnAttach();
    m_abilities_.emplace(ability);
}

void AbilityProcesser::Detach(const std::shared_ptr<IAbility>& ability)
{
    ability->OnDetach();
    m_abilities_.erase(ability);
}

void AbilityProcesser::Update(const float delta_time) const
{
    for (auto &ability : m_abilities_)
    {
        ability->Update(delta_time);
    }
}

void AbilityProcesser::FixedUpdate() const
{
    for (auto &ability : m_abilities_)
    {
        ability->FixedUpdate();
    }
}

std::unordered_set<std::shared_ptr<IAbility>> AbilityProcesser::GetAbilities()
{
    return m_abilities_;
}
