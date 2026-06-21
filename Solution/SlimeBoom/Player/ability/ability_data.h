#pragma once
#include "slash_ability.h"
#include "Components/component.h"

namespace SlimeBoom
{
class AbilityData : public engine::Component
{
public:
    inline static SlashAbilityData slash_ability_data;
    
    void OnInspectorGui() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(slash_ability_data)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AbilityData, 1)
