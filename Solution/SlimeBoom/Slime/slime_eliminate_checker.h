#pragma once
#include "../compute_result.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
class SlimeEliminateChecker : public engine::Component
{
    engine::AssetPtr<ComputeResult> m_compute_result_;
    int m_eliminate_slime_count_;
    engine::Event<int> m_on_eliminate_;
    
public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    size_t AddOnEliminateListener(const std::function<void(int)>& listener);
    void RemoveOnEliminateListener(size_t token);
    
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

CEREAL_CLASS_VERSION(SlimeBoom::SlimeEliminateChecker, 1)