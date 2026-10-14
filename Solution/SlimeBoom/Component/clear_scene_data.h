#pragma once
#include "../Player/player_data.h"

namespace SlimeBoom
{
// クリア画面に渡す結果データ(クリアしたか、撃破数、プレイヤーの最終ステータス)。
struct ClearSceneData
{
    bool is_game_cleared;
    int eliminate_count;
    PlayerData player_data;
};
}