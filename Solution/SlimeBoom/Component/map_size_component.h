#pragma once
#include "Components/component.h"

namespace SlimeBoom
{
class MapSizeComponent : public engine::Component
{
    Vector3 m_min_;
    Vector3 m_max_;
    
public:
    void OnInspectorGui() override;
    Vector3 GetMin() const { return m_min_; }
    Vector3 GetMax() const { return m_max_; }
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_min_),
            CEREAL_NVP(m_max_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::MapSizeComponent, 1)
