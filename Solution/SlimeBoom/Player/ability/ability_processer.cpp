#include "pch.h"
#include "ability_processer.h"

#include "engine_time.h"

namespace SlimeBoom
{
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

void AbilityProcesser::OnUpdate()
{
    for (auto& ability : m_abilities_)
    {
        ability->Update(engine::Time::GetDeltaTime());
    }
}

void AbilityProcesser::OnFixedUpdate()
{
    for (auto& ability : m_abilities_)
    {
        ability->FixedUpdate();
    }
}

std::unordered_set<std::shared_ptr<IAbility>> AbilityProcesser::GetAbilities()
{
    return m_abilities_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AbilityProcesser)
