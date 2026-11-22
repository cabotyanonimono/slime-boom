#pragma once
#include "Animation/animation_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

// AnimationComponentのルートモーション(アニメーションによる移動量)を、このGameObjectの位置に反映するComponent。
// 同じGameObjectに動的なRigidbodyがある場合は位置を直接動かさず、移動量を溜めておく。
// 溜めた移動量はConsumePendingDeltaで取り出し、Rigidbodyの速度として反映すること(壁抜け防止)。
class RootMotionController : public engine::Component
{
    engine::AssetPtr<engine::AnimationComponent> m_animation_;
    Vector3 m_pending_delta_ = Vector3::Zero;
    bool m_is_motion_enabled_ = true;

public:

    void OnInspectorGui() override;
    /// アニメーションの移動量をモデルのスケールで補正し、ローカル位置に加算する。
    /// 動的なRigidbodyがある場合は加算せずに溜めておく。
    void OnUpdate() override;

    /// 溜まっている移動量(World空間)を取り出し、0に戻す。
    Vector3 ConsumePendingDelta();
    /// ルートモーションを反映するかを切り替える。無効の間のアニメーションの移動量は捨てる。
    void SetEnabledMotion(bool enabled);

    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this), CEREAL_NVP(m_animation_));
    }
};

CEREAL_CLASS_VERSION(RootMotionController, 1)
