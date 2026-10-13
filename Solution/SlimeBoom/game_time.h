#pragma once
#include "engine_time.h"
#include "Math/mathf.h"

namespace GameTime
{
inline bool IsGameStopped()
{
    return engine::Mathf::Approximately(engine::Time::Get()->TimeScale(), 0.0f);
}
}
