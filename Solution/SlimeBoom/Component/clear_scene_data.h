#pragma once
#include "../Player/player_data.h"

namespace SlimeBoom
{
struct ClearSceneData
{
    bool is_game_cleared;
    int eliminate_count;
    PlayerData player_data;
};
}