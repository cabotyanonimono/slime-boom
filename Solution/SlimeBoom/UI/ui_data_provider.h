#pragma once
#include "../Exp/level_up_controller.h"
#include "../LevelUpScreen/card_controller.h"
#include "../Player/player_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
class UiDataProvider final : public engine::Component
{
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    engine::AssetPtr<LevelUpController> m_level_up_controller_;
    engine::AssetPtr<CardController> m_card_controller_;
    engine::AssetPtr<PlayerComponent> m_player_;

public:
    void OnInspectorGui() override;

    const PlayerData& GetPlayerData() const;
    int GetCurrentLevelExp() const;
    int GetRequiredExp() const;
    std::array<std::shared_ptr<CardBase>, CardController::kCardCount> GetCards() const;
    float GetAvoidCooldownRatio() const;

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
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::UiDataProvider, 5)
