#pragma once
#include "../Exp/exp_simulation_component.h"
#include "../Exp/level_up_controller.h"
#include "../LevelUpScreen/card_controller.h"
#include "../Player/player_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
// UIが表示に使うゲームのデータ(プレイヤー・レベル・カード)を、まとめて提供するComponent。
class UiDataProvider final : public engine::Component
{
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    engine::AssetPtr<LevelUpController> m_level_up_controller_;
    engine::AssetPtr<CardController> m_card_controller_;
    engine::AssetPtr<PlayerComponent> m_player_;
    engine::AssetPtr<ExpSimulationComponent> m_exp_simulation_;

public:
    void OnInspectorGui() override;

    /// プレイヤーのステータスを返す。
    const PlayerData& GetPlayerData() const;
    /// 現在のレベルで獲得した経験値を返す。
    int GetCurrentLevelExp() const;
    /// 次のレベルに必要な経験値を返す。
    int GetRequiredExp() const;
    /// 現在提示しているカードを返す。
    std::array<std::shared_ptr<CardBase>, CardController::kCardCount> GetCards() const;
    /// 回避のクールダウンの進行度を割合で返す。
    float GetAvoidCooldownRatio() const;
    /// すべての種類のカードを持つCardPoolを返す。
    std::shared_ptr<CardPool> GetCardPool() const;
    /// プレイヤーのアビリティを持つAbilityProcesserを返す。
    std::shared_ptr<AbilityProcesser> GetAbilityProcesser() const;
    /// 経験値の獲得範囲を返す。
    float GetAcquisitionRange() const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_level_up_controller_)
            );
        }
        
        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_card_controller_)
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
                CEREAL_NVP(m_player_)
            );
        }

        if (version >= 6)
        {
            ar(
                CEREAL_NVP(m_exp_simulation_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::UiDataProvider, 6)
