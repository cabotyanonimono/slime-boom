#pragma once
#include "../compute_result.h"
#include "Components/component.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
// 経験値オーブのシミュレーション用バッファをGPUに用意し、獲得数をCPU側から参照できるようにするComponent。
class ExpSimulationComponent : public engine::Component
{
    static constexpr size_t kMaxExpBufferSize = 1024;

    engine::AssetPtr<ComputeResult> m_compute_result_;
    std::shared_ptr<engine::StructuredBuffer> m_exp_buffer_;
    std::shared_ptr<engine::StructuredBuffer> m_exp_count_buffer_;
    float m_acquisition_range_ = 10.0f;

    int m_exp_count_ = 0;
    int m_delta_exp_count_ = 0;

public:
    /// 経験値用のGPUバッファを作成し、シェーダーから参照できるグローバルバッファとして登録する。
    void OnStart() override;
    void OnInspectorGui() override;
    /// ComputeResultの経験値獲得数から、前フレームからの増加数を計算する。
    void OnUpdate() override;

    /// 前フレームからの経験値の獲得数(増加分)を返す。
    int GetDeltaExpCount() const;
    /// 経験値の累計獲得数を返す。
    int GetExpCount() const;
    /// 経験値オーブを獲得できる範囲を返す。
    float GetAcquisitionRange() const;

    /// 経験値オーブを獲得できる範囲を設定する。シェーダーのグローバル値(AcquisitionRange)にも反映される。
    void SetAcquisitionRange(float range);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_acquisition_range_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_compute_result_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ExpSimulationComponent, 3)