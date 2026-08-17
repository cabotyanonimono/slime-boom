#include "pch.h"
#include "player_component.h"
#include "engine_time.h"
#include "gui.h"
#include "input.h"
#include "../StateMachine/comparison_condition.h"
#include "ability/aura_ability.h"
#include "ability/slash_ability.h"
#include "States/avoid_state.h"
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

    m_is_immune_ = false;
    m_state_machine_->SetParameter("Avoid", false);
    m_avoid_cooldown_timer_ -= engine::Time::GetDeltaTime();
    if (m_avoid_timer_ > 0.0f)
    {
        m_is_immune_ = true;
        m_avoid_cooldown_timer_ = m_avoid_cooldown_;
        m_avoid_timer_ -= engine::Time::GetDeltaTime();
        m_state_machine_->SetParameter("Avoid",true);
    }
    
    if (engine::Input::GetKeyDown(DirectX::Keyboard::Space) && m_avoid_cooldown_timer_ <= 0.0f)
    {
        m_is_immune_ = true;
        m_avoid_timer_ = m_player_data_->GetAvoidTime();
        m_state_machine_->SetParameter("Avoid",true);
    }

    m_state_machine_->SetParameter("Dead",IsDead());
}

bool PlayerComponent::IsDead() const
{
    return m_player_data_->GetHp() <= 0.0f;
}

void PlayerComponent::Dead()
{
    m_on_dead_.Invoke();
}

void PlayerComponent::TakeDamage(const float damage)
{
    if (m_is_immune_)
        return;

    auto current_hp = m_player_data_->GetHp();
     current_hp -= damage;
    m_player_data_->SetHp(current_hp);

    if (IsDead())
        Dead();
}

void PlayerComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Level Up Controller", m_level_up_controller_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
    engine::Gui::PropertyField("Rotation Transform", m_rotation_transform_);
    engine::Gui::PropertyField("Camera Transform", m_camera_transform_);
    engine::Gui::PropertyField("Animator", m_animator_);
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
    engine::Gui::PropertyField("Player Data Presenter", m_player_data_presenter_);
    engine::Gui::PropertyField("Player Sword Controller", m_player_sword_controller_);
    engine::Gui::PropertyField("Player Damage Dispatcher", m_player_damage_dispatcher_);
    engine::Gui::PropertyField("Aura Renderer", m_aura_renderer_);
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
    engine::Gui::PropertyField("Avoid Cooldown", m_avoid_cooldown_);
}

void PlayerComponent::OnStart()
{
    m_state_machine_ = std::make_shared<StateMachine>();

    const auto idle_state = std::make_shared<PlayAnimState>(m_animator_, "Idle", kIdleBlendSpeed);
    const auto dash_state = std::make_shared<DashState>("Dash", m_player_data_, m_animator_, m_camera_transform_, m_rotation_transform_);
    const auto avoid_state = std::make_shared<AvoidState>("Avoid", m_player_data_, m_animator_, m_camera_transform_, m_rotation_transform_, m_player_transform_, &m_on_avoid_);
    const auto dead_state = std::make_shared<PlayAnimState>(m_animator_, "Dead");

    const auto dash_condition = std::make_shared<ComparisonCondition<bool>>("Move", true, kOpType::kEqual);
    const auto dash_to_idle = std::make_shared<ComparisonCondition<bool>>("Move", true, kOpType::kNotEqual);
    
    const auto avoid_condition = std::make_shared<ComparisonCondition<bool>>("Avoid", true, kOpType::kEqual);
    const auto avoid_to_idle = std::make_shared<ComparisonCondition<bool>>("Avoid", true, kOpType::kNotEqual);

    const auto dead_condition = std::make_shared<ComparisonCondition<bool>>("Dead", true, kOpType::kEqual);

    m_state_machine_->SetDefaultState(idle_state);
    m_state_machine_->CreateTransition(idle_state, dash_condition, dash_state);
    m_state_machine_->CreateTransition(idle_state, avoid_condition, avoid_state);
    m_state_machine_->CreateTransition(idle_state, dead_condition, dead_state);
    m_state_machine_->CreateTransition(dash_state, dash_to_idle, idle_state);
    m_state_machine_->CreateTransition(dash_state, avoid_condition, avoid_state);
    m_state_machine_->CreateTransition(dash_state, dead_condition, dead_state);
    m_state_machine_->CreateTransition(avoid_state, avoid_to_idle, idle_state);
    m_state_machine_->CreateTransition(avoid_state, dead_condition, dead_state);

    m_player_damage_dispatcher_->AddOnTakeDamageListener([this](const float damage){TakeDamage(damage);});
    m_level_up_controller_->AddOnLevelUpListener([this](const int level){m_player_data_->SetLevel(level);});
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

size_t PlayerComponent::AddOnAvoidEvent(const std::function<void()>& callback)
{
    return m_on_avoid_.AddListener(callback);
}

void PlayerComponent::RemoveOnAvoidEvent(const size_t token)
{
    m_on_avoid_.RemoveListener(token);
}

size_t PlayerComponent::AddOnDeadEvent(const std::function<void()>& callback)
{
    return m_on_dead_.AddListener(callback);
}

void PlayerComponent::RemoveOnDeadEvent(const size_t token)
{
    m_on_dead_.RemoveListener(token);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerComponent)