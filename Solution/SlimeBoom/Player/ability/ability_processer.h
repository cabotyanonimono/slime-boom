#pragma once
#include "iability.h"
#include "Components/component.h"

namespace SlimeBoom
{
class AbilityProcesser : public engine::Component
{
    std::unordered_set<std::shared_ptr<IAbility>> m_abilities_;

public:
    void Attach(const std::shared_ptr<IAbility>& ability);
    void Detach(const std::shared_ptr<IAbility>& ability);
    void OnUpdate() override;
    void OnFixedUpdate() override;

    template <typename T>
    bool HasAbility();
    [[nodiscard]] std::unordered_set<std::shared_ptr<IAbility>> GetAbilities();

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );
    }
};

template <typename T>
bool AbilityProcesser::HasAbility()
{
    return std::any_of(m_abilities_.begin(), m_abilities_.end(), [](const auto& card)
    {
        return std::dynamic_pointer_cast<T>(card) != nullptr;
    });
}
}

CEREAL_CLASS_VERSION(SlimeBoom::AbilityProcesser, 10)
