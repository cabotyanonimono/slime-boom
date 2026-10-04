#include "pch.h"
#include "gui.h"
#include "player_data_presenter.h"

#include "engine_time.h"
#include "Rendering/gpu_resource_manager.h"
#include "Rendering/render_pipeline.h"

void SlimeBoom::PlayerDataPresenter::ExecuteAttack()
{
    m_player_attack_shader_->Execute();
}

void SlimeBoom::PlayerDataPresenter::OnInspectorGui()
{
    engine::Gui::PropertyField("Player Attack Shader", m_player_attack_shader_);
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
    engine::Gui::PropertyField("Position Y Offset", m_position_y_offset_);
    if (engine::Gui::PropertyField("Max AttackData Count", m_max_attack_data_count_))
    {
        m_attack_data_buffer_ = std::make_shared<engine::StructuredBuffer>(
            sizeof(PlayerAttackData), m_max_attack_data_count_);
        m_attack_data_buffer_->CreateBuffer();

        engine::GpuResourceManager::SetGlobalBuffer("player_attacks", m_attack_data_buffer_);
    }
}

void SlimeBoom::PlayerDataPresenter::OnStart()
{
    m_attack_data_count_buffer_ = std::make_shared<engine::ConstantBuffer>(sizeof(int));
    m_attack_data_count_buffer_->CreateBuffer();

    engine::GpuResourceManager::SetGlobalBuffer("AttackDataCount", m_attack_data_count_buffer_);

    m_attack_data_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(PlayerAttackData),
                                                                       m_max_attack_data_count_);
    m_attack_data_buffer_->CreateBuffer();

    engine::GpuResourceManager::SetGlobalBuffer("player_attacks", m_attack_data_buffer_);
}

void SlimeBoom::PlayerDataPresenter::OnUpdate()
{
    engine::GpuResourceManager::SetGlobalVector("PlayerPos", m_player_transform_->Position() + Vector3(0.0f, m_position_y_offset_, 0.0f));

    if (m_lisner_token_ != -1)
    {
        engine::RenderPipeline::Instance()->on_cmd_list_open.RemoveListener(m_lisner_token_);
        m_lisner_token_ = -1;
    }

    if (!m_attack_data_.empty())
    {
        m_attack_data_.resize(m_max_attack_data_count_);
        m_lisner_token_ = engine::RenderPipeline::Instance()->on_cmd_list_open.AddListener([=]()
        {
            m_attack_data_buffer_->UpdateBuffer(m_attack_data_.data());
        });
    }

    const auto attack_data_count = static_cast<int>(m_attack_data_.size());
    m_attack_data_count_buffer_->UpdateBuffer(&attack_data_count);
    m_attack_data_.clear();

    for (auto it = m_attack_frames_.begin(); it != m_attack_frames_.end();)
    {
        auto delta_frames = engine::Time::Get()->Frames() - *it;
        if (delta_frames > 0)
        {
            it = m_attack_frames_.erase(it);
            ExecuteAttack();
        }
        else
        {
            ++it;
        }
    }
}

void SlimeBoom::PlayerDataPresenter::SetAttackData(const PlayerAttackData& player_attack_data)
{
    m_attack_frames_.emplace(engine::Time::Get()->Frames());
    m_attack_data_.emplace_back(player_attack_data);
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerDataPresenter)