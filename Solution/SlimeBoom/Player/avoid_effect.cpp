#include "pch.h"
#include "avoid_effect.h"

#include "engine.h"
#include "Coroutine/coroutine_manager.h"

namespace SlimeBoom
{
engine::Task AvoidEffect::FovEffect()
{
    float timer = 0.0f;
    while (timer <= m_in_time_)
    {
        timer += engine::Time::GetDeltaTime();
        m_camera_component_->property.field_of_view = engine::Mathf::Lerp(m_base_fov_, m_avoid_fov_, timer / m_in_time_);
        co_await engine::WaitForNextFrame();
    }

    co_await engine::WaitForSeconds(m_player_data_->GetPlayerData().avoid_time);
    
    timer = 0.0f;
    while (timer <= m_out_time_)
    {
        timer += engine::Time::GetDeltaTime();
        m_camera_component_->property.field_of_view = engine::Mathf::Lerp(m_avoid_fov_, m_base_fov_, timer / m_out_time_);
        co_await engine::WaitForNextFrame();
    }
    
    co_return;
}

void AvoidEffect::OnInspectorGui()
{
    engine::Gui::PropertyField("In Time", m_in_time_);
    engine::Gui::PropertyField("Out Time", m_out_time_);
    engine::Gui::PropertyField("Cinema Brain", m_base_fov_);
    engine::Gui::PropertyField("From Camera", m_avoid_fov_);
    engine::Gui::PropertyField("Player", m_player_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
    engine::Gui::PropertyField("Camera", m_camera_component_);
}

void AvoidEffect::OnStart()
{
    m_player_->AddOnAvoidEvent([this] {engine::Engine::coroutine.Start(FovEffect());});
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AvoidEffect)