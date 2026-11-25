#include "pch.h"
#include "slime_explosion_effect_component.h"

#include "gui.h"
#include "../Utils/gpu_buffer.h"
#include "../Utils/particle_quad_renderer.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
void SlimeExplosionEffectComponent::OnStart()
{
    m_particle_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(SlimeExplosionParticleData), kMaxParticleCount,
                                                              "slime_explosion_particles");
    engine::Material::SetGlobalBuffer("slime_explosion_particles", m_particle_buffer_.GetHandle());

    m_particle_mesh_ = ParticleQuadRenderer::Setup(m_particle_renderer_, kMaxParticleCount);
}

void SlimeExplosionEffectComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Update Compute", m_update_compute_);
    engine::Gui::PropertyField("Particle Renderer", m_particle_renderer_);

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
    // インスペクターでの調整がすぐ反映されるよう、毎フレーム名前で書き込む
    engine::Material::SetGlobalFloat("explosion_particle_life_time", m_life_time_);
    engine::Material::SetGlobalFloat("explosion_particle_speed_min", m_speed_min_);
    engine::Material::SetGlobalFloat("explosion_particle_speed_max", m_speed_max_);
    engine::Material::SetGlobalFloat("explosion_particle_gravity", m_gravity_);
    engine::Material::SetGlobalFloat("explosion_particle_drag", m_drag_);
    engine::Material::SetGlobalFloat("explosion_particle_bounce", m_bounce_);
    engine::Material::SetGlobalFloat("explosion_particle_up_min", m_up_min_);
    engine::Material::SetGlobalFloat("explosion_particle_up_max", m_up_max_);
    engine::Material::SetGlobalFloat("explosion_particle_bounce_friction", m_bounce_friction_);
    engine::Material::SetGlobalInt("explosion_particle_count", static_cast<int>(m_particle_count_));

    engine::Material::SetGlobalColor("explosion_particle_start_color", m_start_color_);
    engine::Material::SetGlobalColor("explosion_particle_end_color", m_end_color_);
    engine::Material::SetGlobalFloat("explosion_particle_size", m_particle_size_);
    engine::Material::SetGlobalFloat("explosion_particle_glow", m_glow_);
    engine::Material::SetGlobalFloat("explosion_particle_size_random_min", m_size_random_min_);
    engine::Material::SetGlobalFloat("explosion_particle_size_random_max", m_size_random_max_);

    if (m_update_compute_.CastedLock() == nullptr)
        return;

    m_update_compute_->SetGroupCount((kMaxParticleCount + kThreadGroupSize - 1) / kThreadGroupSize, 1, 1);
    m_update_compute_->Execute();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeExplosionEffectComponent)
