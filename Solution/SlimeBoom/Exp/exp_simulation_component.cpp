#include "pch.h"
#include "exp_simulation_component.h"

#include "exp_data.h"
#include "gui.h"
#include "../compute_result.h"
#include "../Utils/gpu_buffer.h"
#include "Rendering/material.h"

void SlimeBoom::ExpSimulationComponent::OnStart()
{
    m_exp_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(ExpData), kMaxExpBufferSize, "exps");
    m_exp_count_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(int), 1, "exp_count");

    engine::Material::SetGlobalBuffer("exps", m_exp_buffer_.GetHandle());
    engine::Material::SetGlobalBuffer("exp_count", m_exp_count_buffer_.GetHandle());
    engine::Material::SetGlobalFloat("acquisition_range", m_acquisition_range_);
}

void SlimeBoom::ExpSimulationComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
    if (engine::Gui::PropertyField("Acquisition Range", m_acquisition_range_))
        SetAcquisitionRange(m_acquisition_range_);
}

void SlimeBoom::ExpSimulationComponent::OnUpdate()
{
    auto exp_count = *static_cast<int *>(m_compute_result_->GetValue("exp"));
    m_delta_exp_count_ = exp_count - m_exp_count_;
    m_exp_count_ = exp_count;
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
    engine::Material::SetGlobalFloat("acquisition_range", m_acquisition_range_);
}

CEREAL_REGISTER_TYPE(SlimeBoom::ExpSimulationComponent)
