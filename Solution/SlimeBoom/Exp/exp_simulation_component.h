#pragma once
#include "../compute_result.h"
#include "Components/component.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
class ExpSimulationComponent : public engine::Component
{
    static constexpr size_t kMaxExpBufferSize = 1024;

    std::shared_ptr<engine::StructuredBuffer> m_exp_buffer_;
    std::shared_ptr<engine::StructuredBuffer> m_exp_count_buffer_;
    engine::AssetPtr<ComputeResult> m_compute_result_;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_compute_result_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ExpSimulationComponent, 1)
