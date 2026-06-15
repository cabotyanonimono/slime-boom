#include "pch.h"

#include "gui.h"
#include "player_attack_component.h"

#include "engine_time.h"
#include "input.h"
#include "Rendering/gpu_resource_manager.h"
#include "Rendering/render_pipeline.h"

void SlimeBoom::PlayerAttackComponent::FetchClosestSlimePos()
{
    if (m_lisner_token_ != -1)
    {
        engine::RenderPipeline::Instance()->on_rendering.RemoveListener(m_lisner_token_);
        m_lisner_token_ = -1;
    }

    if (m_closest_slime_buffer_->FetchBufferData(&m_closest_slime_pos_) || m_is_first_frame_)
    {
        m_lisner_token_ = engine::RenderPipeline::Instance()->on_rendering.AddListener([&](){m_closest_slime_buffer_->RequestReadBack();});
        m_is_first_frame_ = false;
    }
}

void SlimeBoom::PlayerAttackComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Sword Controller", m_sword_controller_);
    engine::Gui::PropertyField("Slash Effect Component", m_slash_effect_component_);
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
    engine::Gui::PropertyField("Attack Data Presenter", m_attack_data_presenter_);
    engine::Gui::PropertyField("CoolDown Time", m_cooldown_);
    engine::Gui::PropertyField("Arc", m_player_attack_data_.arc);
    engine::Gui::PropertyField("Size", m_player_attack_data_.size);
    engine::Gui::PropertyField("Damage", m_player_attack_data_.damage);
    engine::Gui::PropertyField("Power", m_player_attack_data_.power);
}

void SlimeBoom::PlayerAttackComponent::OnStart()
{
    m_closest_slime_buffer_ = std::make_shared<engine::StructuredBuffer>(sizeof(Vector3), 1);
    m_closest_slime_buffer_->CreateBuffer();
    engine::GpuResourceManager::SetGlobalBuffer("closest_slime", m_closest_slime_buffer_);
}

void SlimeBoom::PlayerAttackComponent::OnUpdate()
{
    FetchClosestSlimePos();

    m_cooldown_timer_ += engine::Time::GetDeltaTime();
    if (m_cooldown_timer_ >= m_cooldown_)
    {
        engine::Logger::Log("X : %f, Y : %f, Z : %f", m_closest_slime_pos_.x, m_closest_slime_pos_.y, m_closest_slime_pos_.z);
        
        m_cooldown_timer_ = 0.0f;
        m_player_attack_data_.angle = m_closest_slime_pos_ - m_player_transform_->Position();
        m_attack_data_presenter_->SetAttackData(m_player_attack_data_);
        m_slash_effect_component_->Play();
        m_sword_controller_->Play(m_closest_slime_pos_, m_player_attack_data_.size * 0.5f);
    }
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerAttackComponent)