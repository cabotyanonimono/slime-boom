#pragma once
#include "event.h"
#include "../compute_result.h"
#include "Asset/asset_ptr.h"

namespace SlimeBoom
{
class PlayerDamageDispatcher : public engine::Component
{
    engine::AssetPtr<ComputeResult> m_compute_result_;
    int m_current_damage_ = 0;

    engine::Event<float> m_on_take_damage_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    size_t AddOnTakeDamageListener(const std::function<void(float)>& callback);
    void RemoveOnTakeDamageListener(size_t token);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_compute_result_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerDamageDispatcher, 2)
