#include "pch.h"
#include "clear_scene_controller.h"

#include "engine.h"
#include "gui.h"
#include "input.h"

namespace SlimeBoom
{
engine::Task ClearSceneController::ShowClearSceneBackground() const
{
    m_background_->GameObject()->SetActive(true);
    float alpha = 0.0f;
    auto buff_data = m_background_->shared_material->shared_material_block->GetConstantBufferData("Alpha");
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

engine::Task ClearSceneController::ShowClearScene() const
{
    co_await ShowClearSceneBackground();

    co_await ScoreBoardScaleChangeMotion();
    
    co_return;
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
}

void ClearSceneController::OnStart()
{
    m_restart_button_->AddEventListener([this](){m_restart_scene_transition_controller_->Execute();});
    m_quit_button_->AddEventListener([this](){m_quit_scene_transition_controller_->Execute();});
}

void ClearSceneController::OnEnabled()
{
    engine::Engine::coroutine.Start(ShowClearScene());
}

void ClearSceneController::SetClearSceneData(const ClearSceneData& clear_scene_data) const
{
    m_eliminated_slime_count_->string = std::to_string(clear_scene_data.eliminate_count);
    m_level_->string = std::to_string(clear_scene_data.player_data.level);
    m_game_over_text_->GameObject()->SetActive(!clear_scene_data.is_game_cleared);
    m_clear_text_->GameObject()->SetActive(clear_scene_data.is_game_cleared);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ClearSceneController)