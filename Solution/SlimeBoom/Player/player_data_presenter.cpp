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
    // 作り直したバッファには攻撃データが入っていないので、シェーダーが範囲外を読まないよう数を0にする
    engine::Material::SetGlobalInt("attack_data_count", 0);
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
    CreateAttackDataBuffer();
}

void SlimeBoom::PlayerDataPresenter::OnUpdate()
{
    engine::Material::SetGlobalVector3("player_pos", m_player_transform_->Position() + Vector3(0.0f, m_position_y_offset_, 0.0f));

    // 最大数を超えた分は捨てる
    const auto attack_data_count = std::min(static_cast<int>(m_attack_data_.size()), m_max_attack_data_count_);
    if (attack_data_count > 0 && m_attack_data_buffer_.IsValid())
    {
        // シェーダーはattack_data_count個までの攻撃を判定する。
        // 攻撃の判定は次のフレーム以降に実行されるので、攻撃がないフレームはデータも数も書き換えず、
        // 次の攻撃用ComputeShaderの実行までこのデータを残す
        // (Uploadバッファはフレームごとのコピーを持つので、GPUが読んでいる最中のデータは書き換わらない)
        m_attack_data_.resize(attack_data_count);
        m_attack_data_buffer_.Update(m_attack_data_.data(), sizeof(PlayerAttackData) * m_attack_data_.size());
        engine::Material::SetGlobalInt("attack_data_count", attack_data_count);
    }

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