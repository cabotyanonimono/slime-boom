#include "pch.h"
#include "slime_simulation_component.h"
#include "Rendering/gpu_resource_manager.h"
#include "../Utils/random.h"
#include "Rendering/CabotEngine/Graphics/ConstantBuffer.h"

namespace SlimeBoom
{
void SlimeSimulationComponent::SetCurrentSlimesCount(const int count)
{
    m_current_slime_count_ = std::min(count, kMaxSlimesCount);

    const auto group_count = (m_current_slime_count_ + 64 - 1) / 64;
    if (m_slime_simulation_shader_ != nullptr)
        m_slime_simulation_shader_->SetGroupCountX(group_count);

    if (m_slime_count_buffer_ != nullptr)
        m_slime_count_buffer_->UpdateBuffer(&m_current_slime_count_);

    if (m_slime_renderer_ != nullptr)
        m_slime_renderer_->instance_count = m_current_slime_count_;
}

void SlimeSimulationComponent::OnStart()
{
    m_slime_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(SlimeData), kMaxSlimesCount);
    m_slime_buffer_->CreateBuffer();

    m_damage_buffer_ = std::make_shared<engine::ByteAddressBuffer>(1);
    m_damage_buffer_->CreateBuffer();

    m_slime_count_buffer_ = std::make_shared<engine::ConstantBuffer>(sizeof(uint32_t));
    m_slime_count_buffer_->CreateBuffer();
    m_slime_count_buffer_->UpdateBuffer(&m_current_slime_count_);

    engine::GpuResourceManager::SetGlobalBuffer("slimes", m_slime_buffer_);
    engine::GpuResourceManager::SetGlobalBuffer("damage", m_damage_buffer_);
    engine::GpuResourceManager::SetGlobalBuffer("SlimeCount", m_slime_count_buffer_);
}

void SlimeSimulationComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Slime Simulation Shader", m_slime_simulation_shader_);

    if (engine::Gui::PropertyField("Slime Renderer", m_slime_renderer_))
    {
        m_slime_renderer_->instance_count = m_current_slime_count_;
    }
    
    if (engine::Gui::PropertyField("Current Slime Count", m_current_slime_count_))
        SetCurrentSlimesCount(m_current_slime_count_);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeSimulationComponent)
