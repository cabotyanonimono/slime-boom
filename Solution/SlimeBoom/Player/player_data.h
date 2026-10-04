#pragma once
#include "Components/component.h"

namespace SlimeBoom
{
struct PlayerData
{
    float max_hp;
    float hp;
    float speed;
    float attack_power;
    float attack_speed;
    float attack_range;
    uint32_t attack_num;
    float avoid_time;
    float avoid_speed;
    float exp_multiplier;
    float avoid_percent;
    uint32_t level;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(hp),
            CEREAL_NVP(speed),
            CEREAL_NVP(attack_speed)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(attack_power)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(level)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(avoid_time),
                CEREAL_NVP(avoid_speed)
            );
        }

        if (version >= 5)
        {
            ar(
                CEREAL_NVP(exp_multiplier)
            );
        }

        if (version >= 6)
        {
            ar(
                CEREAL_NVP(max_hp)
            );
        }

        if (version >= 7)
        {
            ar(
                CEREAL_NVP(attack_range),
                CEREAL_NVP(attack_num),
                CEREAL_NVP(avoid_percent)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerData, 7)