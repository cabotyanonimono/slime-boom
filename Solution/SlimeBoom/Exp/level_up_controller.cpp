#include "pch.h"
#include "level_up_controller.h"

#include "input.h"

int SlimeBoom::LevelUpController::CalcRequiredExp()
{
    return m_base_required_exp_ + static_cast<int>(static_cast<float>(m_required_exp_) * m_required_exp_up_rate_);
}

void SlimeBoom::LevelUpController::OnInspectorGui()
{
    engine::Gui::PropertyField("Current Exp", m_current_exp_);
    engine::Gui::PropertyField("Current Level Exp", m_current_level_exp_);
    engine::Gui::PropertyField("Current Level", m_current_level_);
    engine::Gui::PropertyField("Required Exp", m_required_exp_);
    engine::Gui::PropertyField("Base Required Exp", m_base_required_exp_);
    engine::Gui::PropertyField("Required Exp Up Rate", m_required_exp_up_rate_);
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
    engine::Gui::PropertyField("Player Data", m_player_data_);
}

void SlimeBoom::LevelUpController::OnStart()
{
    m_required_exp_ = m_base_required_exp_;
}

void SlimeBoom::LevelUpController::OnUpdate()
{
    const auto exp = *static_cast<int*>(m_compute_result_->GetValue("exp"));
    const auto delta_exp = exp - m_current_exp_;
    
    m_current_exp_ = exp;
    m_current_level_exp_ += delta_exp * m_player_data_->GetExpMultiplier();

    if (m_current_level_exp_ >= m_required_exp_)
    {
        ++m_current_level_;
        engine::Input::SetMouseMode(engine::kMouseMode::kNormal);
        m_on_level_up_.Invoke(m_current_level_);
        m_current_level_exp_ -= m_required_exp_;
        
        m_required_exp_ = CalcRequiredExp();
    }
}

size_t SlimeBoom::LevelUpController::AddOnLevelUpListener(const std::function<void(int)>& callback)
{
    return m_on_level_up_.AddListener(callback);
}

void SlimeBoom::LevelUpController::RemoveOnLevelUpListener(const size_t token)
{
    m_on_level_up_.RemoveListener(token);
}

int SlimeBoom::LevelUpController::GetCurrentExp() const
{
    return m_current_exp_;
}

int SlimeBoom::LevelUpController::GetCurrentLevelExp() const
{
    return m_current_level_exp_;
}

int SlimeBoom::LevelUpController::GetCurrentLevel() const
{
    return m_current_level_;
}

int SlimeBoom::LevelUpController::GetRequiredExp() const
{
    return m_required_exp_;
}

CEREAL_REGISTER_TYPE(SlimeBoom::LevelUpController)
