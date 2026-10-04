#include "pch.h"
#include "default_scene_creator.h"

#include "engine.h"
#include "scene_manager.h"
#include "Asset/Importer/fbx_importer.h"
#include "Audio/audio_listener_component.h"
#include "Components/camera_component.h"
#include "Rendering/model_importer.h"

namespace SlimeBoom
{
engine::Task DefaultSceneCreator::DelayLoadSceneTask()
{
    co_await engine::WaitForFrames(100);

    std::ifstream ifs("Resources/Scenes/Title/title.scene");
    std::stringstream ss;
    ss << ifs.rdbuf();
    engine::SceneManager::DeserializeScene(ss.str());
}

void DefaultSceneCreator::CreateDefaultScene()
{
    engine::Engine::coroutine.Start(DelayLoadSceneTask());
}
}