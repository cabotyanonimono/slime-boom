#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"

class ObjectDisabler : public engine::Component
{
public:
    void OnStart() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );
    }
};

CEREAL_CLASS_VERSION(ObjectDisabler, 1)