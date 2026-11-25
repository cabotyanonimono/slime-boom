#include "pch.h"
#include "slime_explosion_component.h"

#include "engine_time.h"
#include "gui.h"
#include "slime_simulation_component.h"
#include "../Utils/gpu_buffer.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
void SlimeExplosionComponent::OnStart()
{
    m_explosion_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(SlimeExplosionData), kMaxExplosionCount,
                                                               "slime_explosions");
    engine::Material::SetGlobalBuffer("slime_explosions", m_explosion_buffer_.GetHandle());
}

void SlimeExplosionComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Explosion Compute", m_explosion_compute_);
    engine::Gui::PropertyField("Chance", m_chance_);
    engine::Gui::PropertyField("Chain Decay", m_chain_decay_);
    engine::Gui::PropertyField("Damage Color Time", m_damage_color_time_);
}

void SlimeExplosionComponent::OnUpdate()
{
    m_params_.frame = engine::Time::Get()->Frames() + 2;
    m_params_.chain_decay = m_chain_decay_;
    m_params_.damage_color_time = m_damage_color_time_;

    // SetEnabled / SetExplosionで変わる値と毎フレーム変わるframeを、cbuffer ExplosionParamsへ名前で書き込む
    engine::Material::SetGlobalFloat("explosion_chance", m_params_.chance);
    engine::Material::SetGlobalFloat("explosion_radius", m_params_.radius);
    engine::Material::SetGlobalFloat("explosion_damage", m_params_.damage);
    engine::Material::SetGlobalFloat("explosion_knockback", m_params_.knockback);
    engine::Material::SetGlobalFloat("explosion_chain_decay", m_params_.chain_decay);
    engine::Material::SetGlobalInt("explosion_max_generation", static_cast<int>(m_params_.max_generation));
    engine::Material::SetGlobalInt("explosion_frame", static_cast<int>(m_params_.frame));
    engine::Material::SetGlobalInt("explosion_enabled", static_cast<int>(m_params_.enabled));
    engine::Material::SetGlobalFloat("explosion_damage_color_time", m_params_.damage_color_time);

    if (m_params_.enabled == 0 || m_explosion_compute_.CastedLock() == nullptr)
        return;

    m_explosion_compute_->SetGroupCount((SlimeSimulationComponent::GetMaxSlimeCount() + kThreadGroupSize - 1) / kThreadGroupSize, 1, 1);
    m_explosion_compute_->Execute();
}

void SlimeExplosionComponent::SetEnabled(const bool enabled)
{
    m_params_.enabled = enabled ? 1 : 0;
}

void SlimeExplosionComponent::SetExplosion(const float radius, const float damage, const float knockback,
                                           const uint32_t max_generation, const float chance_multiplier)
{
    m_params_.radius = radius;
    m_params_.damage = damage;
    m_params_.knockback = knockback;
    m_params_.max_generation = max_generation;
    m_params_.chance = m_chance_ * chance_multiplier;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeExplosionComponent)
