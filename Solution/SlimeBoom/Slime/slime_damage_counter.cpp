#include "pch.h"
#include "slime_damage_counter.h"

#include "Rendering/gpu_resource_manager.h"
#include "Rendering/render_pipeline.h"

namespace SlimeBoom
{
void SlimeDamageCounter::OnInspectorGui()
{
    engine::Gui::PropertyField("Compute Shader", m_compute_shader_);
}

void SlimeDamageCounter::OnStart()
{
}

void SlimeDamageCounter::OnUpdate()
{
    if (m_lisner_token_ != -1)
    {
        engine::RenderPipeline::Instance()->on_rendering.RemoveListener(m_lisner_token_);
        m_lisner_token_ = -1;
    }

    m_damage_buffer_ = engine::GpuResourceManager::GetGlobalBuffer("damage");

    if (m_damage_buffer_ == nullptr)
        return;
    
    if (m_damage_buffer_->FetchBufferData(&m_damage_counter_) || m_is_first_frame)
    {
        engine::Logger::Log<SlimeDamageCounter>("%d", m_damage_counter_);
        m_lisner_token_ = engine::RenderPipeline::Instance()->on_rendering.AddListener([&](){m_damage_buffer_->RequestReadBack();});
        m_is_first_frame = false;
    }
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeDamageCounter)