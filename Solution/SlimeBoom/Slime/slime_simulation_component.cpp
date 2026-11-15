#include "pch.h"
#include "slime_simulation_component.h"

#include "gui.h"
#include "../Sound/sound_manager_component.h"
#include "../Utils/gpu_buffer.h"
#include "../Utils/random.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
void SlimeSimulationComponent::SetCurrentSlimesCount(const int count)
{
    m_current_slime_count_ = std::min(count, kMaxSlimesCount);

    const auto group_count = (m_current_slime_count_ + 64 - 1) / 64;

    if (m_slime_generator_compute_ != nullptr)
        m_slime_generator_compute_->SetGroupCountX(group_count);
    if (m_slime_physics_compute_ != nullptr)
        m_slime_physics_compute_->SetGroupCountX(group_count);
    if (m_closest_slime_compute_ != nullptr)
        m_closest_slime_compute_->SetGroupCountX(group_count);
    if (m_player_attack_compute_ != nullptr)
        m_player_attack_compute_->SetGroupCountX(group_count);

    engine::Material::SetGlobalInt("slime_count", m_current_slime_count_);

    m_slime_renderer_->SetInstanceCount(count);
}

void SlimeSimulationComponent::SetSpawnRate(const std::array<float, SlimeTypes::Count> rates) const
{
    if (m_slime_generator_compute_.CastedLock() == nullptr)
        return;

    auto &material = m_slime_generator_compute_->GetMaterial();
    material.SetFloat("normal_spawn_rate", rates[0]);
    material.SetFloat("speed_spawn_rate", rates[1]);
    material.SetFloat("tank_spawn_rate", rates[2]);
}

void SlimeSimulationComponent::OnStart()
{
    m_slime_take_damage_event_component_->AddOnDamageEventListener([this](int damage_count){SoundManagerComponent::Play(kSoundTypes::kSlimeTakeDamage);});
    
    m_slime_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(SlimeData), kMaxSlimesCount, "slimes");

    engine::Material::SetGlobalBuffer("slimes", m_slime_buffer_.GetHandle());
    engine::Material::SetGlobalInt("slime_count", m_current_slime_count_);
}

void SlimeSimulationComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Slime Generator", m_slime_generator_compute_);
    engine::Gui::PropertyField("Slime Physics", m_slime_physics_compute_);
    engine::Gui::PropertyField("Closest Slime", m_closest_slime_compute_);
    engine::Gui::PropertyField("Player Attack", m_player_attack_compute_);
    engine::Gui::PropertyField("Slime Take Damage Event", m_slime_take_damage_event_component_);

    engine::Gui::PropertyField("Slime Renderer", m_slime_renderer_);

    if (engine::Gui::PropertyField("Current Slime Count", m_current_slime_count_))
        SetCurrentSlimesCount(m_current_slime_count_);
}

void SlimeSimulationComponent::OnUpdate()
{
    engine::Material::SetGlobalInt("slime_count", m_current_slime_count_);
}

int SlimeSimulationComponent::GetCurrentSlimesCount() const
{
    return m_current_slime_count_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeSimulationComponent)