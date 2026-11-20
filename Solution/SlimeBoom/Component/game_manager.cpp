#include "pch.h"
#include "game_manager.h"

#include "engine_time.h"
#include "gui.h"
#include "Physics/physics.h"

namespace SlimeBoom
{
bool GameManager::IsGameClear() const
{
    return m_clear_time_ < m_time_since_game_start_sec_;
}

bool GameManager::IsGameOver() const
{
    return m_player_->IsDeadEffectEnd();
}

bool GameManager::IsGameEnd() const
{
    return IsGameClear() || IsGameOver();
}

void GameManager::GameEnd() const
{
    engine::Input::SetMouseMode(engine::kMouseMode::kNormal);
    ClearSceneData clear_scene_data;
    clear_scene_data.eliminate_count = *static_cast<int*>(m_compute_result_->GetValue("eliminate_slime_count"));
    clear_scene_data.player_data = m_player_data_->GetPlayerData();
    clear_scene_data.is_game_cleared = IsGameClear();
    m_clear_scene_controller_->SetClearSceneData(clear_scene_data);
    m_clear_scene_controller_->GameObject()->SetActive(true);
    engine::Time::Get()->TimeScale(0.0f);
}

void GameManager::OnInspectorGui()
{
    engine::Gui::PropertyField("Clear Scene Controller", m_clear_scene_controller_);
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
    engine::Gui::PropertyField("Player", m_player_);
    engine::Gui::PropertyField("Clear Time", m_clear_time_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

void GameManager::OnStart()
{
    engine::Physics::SetGravity(Vector3(0.0f, -18.0f, 0.0f));
    //回避(速度79)でも1サブステップの移動量(79 * 0.02 / 4 ≒ 0.4)がプレイヤーのカプセル半径(1.0)を超えないようにし、壁のすり抜けを防ぐ
    engine::Physics::SetSubSteps(kPhysicsSubSteps);
    engine::Input::SetMouseMode(engine::kMouseMode::kLocked);
}

void GameManager::OnUpdate()
{
    m_time_since_game_start_sec_ += engine::Time::GetDeltaTime();

    if (IsGameEnd())
        GameEnd();
}

void GameManager::OnDestroy()
{
    engine::Time::Get()->TimeScale(1.0f);
}

float GameManager::TimeSinceGameStartSec() const
{
    return m_time_since_game_start_sec_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::GameManager)
