#include "pch.h"
#include "exp_simulation_component.h"

#include "exp_data.h"
#include "gui.h"
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
    engine::GpuResourceManager::SetGlobalFloat("AcquisitionRange", m_acquisition_range_);
}

void SlimeBoom::ExpSimulationComponent::OnInspectorGui()
{
    if (engine::Gui::PropertyField("Acquisition Range", m_acquisition_range_))
        SetAcquisitionRange(m_acquisition_range_);
}

int SlimeBoom::ExpSimulationComponent::GetDeltaExpCount() const
{
    return m_delta_exp_count_;
}

int SlimeBoom::ExpSimulationComponent::GetExpCount() const
{
    return m_exp_count_;
}

float SlimeBoom::ExpSimulationComponent::GetAcquisitionRange() const
{
    return m_acquisition_range_;
}

void SlimeBoom::ExpSimulationComponent::SetAcquisitionRange(const float range)
{
    m_acquisition_range_ = range;
    engine::GpuResourceManager::SetGlobalFloat("AcquisitionRange", m_acquisition_range_);
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExpSimulationComponent)
