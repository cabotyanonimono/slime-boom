#include "pch.h"
#include "slime_explosion_component.h"

#include "engine_time.h"
#include "gui.h"
#include "slime_simulation_component.h"
#include "Rendering/gpu_resource_manager.h"

namespace SlimeBoom
{
void SlimeExplosionComponent::OnStart()
{
    m_explosion_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(SlimeExplosionData), kMaxExplosionCount);
    m_explosion_buffer_->CreateBuffer();

    m_params_buffer_ = std::make_shared<engine::ConstantBuffer>(sizeof(SlimeExplosionParams));
    m_params_buffer_->CreateBuffer();

    engine::GpuResourceManager::SetGlobalBuffer("slime_explosions", m_explosion_buffer_);
    engine::GpuResourceManager::SetGlobalBuffer("ExplosionParams", m_params_buffer_);
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
    // バッファの初期値(frame = 0)が爆発として扱われないように、2以上から数える
    m_params_.frame = engine::Time::Get()->Frames() + 2;
    m_params_.chain_decay = m_chain_decay_;
    m_params_.damage_color_time = m_damage_color_time_;
    m_params_buffer_->UpdateBuffer(&m_params_);

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
