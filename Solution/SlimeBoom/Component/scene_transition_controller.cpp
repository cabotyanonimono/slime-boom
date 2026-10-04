#include "pch.h"
#include "scene_transition_controller.h"
#include "scene_manager.h"
#include "engine_time.h"
#include "scene.h"
#include "gui.h"

namespace SlimeBoom
{
void SceneTransitionController::ChangeScene() const
{
    std::ifstream ifs(m_scene_path_.c_str());
    std::stringstream ss;
    ss << ifs.rdbuf();
    engine::SceneManager::DeserializeScene(ss.str());
    const auto active_scene = engine::SceneManager::GetActiveScene();
    engine::SceneManager::DestroyScene(active_scene->Name());
}

void SceneTransitionController::SetScenePath(const std::string& scene_path)
{
    m_scene_path_ = scene_path;
}

void SceneTransitionController::Execute() const
{
    ChangeScene();
}

void SceneTransitionController::OnInspectorGui()
{
    engine::Gui::PropertyField("Scene Path", m_scene_path_);

    if (ImGui::Button("Execute"))
        Execute();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SceneTransitionController)
