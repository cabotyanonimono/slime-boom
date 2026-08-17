#include "pch.h"
#include "slime_simulation_component.h"

#include "../Sound/sound_manager_component.h"
#include "Rendering/gpu_resource_manager.h"
#include "../Utils/random.h"
#include "Rendering/render_pipeline.h"
#include "Rendering/CabotEngine/Graphics/ConstantBuffer.h"

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
    
    if (m_slime_count_buffer_ != nullptr)
        m_slime_count_buffer_->UpdateBuffer(&m_current_slime_count_);

    if (m_slime_renderer_ != nullptr)
        m_slime_renderer_->instance_count = m_current_slime_count_;
}

void SlimeSimulationComponent::SetSpawnRate(const std::array<float, SlimeTypes::Count> rates) const
{
    auto spawn_rate_buffer = m_slime_generator_compute_->GetMaterialBlock()->GetConstantBufferData("SpawnRate");
    if (spawn_rate_buffer == nullptr)
        return;

    spawn_rate_buffer->SetFloatData("normal_spawn_rate", rates[0]);
    spawn_rate_buffer->SetFloatData("speed_spawn_rate", rates[1]);
    spawn_rate_buffer->SetFloatData("tank_spawn_rate", rates[2]);
}

void SlimeSimulationComponent::OnStart()
{
    m_slime_take_damage_event_component_->AddOnDamageEventListener([this](int damage_count){SoundManagerComponent::Play(kSoundTypes::kSlimeTakeDamage);});
    
    m_slime_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(SlimeData), kMaxSlimesCount);
    m_slime_buffer_->CreateBuffer();

    m_slime_count_buffer_ = std::make_shared<engine::ConstantBuffer>(sizeof(uint32_t));
    m_slime_count_buffer_->CreateBuffer();
    m_slime_count_buffer_->UpdateBuffer(&m_current_slime_count_);

    engine::GpuResourceManager::SetGlobalBuffer("slimes", m_slime_buffer_);
    engine::GpuResourceManager::SetGlobalBuffer("SlimeCount", m_slime_count_buffer_);
}

void SlimeSimulationComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Slime Generator", m_slime_generator_compute_);
    engine::Gui::PropertyField("Slime Physics", m_slime_physics_compute_);
    engine::Gui::PropertyField("Closest Slime", m_closest_slime_compute_);
    engine::Gui::PropertyField("Player Attack", m_player_attack_compute_);
    engine::Gui::PropertyField("Slime Take Damage Event", m_slime_take_damage_event_component_);

    if (engine::Gui::PropertyField("Slime Renderer", m_slime_renderer_))
    {
        m_slime_renderer_->instance_count = m_current_slime_count_;
    }
    
    if (engine::Gui::PropertyField("Current Slime Count", m_current_slime_count_))
        SetCurrentSlimesCount(m_current_slime_count_);
}

void SlimeSimulationComponent::OnUpdate()
{
    m_slime_count_buffer_->UpdateBuffer(&m_current_slime_count_);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeSimulationComponent)