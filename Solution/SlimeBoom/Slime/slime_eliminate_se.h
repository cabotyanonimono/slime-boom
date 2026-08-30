#pragma once
#include "slime_eliminate_checker.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
class SlimeEliminateSe : public engine::Component
{
    engine::AssetPtr<SlimeEliminateChecker> m_slime_eliminate_checker_;
    
public:
    void OnInspectorGui() override;
    void OnStart() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_slime_eliminate_checker_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeEliminateSe, 1)
