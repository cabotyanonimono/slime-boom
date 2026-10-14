#pragma once
#include "Animation/animation_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

// AnimationComponentのルートモーション(アニメーションによる移動量)を、このGameObjectの位置に反映するComponent。
class RootMotionController : public engine::Component
{
    engine::AssetPtr<engine::AnimationComponent> m_animation_;
    
public:
    
    void OnInspectorGui() override;
    /// アニメーションの移動量をモデルのスケールで補正し、ローカル位置に加算する。
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this), CEREAL_NVP(m_animation_));
    }
};

CEREAL_CLASS_VERSION(RootMotionController, 1)