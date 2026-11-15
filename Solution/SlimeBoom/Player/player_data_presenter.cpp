#include "pch.h"
#include "gui.h"
#include "player_data_presenter.h"

#include "engine_time.h"
#include "Rendering/material.h"
#include "../Utils/gpu_buffer.h"

void SlimeBoom::PlayerDataPresenter::ExecuteAttack()
{
    m_player_attack_shader_->Execute();
}

void SlimeBoom::PlayerDataPresenter::CreateAttackDataBuffer()
{
    m_max_attack_data_count_ = std::max(m_max_attack_data_count_, 1);
    m_attack_data_buffer_ = GpuBuffer::CreateUploadStructured(
        sizeof(PlayerAttackData), static_cast<uint32_t>(m_max_attack_data_count_), "player_attacks");

    engine::Material::SetGlobalBuffer("player_attacks", m_attack_data_buffer_.GetHandle());
}

void SlimeBoom::PlayerDataPresenter::OnInspectorGui()
{
    engine::Gui::PropertyField("Player Attack Shader", m_player_attack_shader_);
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
    engine::Gui::PropertyField("Position Y Offset", m_position_y_offset_);
    if (engine::Gui::PropertyField("Max AttackData Count", m_max_attack_data_count_))
        CreateAttackDataBuffer();
}

void SlimeBoom::PlayerDataPresenter::OnStart()
{
    engine::Material::SetGlobalInt("attack_data_count", 0);
    CreateAttackDataBuffer();
}

void SlimeBoom::PlayerDataPresenter::OnUpdate()
{
    engine::Material::SetGlobalVector3("player_pos", m_player_transform_->Position() + Vector3(0.0f, m_position_y_offset_, 0.0f));

    // 最大数を超えた分は捨てる
    const auto attack_data_count = std::min(static_cast<int>(m_attack_data_.size()), m_max_attack_data_count_);
    if (attack_data_count > 0 && m_attack_data_buffer_.IsValid())
    {
        // シェーダーは最大数までの攻撃を判定するので、使わない分は0(範囲0 = 当たらない)で埋めて送る。
        // 攻撃がないフレームは書き込まず、次の攻撃用ComputeShaderの実行までこのデータを残す
        m_attack_data_.resize(m_max_attack_data_count_);
        m_attack_data_buffer_.Update(m_attack_data_.data(), sizeof(PlayerAttackData) * m_attack_data_.size());
    }

    engine::Material::SetGlobalInt("attack_data_count", attack_data_count);
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