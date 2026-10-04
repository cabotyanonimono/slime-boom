#pragma once
#include "../compute_result.h"
#include "Components/component.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
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
    void OnStart() override;
    void OnInspectorGui() override;
    void OnUpdate() override;

    int GetDeltaExpCount() const;
    int GetExpCount() const;
    float GetAcquisitionRange() const;

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