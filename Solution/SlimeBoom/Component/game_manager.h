#pragma once
#include "clear_scene_controller.h"
#include "../Player/player_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
class GameManager : public engine::Component
{
    float m_clear_time_;
    float m_time_since_game_start_sec_ = 0.0f;
    engine::AssetPtr<ClearSceneController> m_clear_scene_controller_;
    engine::AssetPtr<ComputeResult> m_compute_result_;
    engine::AssetPtr<PlayerComponent> m_player_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;

    void GameEnd() const;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;
    void OnDestroy() override;

    bool IsGameClear() const;
    bool IsGameOver() const;
    bool IsGameEnd() const;

    float TimeSinceGameStartSec() const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this));

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_clear_scene_controller_),
                CEREAL_NVP(m_compute_result_),
                CEREAL_NVP(m_player_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_clear_scene_controller_),
                CEREAL_NVP(m_compute_result_),
                CEREAL_NVP(m_player_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_clear_time_)
            );
        }

        if (version >= 6)
        {
            ar(
                CEREAL_NVP(m_player_data_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::GameManager, 6)
