#include "pch.h"
#include "player_component.h"
#include "engine_time.h"
#include "gui.h"
#include "input.h"
#include "../game_time.h"
#include "../StateMachine/comparison_condition.h"
#include "../Utils/random.h"
#include "ability/aura_ability.h"
#include "ability/slash_ability.h"
#include "States/avoid_state.h"
#include "States/dash_state.h"
#include "States/jump_state.h"
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
    
    UpdateJump();

    if (engine::Input::GetKeyDown(DirectX::Keyboard::LeftShift) && m_avoid_cooldown_timer_ <= 0.0f)
    {
        m_is_immune_ = true;
        m_avoid_timer_ = m_player_data_->GetAvoidTime();
        m_state_machine_->SetParameter("Avoid",true);
    }

    m_state_machine_->SetParameter("Jump", m_is_jumping_);
    m_state_machine_->SetParameter("Dead",IsDead());
}

void PlayerComponent::UpdateJump()
{
    const auto motor = m_motor_.lock();
    if (motor == nullptr || m_is_jumping_ || IsDead() || m_avoid_timer_ > 0.0f)
    {
        m_air_timer_ = 0.0f;
        return;
    }

    if (motor->IsGrounded())
    {
        m_air_timer_ = 0.0f;
        if (engine::Input::GetKeyDown(DirectX::Keyboard::Space))
        {
            motor->Jump();
            m_is_jumping_ = true;
        }
        return;
    }

    m_air_timer_ += engine::Time::GetDeltaTime();
    if (m_air_timer_ >= kFallStartTime)
        m_is_jumping_ = true;
}

void PlayerComponent::FinishJump()
{
    m_is_jumping_ = false;
    m_air_timer_ = 0.0f;
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

    auto random = Random(0, 100);
    if (random < m_player_data_->GetAvoidPercent())
        return;
    
    auto current_hp = m_player_data_->GetHp();
     current_hp -= damage;

    if (current_hp <= 0.0f)
        current_hp = 0.0f;
    
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
    engine::Gui::PropertyField("Dead Delay Time", m_dead_delay_time_);
}

void PlayerComponent::OnStart()
{
    m_dead_delay_timer_ = m_dead_delay_time_;
    const auto rigidbody = GameObject()->GetComponentInParent<engine::RigidbodyComponent>();
    if (rigidbody != nullptr)
    {
        auto motor = rigidbody->GameObject()->GetComponent<CharacterMotor>();
        if (motor == nullptr)
        {
            //CharacterMotorがないとRigidbodyが動かないので、付け忘れていたら追加する
            engine::Logger::Warn<PlayerComponent>("CharacterMotor is not found on '%s'. Added it at runtime.", rigidbody->GameObject()->Name().c_str());
            motor = rigidbody->GameObject()->AddComponent<CharacterMotor>();
        }
        m_motor_ = motor;
    }
    m_state_machine_ = std::make_shared<StateMachine>();

    const auto idle_state = std::make_shared<PlayAnimState>(m_animator_, "Idle", kIdleBlendSpeed);
    const auto dash_state = std::make_shared<DashState>("Dash", m_player_data_, m_animator_, m_camera_transform_, m_rotation_transform_);
    const auto avoid_state = std::make_shared<AvoidState>("Avoid", m_player_data_, m_animator_, m_camera_transform_, m_rotation_transform_,
                                                         [this](const Vector3& move)
                                                         {
                                                             if (const auto motor = m_motor_.lock())
                                                                 motor->AddMove(move);
                                                         },
                                                         rigidbody,
                                                         &m_on_avoid_);
    const auto dead_state = std::make_shared<PlayAnimState>(m_animator_, "Dead");
    const auto jump_state = std::make_shared<JumpState>("Jump", m_animator_, m_camera_transform_, m_rotation_transform_,
                                                        [this] { return m_motor_.lock(); },
                                                        [this] { FinishJump(); });

    const auto dash_condition = std::make_shared<ComparisonCondition<bool>>("Move", true, kOpType::kEqual);
    const auto dash_to_idle = std::make_shared<ComparisonCondition<bool>>("Move", true, kOpType::kNotEqual);
    
    const auto avoid_condition = std::make_shared<ComparisonCondition<bool>>("Avoid", true, kOpType::kEqual);
    const auto avoid_to_idle = std::make_shared<ComparisonCondition<bool>>("Avoid", true, kOpType::kNotEqual);

    const auto dead_condition = std::make_shared<ComparisonCondition<bool>>("Dead", true, kOpType::kEqual);

    const auto jump_condition = std::make_shared<ComparisonCondition<bool>>("Jump", true, kOpType::kEqual);
    const auto jump_to_idle = std::make_shared<ComparisonCondition<bool>>("Jump", true, kOpType::kNotEqual);

    m_state_machine_->SetDefaultState(idle_state);
    m_state_machine_->CreateTransition(idle_state, dash_condition, dash_state);
    m_state_machine_->CreateTransition(idle_state, avoid_condition, avoid_state);
    m_state_machine_->CreateTransition(idle_state, dead_condition, dead_state);
    m_state_machine_->CreateTransition(dash_state, dash_to_idle, idle_state);
    m_state_machine_->CreateTransition(dash_state, avoid_condition, avoid_state);
    m_state_machine_->CreateTransition(dash_state, dead_condition, dead_state);

    Transition avoid_to_idle_transition;
    avoid_to_idle_transition.AddCondition(avoid_to_idle);
    avoid_to_idle_transition.AddCondition(jump_to_idle);
    m_state_machine_->CreateTransition(avoid_state, avoid_to_idle_transition, idle_state);
    Transition avoid_to_jump_transition;
    avoid_to_jump_transition.AddCondition(avoid_to_idle);
    avoid_to_jump_transition.AddCondition(jump_condition);
    m_state_machine_->CreateTransition(avoid_state, avoid_to_jump_transition, jump_state);
    m_state_machine_->CreateTransition(avoid_state, dead_condition, dead_state);
    m_state_machine_->CreateTransition(idle_state, jump_condition, jump_state);
    m_state_machine_->CreateTransition(dash_state, jump_condition, jump_state);
    m_state_machine_->CreateTransition(jump_state, jump_to_idle, idle_state);
    m_state_machine_->CreateTransition(jump_state, dead_condition, dead_state);
    m_state_machine_->CreateTransition(jump_state, avoid_condition, avoid_state);

    m_player_damage_dispatcher_->AddOnTakeDamageListener([this](const float damage){TakeDamage(damage);});
    m_level_up_controller_->AddOnLevelUpListener([this](const int level){m_player_data_->SetLevel(level);});
}

void PlayerComponent::OnUpdate()
{
    if (GameTime::IsGameStopped())
        return;
    
    if (IsDead())
    {
        m_dead_delay_timer_ -= engine::Time::GetDeltaTime();
    }
    
    UpdateParameter();
    m_state_machine_->Update();
}

void PlayerComponent::OnFixedUpdate()
{
    m_state_machine_->FixedUpdate();
}

bool PlayerComponent::IsDeadEffectEnd()
{
    return m_dead_delay_timer_ <= 0.0f;
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

float PlayerComponent::GetAvoidCooldown() const
{
    return m_avoid_cooldown_;
}

float PlayerComponent::GetAvoidTimer() const
{
    return m_avoid_cooldown_timer_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerComponent)