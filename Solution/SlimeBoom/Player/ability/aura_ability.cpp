#include "pch.h"
#include "aura_ability.h"

#include "../player_attack_data.h"
#include "ability_data/ability_data_store.h"

namespace SlimeBoom
{
void AuraAbility::Execute(float delta_time)
{
    PlayerAttackData attack_data;
    const auto slash_data = AbilityDataStore::GetAbilityData(kAbilityTypesAura);
    m_cooldown_ = slash_data.cooldown / m_player_data_->GetAttackSpeed();

    auto transform = m_aura_renderer_->Transform();
    transform->SetLocalScale(Vector3(slash_data.size, slash_data.size, slash_data.size));

    attack_data.arc = DirectX::XM_2PI;
    attack_data.angle = Vector3(0.0f, 0.0f, -1.0f);
    attack_data.size = slash_data.size + m_player_data_->GetAttackRange();
    attack_data.damage = m_player_data_->GetAttackPower() * slash_data.value;
    attack_data.power = slash_data.knockback;
    m_effekseer_renderer_->Play();
    m_player_data_presenter_->SetAttackData(attack_data);
}

AuraAbility::AuraAbility(const std::shared_ptr<PlayerDataComponent>& player_data,
                                    const std::shared_ptr<engine::GameObject>& aura_renderer,
                                    const std::shared_ptr<PlayerDataPresenter>& player_data_presenter,
                                    const std::shared_ptr<engine::EffekseerRenderer>& effect_renderer):
    CooldownAbility(1.0f, player_data),
    m_player_data_presenter_(player_data_presenter),
    m_player_data_(player_data),
    m_aura_renderer_(aura_renderer),
    m_effekseer_renderer_(effect_renderer)
{
    m_aura_renderer_->SetActive(true);
}
}