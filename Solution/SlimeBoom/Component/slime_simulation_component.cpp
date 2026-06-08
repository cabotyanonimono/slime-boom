#include "pch.h"
#include "slime_simulation_component.h"
#include "Rendering/gpu_resource_manager.h"
#include "../Utils/random.h"
#include "Rendering/CabotEngine/Graphics/ConstantBuffer.h"

namespace SlimeBoom
{
void SlimeSimulationComponent::OnStart()
{
    if (m_slime_buffer_ == nullptr)
    {
        m_slime_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(SlimeData), m_num_slimes_);
        m_slime_buffer_->CreateBuffer();

        m_damage_buffer_ = std::make_shared<engine::ByteAddressBuffer>(1);
        m_damage_buffer_->CreateBuffer();

        m_num_slime_buffer_ = std::make_shared<engine::ConstantBuffer>(sizeof(uint32_t));
        m_num_slime_buffer_->CreateBuffer();
        m_num_slime_buffer_->UpdateBuffer(&m_num_slimes_);

        engine::GpuResourceManager::SetGlobalBuffer("slimes", m_slime_buffer_);
        engine::GpuResourceManager::SetGlobalBuffer("damage", m_damage_buffer_);
        engine::GpuResourceManager::SetGlobalBuffer("SlimeCount", m_num_slime_buffer_);
    }
}

void SlimeSimulationComponent::OnInspectorGui()
{
    if (engine::Gui::PropertyField("Num Slimes", m_num_slimes_))
    {
        m_num_slimes_ = static_cast<uint32_t>(m_num_slimes_);
        
        m_slime_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(SlimeData), m_num_slimes_);
        m_slime_buffer_->CreateBuffer();
        engine::GpuResourceManager::SetGlobalBuffer("slimes", m_slime_buffer_);

        m_num_slime_buffer_->UpdateBuffer(&m_num_slimes_);
    }
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeSimulationComponent)