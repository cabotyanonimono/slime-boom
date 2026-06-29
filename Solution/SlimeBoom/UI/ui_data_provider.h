#pragma once
#include "../Exp/level_up_controller.h"
#include "../Player/player_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
class UiDataProvider final : public engine::Component
{
    engine::AssetPtr<PlayerComponent> m_player_component_;
    engine::AssetPtr<LevelUpController> m_level_up_controller_;

public:
    void OnInspectorGui() override;

    const PlayerData& GetPlayerData() const;
    int GetCurrentLevelExp() const;
    int GetRequiredExp() const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_player_component_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_level_up_controller_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::UiDataProvider, 2)
