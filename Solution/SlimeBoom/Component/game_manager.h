#pragma once
#include "clear_scene_controller.h"
#include "../Player/player_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
// ゲーム全体の進行を管理するComponent。
// 経過時間の計測、クリア/ゲームオーバーの判定、終了時のクリア画面の表示を行う。
class GameManager : public engine::Component
{
    float m_clear_time_;
    float m_time_since_game_start_sec_ = 0.0f;
    engine::AssetPtr<ClearSceneController> m_clear_scene_controller_;
    engine::AssetPtr<ComputeResult> m_compute_result_;
    engine::AssetPtr<PlayerComponent> m_player_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;

    /// マウスを解放し、結果データをクリア画面に渡して表示したうえで、TimeScaleを0にしてゲームを停止する。
    void GameEnd() const;

public:
    void OnInspectorGui() override;
    /// マウスをロックする。
    void OnStart() override;
    /// 経過時間を加算し、クリアまたはゲームオーバーになったらGameEndを呼ぶ。
    void OnUpdate() override;
    /// 他のシーンに影響しないよう、TimeScaleを1に戻す。
    void OnDestroy() override;

    /// 経過時間がクリア時間を超えたかどうかを返す。
    bool IsGameClear() const;
    /// プレイヤーの死亡演出が終わったかどうかを返す。
    bool IsGameOver() const;
    /// クリアまたはゲームオーバーになったかどうかを返す。
    bool IsGameEnd() const;

    /// ゲーム開始からの経過時間(秒)を返す。
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
