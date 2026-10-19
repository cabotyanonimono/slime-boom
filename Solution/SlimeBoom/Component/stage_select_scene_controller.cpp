#include "pch.h"
#include "stage_select_scene_controller.h"

#include "gui.h"

namespace SlimeBoom
{
void StageSelectSceneController::SelectStage(const kStage stage) const
{
    StageSelection::SetStage(stage);
    m_scene_transition_controller_->Execute();
}

void StageSelectSceneController::OnInspectorGui()
{
    engine::Gui::PropertyField("Scene Transition Controller", m_scene_transition_controller_);
    engine::Gui::PropertyField("Stage1 Button", m_stage1_button_);
    engine::Gui::PropertyField("Stage2 Button", m_stage2_button_);
    engine::Gui::PropertyField("Stage3 Button", m_stage3_button_);
}

void StageSelectSceneController::OnStart()
{
    m_stage1_button_->AddEventListener([this](){ SelectStage(kStage::kStage1); });
    m_stage2_button_->AddEventListener([this](){ SelectStage(kStage::kStage2); });
    m_stage3_button_->AddEventListener([this](){ SelectStage(kStage::kStage3); });
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::StageSelectSceneController)
