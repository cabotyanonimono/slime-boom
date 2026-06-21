#include "pch.h"
#include "compute_result.h"

#include "Rendering/gpu_resource_manager.h"
#include "Rendering/render_pipeline.h"

namespace SlimeBoom
{
void ComputeResult::OnStart()
{
    m_compute_result_buffer_ = std::make_shared<engine::ByteAddressBuffer>(ComputeResultTypes::GetComputeResultBufferSize());
    m_compute_result_buffer_->CreateBuffer();

    engine::GpuResourceManager::SetGlobalBuffer("result", m_compute_result_buffer_);
}

void ComputeResult::OnUpdate()
{
    if (m_listener_token_ != -1)
    {
        engine::RenderPipeline::Instance()->on_rendering.RemoveListener(m_listener_token_);
        m_listener_token_ = -1;
    }
    
    if (m_compute_result_buffer_->FetchBufferData(m_result_.data()) || m_is_first_frame_)
    {
        m_listener_token_ = engine::RenderPipeline::Instance()->on_rendering.AddListener([&] {m_compute_result_buffer_->RequestReadBack();});
        m_is_first_frame_ = false;
    }
}

void *ComputeResult::GetValue(const std::string& name)
{
    return m_result_.data() + ComputeResultTypes::GetComputeResultOffset(name);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ComputeResult)