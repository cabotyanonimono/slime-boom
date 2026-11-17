#ifndef SLIME_GLOBALS_HLSLI
#define SLIME_GLOBALS_HLSLI

// ゲーム側が毎フレーム Material::SetGlobal〜 で渡す値（変数名で設定されます）
// player_pos        : PlayerDataPresenter / PlayerPositionPresenter
// slime_count       : SlimeSimulationComponent
// attack_data_count : PlayerDataPresenter
#ifndef SLIME_GLOBALS_REGISTER
#define SLIME_GLOBALS_REGISTER b0
#endif

cbuffer SlimeGlobals : register(SLIME_GLOBALS_REGISTER)
{
    float3 player_pos;
    int slime_count;
    int attack_data_count;
}

#endif
