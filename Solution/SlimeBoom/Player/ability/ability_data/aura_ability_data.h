#pragma once

namespace SlimeBoom
{
struct AuraAbilityData
{
    float arc;
    float size;
    float damage_multiplier;
    float power_multiplier;
    float cooldown;

    template<typename Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(arc),
            CEREAL_NVP(size),
            CEREAL_NVP(damage_multiplier),
            CEREAL_NVP(power_multiplier),
            CEREAL_NVP(cooldown)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AuraAbilityData, 1)