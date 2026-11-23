#pragma once

namespace SlimeBoom
{
// 1ウェーブ分の設定(継続時間・基本のスライム数・毎秒の追加数・種類ごとの出現率)。
struct WaveData
{
    float time;
    uint32_t num_slimes;
    uint32_t spawn_num_per_second;
    // 種類の表(SlimeSimulationComponent)の並び順ごとの出現率
    std::vector<float> spawn_rates;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(time),
            CEREAL_NVP(num_slimes)
        );

        if (version >= 5)
        {
            ar(
                CEREAL_NVP(spawn_rates)
            );
        }
        else if (version >= 3)
        {
            // version 4までは Normal・Speed・Tank の3つ固定の配列で保存していた
            std::array<float, 3> legacy_spawn_rates{};
            ar(
                cereal::make_nvp("spawn_rates", legacy_spawn_rates)
            );
            spawn_rates.assign(legacy_spawn_rates.begin(), legacy_spawn_rates.end());
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

CEREAL_CLASS_VERSION(SlimeBoom::WaveData, 5)
