#pragma once
#include "wave_data.h"
#include "../Slime/slime_simulation_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
// 経過時間に応じてウェーブを進め、スライムの数と出現率を切り替えるComponent。
class WaveController : public engine::Component
{
    uint32_t m_current_wave_index_ = 0;
    float m_current_time_ = 0.0f;
    std::vector<WaveData> m_wave_data_;
    engine::AssetPtr<SlimeSimulationComponent> m_slime_simulation_component_;
    
public:
    void OnInspectorGui() override;
    void OnStart() override;
    /// 毎秒の追加数を加味してスライムの数を更新し、ウェーブの時間が経過したら次のウェーブへ進める。
    void OnUpdate() override;

    /// 現在のウェーブの基本のスライム数を返す。
    [[nodiscard]] uint32_t GetCurrentNumSlimes() const;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_wave_data_),
            CEREAL_NVP(m_slime_simulation_component_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::WaveController, 1)