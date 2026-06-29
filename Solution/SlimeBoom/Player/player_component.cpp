#include "pch.h"
#include "player_component.h"

#include "engine_time.h"
#include "gui.h"
#include "input.h"
#include "../StateMachine/comparison_condition.h"
#include "ability/slash_ability.h"
#include "States/dash_state.h"
#include "States/play_anim_state.h"

namespace SlimeBoom
{
void PlayerComponent::UpdateParameter() const
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

void PlayerComponent::TakeDamage(const int damage) const
{
    m_player_data_->hp -= damage;
}

void PlayerComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Rotation Transform", m_rotation_transform_);
    engine::Gui::PropertyField("Camera Transform", m_camera_transform_);
    engine::Gui::PropertyField("Animator", m_animator_);
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
    engine::Gui::PropertyField("Player Data Presenter", m_player_data_presenter_);
    engine::Gui::PropertyField("Player Sword Controller", m_player_sword_controller_);
    engine::Gui::PropertyField("Player Damage Dispatcher", m_player_damage_dispatcher_);
    
    engine::Gui::PropertyField("Hp", m_player_data_->hp);
    engine::Gui::PropertyField("Speed", m_player_data_->speed);
    engine::Gui::PropertyField("Attack Power", m_player_data_->attack_power);
    engine::Gui::PropertyField("Attack Speed", m_player_data_->attack_speed);
    engine::Gui::PropertyField("Exp", m_player_data_->exp);
    engine::Gui::PropertyField("Level", m_player_data_->level);
}

void PlayerComponent::OnConstructed()
{
    m_player_data_ = std::make_shared<PlayerData>();
}

void PlayerComponent::OnStart()
{
    m_state_machine_ = std::make_shared<StateMachine>();

    const auto idle_state = std::make_shared<PlayAnimState>(m_animator_, "Idle");
    const auto dash_state = std::make_shared<DashState>("Dash", m_player_data_, m_animator_, m_camera_transform_, m_rotation_transform_);

    const auto dash_condition = std::make_shared<ComparisonCondition<bool>>("Move", true, kOpType::kEqual);
    auto dash_to_idle = std::make_shared<ComparisonCondition<bool>>("Move", true, kOpType::kNotEqual);

    m_state_machine_->SetDefaultState(idle_state);
    m_state_machine_->CreateTransition(idle_state, dash_condition, dash_state);
    m_state_machine_->CreateTransition(dash_state, dash_to_idle, idle_state);

    m_ability_processer_.Attach(std::make_shared<SlashAbility>(m_compute_result_, m_player_data_, GameObject()->Transform(), m_player_data_presenter_, m_player_sword_controller_));

    m_player_damage_dispatcher_->AddOnTakeDamageListener([this](const int damage){TakeDamage(damage);});
}

void PlayerComponent::OnUpdate()
{
    UpdateParameter();
    m_state_machine_->Update();
    m_ability_processer_.Update(engine::Time::GetDeltaTime());
}

void PlayerComponent::OnFixedUpdate()
{
    m_state_machine_->FixedUpdate();
    m_ability_processer_.FixedUpdate();
}

const PlayerData& PlayerComponent::GetPlayerData() const
{
    return *m_player_data_.get();
}

void PlayerComponent::SetPlayerData(const PlayerData& data) const
{
    *m_player_data_ = data;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerComponent)