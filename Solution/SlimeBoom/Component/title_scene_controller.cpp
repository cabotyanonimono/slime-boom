#include "pch.h"
#include "title_scene_controller.h"

#include "gui.h"
#include "scene_manager.h"
#include "input.h"

namespace SlimeBoom
{
void TitleSceneController::ShowStageMenu(const bool show) const
{
    m_play_button_->GameObject()->SetActive(!show);
    m_stop_button_->GameObject()->SetActive(!show);
    m_stage1_button_->GameObject()->SetActive(show);
    m_stage2_button_->GameObject()->SetActive(show);
    m_stage3_button_->GameObject()->SetActive(show);
    m_back_button_->GameObject()->SetActive(show);
    m_stage_menu_background_->GameObject()->SetActive(show);
    m_title_logo_->GameObject()->SetActive(!show);
}

void TitleSceneController::SelectStage(const kStage stage) const
{
    StageSelection::SetStage(stage);
    m_scene_transition_controller_->Execute();
}

void TitleSceneController::OnInspectorGui()
{
    engine::Gui::PropertyField("Play Button", m_play_button_);
    engine::Gui::PropertyField("Stop Button", m_stop_button_);
    engine::Gui::PropertyField("Stage1 Button", m_stage1_button_);
    engine::Gui::PropertyField("Stage2 Button", m_stage2_button_);
    engine::Gui::PropertyField("Stage3 Button", m_stage3_button_);
    engine::Gui::PropertyField("Back Button", m_back_button_);
    engine::Gui::PropertyField("Stage Menu Background", m_stage_menu_background_);
    engine::Gui::PropertyField("Title Logo", m_title_logo_);
    if (ImGui::Button("Play"))
    {
        m_scene_transition_controller_.CastedLock()->GameObject()->SetActive(true);
    }
}

void TitleSceneController::OnStart()
{
    m_play_button_->AddEventListener([this](){ ShowStageMenu(true); });
    m_stop_button_->AddEventListener([this](){});
    m_stage1_button_->AddEventListener([this](){ SelectStage(kStage::kStage1); });
    m_stage2_button_->AddEventListener([this](){ SelectStage(kStage::kStage2); });
    m_stage3_button_->AddEventListener([this](){ SelectStage(kStage::kStage3); });
    m_back_button_->AddEventListener([this](){ ShowStageMenu(false); });
}

void TitleSceneController::OnUpdate()
{
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::TitleSceneController)
