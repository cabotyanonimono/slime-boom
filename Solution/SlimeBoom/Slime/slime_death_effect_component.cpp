#include "pch.h"
#include "slime_death_effect_component.h"

#include "gui.h"
#include "Rendering/material.h"
#include "../Utils/gpu_buffer.h"
#include "../Utils/particle_quad_renderer.h"

namespace SlimeBoom
{
void SlimeDeathEffectComponent::OnStart()
{
    m_particle_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(SlimeDeathParticleData), kMaxParticleCount,
                                                              "slime_death_particles");
    m_particle_counter_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(uint32_t), 1,
                                                                      "slime_death_particle_counter");

    engine::Material::SetGlobalBuffer("slime_death_particles", m_particle_buffer_.GetHandle());
    engine::Material::SetGlobalBuffer("slime_death_particle_counter", m_particle_counter_buffer_.GetHandle());

    m_particle_mesh_ = ParticleQuadRenderer::Setup(m_particle_renderer_, kMaxParticleCount);
}

void SlimeDeathEffectComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Update Compute", m_update_compute_);
    engine::Gui::PropertyField("Particle Renderer", m_particle_renderer_);
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
