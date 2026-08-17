#pragma once
#include "../Slime/slime_types.h"

namespace SlimeBoom
{
struct WaveData
{
    float time;
    uint32_t num_slimes;
    std::array<float, SlimeTypes::Count> spawn_rates;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(time),
            CEREAL_NVP(num_slimes)
        );

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(spawn_rates)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::WaveData, 3)
