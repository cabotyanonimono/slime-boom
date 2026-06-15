#include "pch.h"
#include "gui.h"
#include "player_data_presenter.h"
#include "Rendering/gpu_resource_manager.h"
#include "Rendering/render_pipeline.h"

void SlimeBoom::PlayerDataPresenter::OnInspectorGui()
{
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
    if (engine::Gui::PropertyField("Max AttackData Count", m_max_attack_data_count_))
    {
        m_attack_data_buffer_ = std::make_shared<engine::StructuredBufferData>();
        m_attack_data_buffer_->SetCount(m_max_attack_data_count_);
        m_attack_data_buffer_->SetStride(sizeof(PlayerAttackData));

        engine::GpuResourceManager::SetGlobalBufferData("AttackData", m_attack_data_buffer_);
    }
}

void SlimeBoom::PlayerDataPresenter::OnStart()
{
    m_attack_data_count_buffer_ = std::make_shared<engine::ConstantBuffer>(sizeof(int));
    m_attack_data_count_buffer_->CreateBuffer();
    m_attack_data_count_buffer_->UpdateBuffer(&m_max_attack_data_count_);

    engine::GpuResourceManager::SetGlobalBuffer("AttackDataCount", m_attack_data_count_buffer_);

    m_attack_data_buffer_ = std::make_shared<engine::StructuredBufferData>();
    m_attack_data_buffer_->SetCount(m_max_attack_data_count_);
    m_attack_data_buffer_->SetStride(sizeof(PlayerAttackData));

    engine::GpuResourceManager::SetGlobalBufferData("player_attacks", m_attack_data_buffer_);
}

void SlimeBoom::PlayerDataPresenter::OnUpdate()
{
    engine::GpuResourceManager::SetGlobalVector("PlayerPos", m_player_transform_->Position());

    if (m_lisner_token_ != -1)
    {
        engine::RenderPipeline::Instance()->on_rendering.RemoveListener(m_lisner_token_);
        m_lisner_token_ = -1;
    }
    
    if (!m_attack_data_.empty())
    {
        m_attack_data_buffer_->SetData(m_attack_data_.data());
        m_lisner_token_ = engine::RenderPipeline::Instance()->on_rendering.AddListener([&](){engine::GpuResourceManager::SetGlobalBufferData("player_attacks", m_attack_data_buffer_);});
    }
    
    const auto attack_data_count = m_attack_data_.size();
    m_attack_data_count_buffer_->UpdateBuffer(&attack_data_count);

    m_attack_data_.clear();
}

void SlimeBoom::PlayerDataPresenter::SetAttackData(const PlayerAttackData& player_attack_data)
{
    m_attack_data_.emplace_back(player_attack_data);
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerDataPresenter)
