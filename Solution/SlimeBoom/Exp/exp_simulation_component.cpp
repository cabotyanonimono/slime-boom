#include "pch.h"
#include "exp_simulation_component.h"

#include "exp_data.h"
#include "../compute_result.h"
#include "Rendering/gpu_resource_manager.h"

void SlimeBoom::ExpSimulationComponent::OnStart()
{
    m_exp_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(ExpData), kMaxExpBufferSize);
    m_exp_buffer_->CreateBuffer();

    m_exp_count_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(int), 1);
    m_exp_count_buffer_->CreateBuffer();

    engine::GpuResourceManager::SetGlobalBuffer("exps", m_exp_buffer_);
    engine::GpuResourceManager::SetGlobalBuffer("exp_count", m_exp_count_buffer_);
}

int SlimeBoom::ExpSimulationComponent::GetDeltaExpCount()
{
    return m_delta_exp_count_;
}

int SlimeBoom::ExpSimulationComponent::GetExpCount()
{
    return m_exp_count_;
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExpSimulationComponent)
