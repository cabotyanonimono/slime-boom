#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/transform.h"

namespace SlimeBoom
{
class DelayFollower : public engine::Component
{
    float m_base_speed_ = 1.5f;
    float m_max_speed_distance_ = 15.0f;
    engine::AssetPtr<engine::Transform> m_target_;
    
public:

    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this), CEREAL_NVP(m_base_speed_), CEREAL_NVP(m_max_speed_distance_), CEREAL_NVP(m_target_));
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::DelayFollower, 1)