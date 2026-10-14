#pragma once
#include "event.h"
#include "../compute_result.h"
#include "Asset/asset_ptr.h"

namespace SlimeBoom
{
// ComputeResultの被ダメージ量(taken_damage)から、フレームごとの被ダメージを計算して通知するComponent。
class PlayerDamageDispatcher : public engine::Component
{
    engine::AssetPtr<ComputeResult> m_compute_result_;
    int m_current_damage_ = 0;
    float m_receive_damage_multiplier_ = 1.0f;
    engine::Event<float> m_on_take_damage_;

public:
    void OnInspectorGui() override;
    /// 被ダメージ量の増加分に、経過時間と被ダメージ倍率を掛けてダメージイベントを発火する。TimeScaleが0の間は発火しない。
    void OnUpdate() override;

    /// ダメージを受けたときに呼ばれるコールバックを登録する。
    /// @param callback ダメージ量を引数に呼ばれる
    /// @return RemoveOnTakeDamageListenerに渡すトークン
    size_t AddOnTakeDamageListener(const std::function<void(float)>& callback);
    /// ダメージ時のコールバックを解除する。
    /// @param token AddOnTakeDamageListenerで受け取ったトークン
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

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_receive_damage_multiplier_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerDamageDispatcher, 3)
