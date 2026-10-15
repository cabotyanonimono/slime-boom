#include "pch.h"
#include <cmath>
#include "clear_scene_controller.h"

#include "engine.h"
#include "engine_time.h"
#include "gui.h"
#include "input.h"

namespace SlimeBoom
{
engine::Task ClearSceneController::ShowClearSceneBackground() const
{
    m_background_->GameObject()->SetActive(true);
    float alpha = 0.0f;
    auto buff_data = m_background_->shared_material->shared_material_block->GetConstantBufferData("Alpha");

    if (buff_data == nullptr)
        co_return;
    
    while (alpha < 1.0f)
    {
        alpha += 0.01f;
        buff_data->SetFloatData("alpha", alpha);
        co_await engine::WaitForNextFrame();
    }

    co_return;
}

engine::Task ClearSceneController::ScoreBoardScaleChangeMotion() const
{
    float scale = 0.0f;

    Vector2 base_scale = m_score_board_->size_delta;
    m_score_board_->GameObject()->SetActive(true);
    m_text_ui_->SetActive(false);

    while (scale < 1.1f)
    {
        scale += 0.025f;
        m_score_board_->size_delta = base_scale * scale;
        co_await engine::WaitForNextFrame();
    }

    while (scale > 1.0f)
    {
        scale -= 0.025f;
        m_score_board_->size_delta = base_scale * scale;
        co_await engine::WaitForNextFrame();
    }
    m_game_over_text_->scale = 1.5f;
    m_clear_text_->scale = 1.5f;

    co_return;
}

engine::Task ClearSceneController::CountUpResultValues() const
{
    const float duration = (std::max)(m_count_up_duration_sec_, 0.01f);
    float elapsed = 0.0f;

    while (elapsed < duration)
    {
        elapsed += engine::Time::Get()->UnscaledDeltaTime();
        const float t = std::min(elapsed / duration, 1.0f);
        const float inv = 1.0f - t;
        const float eased = 1.0f - inv * inv * inv;

        m_eliminated_slime_count_->string = std::to_string(static_cast<int>(static_cast<float>(m_target_eliminate_count_) * eased));
        m_level_->string = std::to_string(static_cast<uint32_t>(static_cast<float>(m_target_level_) * eased));
        co_await engine::WaitForNextFrame();
    }

    m_eliminated_slime_count_->string = std::to_string(m_target_eliminate_count_);
    m_level_->string = std::to_string(m_target_level_);

    co_return;
}

engine::Task ClearSceneController::ShowClearScene()
{
    m_eliminated_slime_count_->string = "0";
    m_level_->string = "0";
    // ランクはカウントアップが終わるまで隠しておく
    if (const auto rank_text = m_rank_text_.CastedLock())
    {
        m_rank_base_scale_ = rank_text->scale;
        rank_text->GameObject()->SetActive(false);
    }

    co_await ShowClearSceneBackground();

    co_await ScoreBoardScaleChangeMotion();

    co_await CountUpResultValues();

    co_await ShowRank();

    StartIdleMotion();

    co_return;
}

const char* ClearSceneController::CalcRank(const int eliminate_count) const
{
    if (eliminate_count >= m_rank_s_threshold_)
        return "S";
    if (eliminate_count >= m_rank_a_threshold_)
        return "A";
    if (eliminate_count >= m_rank_b_threshold_)
        return "B";
    if (eliminate_count >= m_rank_c_threshold_)
        return "C";
    return "D";
}

engine::Task ClearSceneController::ShowRank() const
{
    const auto rank_text = m_rank_text_.CastedLock();
    if (!rank_text)
        co_return;

    rank_text->string = CalcRank(m_target_eliminate_count_);
    rank_text->scale = m_rank_base_scale_ * m_rank_pop_scale_;
    rank_text->GameObject()->SetActive(true);

    const float duration = (std::max)(m_rank_pop_duration_sec_, 0.01f);
    float elapsed = 0.0f;

    while (elapsed < duration)
    {
        elapsed += engine::Time::Get()->UnscaledDeltaTime();
        const float t = (std::min)(elapsed / duration, 1.0f);
        const float eased = 1.0f - (1.0f - t) * (1.0f - t);
        rank_text->scale = m_rank_base_scale_ * (m_rank_pop_scale_ + (1.0f - m_rank_pop_scale_) * eased);
        co_await engine::WaitForNextFrame();
    }

    rank_text->scale = m_rank_base_scale_;

    co_return;
}

void ClearSceneController::StartIdleMotion()
{
    m_idle_base_scale_ = m_clear_text_->scale;
    m_idle_elapsed_sec_ = 0.0f;
    m_is_idle_motion_active_ = true;
}

void ClearSceneController::OnInspectorGui()
{
    engine::Gui::PropertyField("Background Image", m_background_);
    engine::Gui::PropertyField("Score Board", m_score_board_);
    engine::Gui::PropertyField("Eliminated Slime Count", m_eliminated_slime_count_);
    engine::Gui::PropertyField("Level", m_level_);
    engine::Gui::PropertyField("Game Over Text", m_game_over_text_);
    engine::Gui::PropertyField("Clear Scene", m_clear_text_);
    engine::Gui::PropertyField("Text Ui", m_text_ui_);
    engine::Gui::PropertyField("Restart Scene Controller", m_restart_scene_transition_controller_);
    engine::Gui::PropertyField("Quit Scene Controller", m_quit_scene_transition_controller_);
    engine::Gui::PropertyField("Restart Button", m_restart_button_);
    engine::Gui::PropertyField("Quit Button", m_quit_button_);
    engine::Gui::PropertyField("Count Up Duration Sec", m_count_up_duration_sec_);
    engine::Gui::PropertyField("Idle Pulse Amplitude", m_idle_pulse_amplitude_);
    engine::Gui::PropertyField("Idle Pulse Period Sec", m_idle_pulse_period_sec_);
    engine::Gui::PropertyField("Rank Text", m_rank_text_);
    engine::Gui::PropertyField("Rank S Threshold", m_rank_s_threshold_);
    engine::Gui::PropertyField("Rank A Threshold", m_rank_a_threshold_);
    engine::Gui::PropertyField("Rank B Threshold", m_rank_b_threshold_);
    engine::Gui::PropertyField("Rank C Threshold", m_rank_c_threshold_);
    engine::Gui::PropertyField("Rank Pop Scale", m_rank_pop_scale_);
    engine::Gui::PropertyField("Rank Pop Duration Sec", m_rank_pop_duration_sec_);
}

void ClearSceneController::OnStart()
{
    m_restart_button_->AddEventListener([this]() { m_restart_scene_transition_controller_->Execute(); });
    m_quit_button_->AddEventListener([this]() { m_quit_scene_transition_controller_->Execute(); });
}

void ClearSceneController::OnEnabled()
{
    m_is_idle_motion_active_ = false;
    engine::Engine::coroutine.Start(ShowClearScene());
}

void ClearSceneController::OnUpdate()
{
    if (!m_is_idle_motion_active_)
        return;

    constexpr float kTwoPi = 6.28318530718f;
    const float period = (std::max)(m_idle_pulse_period_sec_, 0.01f);

    m_idle_elapsed_sec_ += engine::Time::Get()->UnscaledDeltaTime();
    m_idle_elapsed_sec_ = std::fmod(m_idle_elapsed_sec_, period);

    const float scale = m_idle_base_scale_ * (1.0f + m_idle_pulse_amplitude_ * std::sin(kTwoPi * m_idle_elapsed_sec_ / period));
    m_clear_text_->scale = scale;
    m_game_over_text_->scale = scale;
}

void ClearSceneController::OnDestroy()
{
    const auto buff_data = m_background_->shared_material->shared_material_block->GetConstantBufferData("Alpha");\
    if (buff_data == nullptr)
        return;
    buff_data->SetFloatData("alpha", 0.0f);
}

void ClearSceneController::SetClearSceneData(const ClearSceneData& clear_scene_data)
{
    m_target_eliminate_count_ = clear_scene_data.eliminate_count;
    m_target_level_ = clear_scene_data.player_data.level;
    m_game_over_text_->GameObject()->SetActive(!clear_scene_data.is_game_cleared);
    m_clear_text_->GameObject()->SetActive(clear_scene_data.is_game_cleared);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ClearSceneController)
