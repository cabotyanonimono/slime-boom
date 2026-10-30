#include "pch.h"
#include "explosion_ability.h"
#include "ability_data/ability_data_store.h"

namespace SlimeBoom
{
ExplosionAbility::ExplosionAbility(const std::shared_ptr<PlayerDataComponent>& player_data,
                                   const std::shared_ptr<SlimeExplosionComponent>& explosion_component):
    m_player_data_(player_data),
    m_explosion_component_(explosion_component)
{
}

void ExplosionAbility::OnAttach()
{
    m_explosion_component_->SetEnabled(true);
}

void ExplosionAbility::OnDetach()
{
    m_explosion_component_->SetEnabled(false);
}

void ExplosionAbility::Update(float delta_time)
{
    // value: ダメージ倍率, size: 爆発半径, knockback: ノックバック, count: 最大連鎖数, arc: 発動確率の倍率
    const auto explosion_data = AbilityDataStore::GetAbilityData(kAbilityTypesExplosion);
    m_explosion_component_->SetExplosion(
        explosion_data.size + m_player_data_->GetAttackRange(),
        m_player_data_->GetAttackPower() * explosion_data.value,
        explosion_data.knockback,
        static_cast<uint32_t>(std::max(explosion_data.count, 1.0f)),
        explosion_data.arc);
}
}
