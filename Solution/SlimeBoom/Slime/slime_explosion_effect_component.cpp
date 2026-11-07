#include "pch.h"
#include "slime_explosion_effect_component.h"

#include "gui.h"
#include "Rendering/gpu_resource_manager.h"

namespace SlimeBoom
{
void SlimeExplosionEffectComponent::OnStart()
{
    m_particle_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(SlimeExplosionParticleData), kMaxParticleCount);
    m_particle_buffer_->CreateBuffer();

    m_params_buffer_ = std::make_shared<engine::ConstantBuffer>(sizeof(SlimeExplosionParticleParams));
    m_params_buffer_->CreateBuffer();

    m_look_buffer_ = std::make_shared<engine::ConstantBuffer>(sizeof(SlimeExplosionParticleLook));
    m_look_buffer_->CreateBuffer();

    engine::GpuResourceManager::SetGlobalBuffer("slime_explosion_particles", m_particle_buffer_);
    engine::GpuResourceManager::SetGlobalBuffer("ExplosionParticleParams", m_params_buffer_);
    engine::GpuResourceManager::SetGlobalBuffer("ExplosionParticleLook", m_look_buffer_);
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
    SlimeExplosionParticleParams params = {};
    params.life_time = m_life_time_;
    params.speed_min = m_speed_min_;
    params.speed_max = m_speed_max_;
    params.gravity = m_gravity_;
    params.drag = m_drag_;
    params.bounce = m_bounce_;
    params.up_min = m_up_min_;
    params.up_max = m_up_max_;
    params.bounce_friction = m_bounce_friction_;
    params.particle_count = m_particle_count_;
    m_params_buffer_->UpdateBuffer(&params);

    SlimeExplosionParticleLook look = {};
    look.start_color = m_start_color_;
    look.end_color = m_end_color_;
    look.particle_size = m_particle_size_;
    look.glow = m_glow_;
    look.size_random_min = m_size_random_min_;
    look.size_random_max = m_size_random_max_;
    m_look_buffer_->UpdateBuffer(&look);

    if (m_update_compute_.CastedLock() == nullptr)
        return;

    m_update_compute_->SetGroupCount((kMaxParticleCount + kThreadGroupSize - 1) / kThreadGroupSize, 1, 1);
    m_update_compute_->Execute();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeExplosionEffectComponent)
