#ifndef __SLIME_EXPLOSION_HLSLI__
#define __SLIME_EXPLOSION_HLSLI__

// スライム撃破時の連鎖爆発
// C++側の SlimeExplosionData / SlimeExplosionParams / kMaxExplosionCount と合わせること

#define SLIME_EXPLOSION_MAX 128
// 先頭の要素はヘッダー。generationを登録数のカウンターとして使い、爆発は1番目以降に入れる
// (エンジンのルートシグネチャのUAVはu1～u6までなので、カウンター用のバッファを別に用意できない)
#define SLIME_EXPLOSION_HEADER 0

struct SlimeExplosion
{
    float3 position;
    uint generation; // 何段目の連鎖か(プレイヤーの攻撃で倒した場合は0)
    uint frame;      // 登録されたフレーム。次のフレームにダメージ判定される
    float3 padding;
};

#endif
