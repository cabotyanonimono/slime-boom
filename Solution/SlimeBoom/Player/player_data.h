#pragma once
#include "Components/component.h"

namespace SlimeBoom
{
struct PlayerData
{
    float hp;
    float speed;
    float attack_power;
    float attack_speed;

    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(hp),
            CEREAL_NVP(speed),
            CEREAL_NVP(attack_speed),
            CEREAL_NVP(attack_speed)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerData, 1)