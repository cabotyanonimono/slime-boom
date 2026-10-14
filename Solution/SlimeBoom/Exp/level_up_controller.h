#pragma once
#include "event.h"
#include "../compute_result.h"
#include "../Player/player_data_component.h"
#include "Asset/asset_ptr.h"

namespace SlimeBoom
{
// 経験値の管理とレベルアップの判定を行うComponent。レベルアップ時はイベントで通知する。
class LevelUpController : public engine::Component
{
    engine::AssetPtr<ComputeResult> m_compute_result_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    int m_current_exp_ = 0;
    int m_current_level_exp_ = 0;
    int m_current_level_ = 0;
    int m_required_exp_ = 0;
    int m_base_required_exp_ = 50;
    float m_required_exp_up_rate_ = 1.1f;
    
    engine::Event<int> m_on_level_up_;

    /// 現在の必要経験値に上昇率を掛けた値と基本必要経験値から、次のレベルに必要な経験値を計算する。
    int CalcRequiredExp();

public:
    void OnInspectorGui() override;
    void OnStart() override;
    /// ComputeResultの累計経験値の増加分に経験値倍率を掛けて加算し、必要経験値に達したらレベルアップを通知する。
    /// レベルアップ時はマウスを解放する。
    void OnUpdate() override;

    /// レベルアップ時に呼ばれるコールバックを登録する。
    /// @param callback 新しいレベルを引数に呼ばれる
    /// @return RemoveOnLevelUpListenerに渡すトークン
    size_t AddOnLevelUpListener(const std::function<void(int)>& callback);
    /// レベルアップ時のコールバックを解除する。
    /// @param token AddOnLevelUpListenerで受け取ったトークン
    void RemoveOnLevelUpListener(size_t token);

    /// @return 現在の総獲得EXP
    int GetCurrentExp() const;
    /// @return 現在のレベルで獲得したEXP。
    int GetCurrentLevelExp() const;
    /// 現在のレベルを返す。
    int GetCurrentLevel() const;
    /// 次のレベルに必要な経験値を返す。
    int GetRequiredExp() const;

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
                CEREAL_NVP(m_current_level_exp_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_player_data_)
            );
        }

        if (version >= 5)
        {
            ar(
                CEREAL_NVP(m_base_required_exp_),
                CEREAL_NVP(m_required_exp_up_rate_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::LevelUpController, 5)
