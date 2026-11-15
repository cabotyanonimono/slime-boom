#include "pch.h"
#include "compute_result.h"

#include <CaboRenderingSystem/render_engine.h>
#include <Rendering/crs_bridge.h>

#include "update_manager.h"
#include "Utils/gpu_buffer.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
void ComputeResult::SetSubscribed(const bool subscribed)
{
    if (m_is_subscribed_ == subscribed)
        return;

    m_is_subscribed_ = subscribed;
    if (subscribed)
        engine::UpdateManager::SubscribeRender(shared_from_base<::IRenderReceiver>());
    else
        engine::UpdateManager::UnsubscribeRender(shared_from_base<::IRenderReceiver>());
}

void ComputeResult::OnStart()
{
    // 結果は4byte(uint / int / float)単位で並んでいる
    const auto size = static_cast<uint32_t>(ComputeResultTypes::GetComputeResultBufferSize() * sizeof(uint32_t));
    m_compute_result_buffer_ = GpuBuffer::CreateReadWriteRaw(size, "result");

    engine::Material::SetGlobalBuffer("result", m_compute_result_buffer_.GetHandle());
    SetSubscribed(true);
}

void ComputeResult::OnEnabled()
{
    SetSubscribed(true);
}

void ComputeResult::OnDisabled()
{
    SetSubscribed(false);
}

void ComputeResult::OnUpdate()
{
    if (m_ticket_.id == 0)
        return;

    std::vector<uint8_t> data;
    if (!engine::CrsBridge::Crs().Device().TryGetReadback(m_ticket_, data))
        return;

    std::memcpy(m_result_.data(), data.data(), std::min(data.size(), sizeof(m_result_)));
    m_ticket_ = {};
}

void ComputeResult::OnDestroy()
{
    SetSubscribed(false);
}

void ComputeResult::Render()
{
    if (m_ticket_.id != 0 || !m_compute_result_buffer_.IsValid())
        return;

    m_ticket_ = engine::CrsBridge::Crs().Device().RequestReadback(m_compute_result_buffer_.GetHandle());
}

void *ComputeResult::GetValue(const std::string& name)
{
    return m_result_.data() + ComputeResultTypes::GetComputeResultOffset(name);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ComputeResult)
