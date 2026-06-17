#include "pch.h"
#include "player_component.h"
#include "gui.h"
#include "input.h"
#include "../StateMachine/comparison_condition.h"
#include "States/dash_state.h"
#include "States/play_anim_state.h"

namespace SlimeBoom
{
void PlayerComponent::UpdateParameter()
{
    Vector3 dir = Vector3::Zero;
    if (engine::Input::GetKey(DirectX::Keyboard::W))
        dir.z += 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::S))
        dir.z -= 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::A))
        dir.x += 1.0f;
    if (engine::Input::GetKey(DirectX::Keyboard::D))
        dir.x -= 1.0f;
    
    m_state_machine_->SetParameter("Move", dir.Length() > 0.0f);
}

void PlayerComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Hp", player_data->hp);
    engine::Gui::PropertyField("Speed", player_data->speed);
    engine::Gui::PropertyField("Attack Power", player_data->attack_power);
    engine::Gui::PropertyField("Attack Speed", player_data->attack_speed);
    
    engine::Gui::PropertyField("Rotation Transform", m_rotation_transform_);
    engine::Gui::PropertyField("Camera Transform", m_camera_transform_);
    engine::Gui::PropertyField("Animator", m_animator_);
}

void PlayerComponent::OnConstructed()
{
    player_data = std::make_shared<PlayerData>();
}

void PlayerComponent::OnStart()
{
    m_state_machine_ = std::make_shared<StateMachine>();

    const auto idle_state = std::make_shared<PlayAnimState>(m_animator_, "Idle");
    const auto dash_state = std::make_shared<DashState>("Dash", player_data, m_animator_, m_camera_transform_, m_rotation_transform_);

    const auto dash_condition = std::make_shared<ComparisonCondition<bool>>("Move", true, kOpType::kEqual);
    auto dash_to_idle = std::make_shared<ComparisonCondition<bool>>("Move", true, kOpType::kNotEqual);

    m_state_machine_->SetDefaultState(idle_state);
    m_state_machine_->CreateTransition(idle_state, dash_condition, dash_state);
    m_state_machine_->CreateTransition(dash_state, dash_to_idle, idle_state);
}

void PlayerComponent::OnUpdate()
{
    UpdateParameter();
    m_state_machine_->Update();
}

void PlayerComponent::OnFixedUpdate()
{
    m_state_machine_->FixedUpdate();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerComponent)