#pragma once
#include "iability.h"
#include "../player_data_component.h"
#include "../../Slime/slime_explosion_component.h"

namespace SlimeBoom
{
// 倒したスライムが確率で爆発し、周囲のスライムを巻き込んで連鎖するアビリティ。
// 判定はGPU側(SlimeExplosionComponent)で行い、このクラスは性能を毎フレーム送るだけ。
class ExplosionAbility : public IAbility
{
    std::shared_ptr<PlayerDataComponent> m_player_data_;
    std::shared_ptr<SlimeExplosionComponent> m_explosion_component_;

public:
    /// @param player_data プレイヤーのステータス
    /// @param explosion_component 連鎖爆発を処理するComponent
    ExplosionAbility(const std::shared_ptr<PlayerDataComponent>& player_data,
                     const std::shared_ptr<SlimeExplosionComponent>& explosion_component);

    void OnAttach() override;
    void OnDetach() override;
    /// AbilityDataとプレイヤーの攻撃力から爆発の性能を計算し、SlimeExplosionComponentに送る。
    void Update(float delta_time) override;
};
}
