#include "pch.h"
#include "scene_changer.h"

#include "scene.h"
#include "scene_manager.h"

void SceneChanger::OnInspectorGui()
{
    if (ImGui::Button("Change Scene"))
    {
        const auto scene = engine::SceneManager::GetActiveScene();
        engine::SceneManager::DestroyScene(scene->Name());
    }
}

CEREAL_REGISTER_TYPE(SceneChanger)