#pragma once
#include "../Slime/slime_types.h"

namespace SlimeBoom
{
// 1ウェーブ分の設定(継続時間・基本のスライム数・毎秒の追加数・種類ごとの出現率)。
struct WaveData
{
    float time;
    uint32_t num_slimes;
    uint32_t spawn_num_per_second;
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

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(spawn_num_per_second)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::WaveData, 4)
