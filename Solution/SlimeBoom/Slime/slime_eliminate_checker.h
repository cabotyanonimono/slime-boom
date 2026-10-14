#pragma once
#include "event.h"
#include "../compute_result.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
// ComputeResultの撃破数(eliminate_slime_count)を監視し、増加分を通知するComponent。
class SlimeEliminateChecker : public engine::Component
{
    engine::AssetPtr<ComputeResult> m_compute_result_;
    int m_eliminate_slime_count_;
    engine::Event<int> m_on_eliminate_;
    
public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    /// スライムが倒されたときに呼ばれるコールバックを登録する。
    /// @param listener 前回からの撃破数の増加分を引数に呼ばれる
    /// @return RemoveOnEliminateListenerに渡すトークン
    size_t AddOnEliminateListener(const std::function<void(int)>& listener);
    /// 撃破時のコールバックを解除する。
    /// @param token AddOnEliminateListenerで受け取ったトークン
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