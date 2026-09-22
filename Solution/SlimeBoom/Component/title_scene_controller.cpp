#include "pch.h"
#include "title_scene_controller.h"

#include "gui.h"
#include "scene_manager.h"
#include "input.h"

namespace SlimeBoom
{
void TitleSceneController::OnInspectorGui()
{
    engine::Gui::PropertyField("Play Button", m_play_button_);
    engine::Gui::PropertyField("Stop Button", m_stop_button_);
    if (ImGui::Button("Play"))
    {
        m_scene_transition_controller_.CastedLock()->GameObject()->SetActive(true);
    }
}

void TitleSceneController::OnStart()
{
    m_play_button_->AddEventListener([this](){m_scene_transition_controller_->Execute();});
    m_stop_button_->AddEventListener([this](){});
}

void TitleSceneController::OnUpdate()
{
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::TitleSceneController)