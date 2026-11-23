#include "pch.h"
#include "slime_explosion_effect_component.h"

#include "gui.h"
#include "../Utils/gpu_buffer.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
void SlimeExplosionEffectComponent::OnStart()
{
    m_particle_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(SlimeExplosionParticleData), kMaxParticleCount,
                                                              "slime_explosion_particles");
    engine::Material::SetGlobalBuffer("slime_explosion_particles", m_particle_buffer_.GetHandle());
}

void SlimeExplosionEffectComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Update Compute", m_update_compute_);

    ImGui::SeparatorText("Motion");
    engine::Gui::PropertyField("Life Time", m_life_time_);
    engine::Gui::PropertyField("Speed Min", m_speed_min_);
    engine::Gui::PropertyField("Speed Max", m_speed_max_);
    engine::Gui::PropertyField("Gravity", m_gravity_);
    engine::Gui::PropertyField("Drag", m_drag_);
    engine::Gui::PropertyField("Bounce", m_bounce_);
    engine::Gui::PropertyField("Bounce Friction", m_bounce_friction_);
    engine::Gui::PropertyField("Up Min", m_up_min_);
    engine::Gui::PropertyField("Up Max", m_up_max_);
    if (engine::Gui::PropertyField("Particle Count", m_particle_count_))
        m_particle_count_ = std::clamp(m_particle_count_, 1u, kParticlePerExplosion);

    ImGui::SeparatorText("Look");
    engine::Gui::PropertyField("Start Color", m_start_color_);
    engine::Gui::PropertyField("End Color", m_end_color_);
    engine::Gui::PropertyField("Particle Size", m_particle_size_);
    engine::Gui::PropertyField("Glow", m_glow_);
    engine::Gui::PropertyField("Size Random Min", m_size_random_min_);
    engine::Gui::PropertyField("Size Random Max", m_size_random_max_);
}

void SlimeExplosionEffectComponent::OnUpdate()
{
    // cbuffer ExplosionParticleParamsの各変数へ、名前で書き込む
    engine::Material::SetGlobalFloat("life_time", m_life_time_);
    engine::Material::SetGlobalFloat("speed_min", m_speed_min_);
    engine::Material::SetGlobalFloat("speed_max", m_speed_max_);
    engine::Material::SetGlobalFloat("gravity", m_gravity_);
    engine::Material::SetGlobalFloat("drag", m_drag_);
    engine::Material::SetGlobalFloat("bounce", m_bounce_);
    engine::Material::SetGlobalFloat("up_min", m_up_min_);
    engine::Material::SetGlobalFloat("up_max", m_up_max_);
    engine::Material::SetGlobalFloat("bounce_friction", m_bounce_friction_);
    engine::Material::SetGlobalInt("particle_count", static_cast<int>(m_particle_count_));

    // cbuffer ExplosionParticleLookの各変数へ、名前で書き込む
    engine::Material::SetGlobalColor("start_color", m_start_color_);
    engine::Material::SetGlobalColor("end_color", m_end_color_);
    engine::Material::SetGlobalFloat("particle_size", m_particle_size_);
    engine::Material::SetGlobalFloat("glow", m_glow_);
    engine::Material::SetGlobalFloat("size_random_min", m_size_random_min_);
    engine::Material::SetGlobalFloat("size_random_max", m_size_random_max_);

    if (m_update_compute_.CastedLock() == nullptr)
        return;

    m_update_compute_->SetGroupCount((kMaxParticleCount + kThreadGroupSize - 1) / kThreadGroupSize, 1, 1);
    m_update_compute_->Execute();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeExplosionEffectComponent)
