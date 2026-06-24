#pragma once

namespace SlimeBoom
{
struct WaveData
{
    float time;
    uint32_t num_slimes;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(time),
            CEREAL_NVP(num_slimes)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::WaveData, 1)
