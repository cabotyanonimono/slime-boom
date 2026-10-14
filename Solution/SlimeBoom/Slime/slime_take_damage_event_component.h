#pragma once
#include "event.h"
#include "../compute_result.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
// ComputeResultのダメージ回数(deal_damage_count)を監視し、スライムがダメージを受けたことを通知するComponent。
class SlimeTakeDamageEventComponent : public engine::Component
{
    int m_before_deal_damage_count_ = 0;
    engine::Event<int> m_on_damage_event_;
    engine::AssetPtr<ComputeResult> m_compute_result_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    /// スライムがダメージを受けたときに呼ばれるコールバックを登録する。
    /// @param callback 前回からのダメージ回数の増加分を引数に呼ばれる
    /// @return RemoveOnDamageEventListenerに渡すトークン
    size_t AddOnDamageEventListener(const std::function<void(int)>& callback);
    /// ダメージ時のコールバックを解除する。
    /// @param token AddOnDamageEventListenerで受け取ったトークン
    void RemoveOnDamageEventListener(size_t token);
    
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

CEREAL_CLASS_VERSION(SlimeBoom::SlimeTakeDamageEventComponent, 1)
