#pragma once
#include "engine_time.h"
#include "Math/mathf.h"

namespace GameTime
{
/// ゲームが停止中(TimeScaleが0)かどうかを返す。ゲームオーバー画面やレベルアップ画面の表示中に停止する。
inline bool IsGameStopped()
{
    return engine::Mathf::Approximately(engine::Time::Get()->TimeScale(), 0.0f);
}
}
