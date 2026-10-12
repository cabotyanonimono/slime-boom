#include "pch.h"
#include "slime_death_effect_component.h"

#include "gui.h"
#include "Rendering/gpu_resource_manager.h"

namespace SlimeBoom
{
void SlimeDeathEffectComponent::OnStart()
{
    m_particle_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(SlimeDeathParticleData), kMaxParticleCount);
    m_particle_buffer_->CreateBuffer();

    m_particle_counter_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(uint32_t), 1);
    m_particle_counter_buffer_->CreateBuffer();

    engine::GpuResourceManager::SetGlobalBuffer("slime_death_particles", m_particle_buffer_);
    engine::GpuResourceManager::SetGlobalBuffer("slime_death_particle_counter", m_particle_counter_buffer_);
}

void SlimeDeathEffectComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Update Compute", m_update_compute_);
}

void SlimeDeathEffectComponent::OnUpdate()
{
    if (m_update_compute_.CastedLock() == nullptr)
        return;

    m_update_compute_->SetGroupCount((kMaxParticleCount + kThreadGroupSize - 1) / kThreadGroupSize, 1, 1);
    m_update_compute_->Execute();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeDeathEffectComponent)
