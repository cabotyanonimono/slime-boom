#include "pch.h"
#include "game_manager.h"

#include "engine_time.h"

namespace SlimeBoom
{
void GameManager::OnUpdate()
{
    m_time_since_game_start_ += engine::Time::GetDeltaTime();
}

float GameManager::TimeSinceGameStart() const
{
    return m_time_since_game_start_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::GameManager)
