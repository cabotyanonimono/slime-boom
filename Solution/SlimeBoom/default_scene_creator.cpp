#include "pch.h"
#include "default_scene_creator.h"
#include "scene_manager.h"
#include "Asset/Importer/fbx_importer.h"
#include "Audio/audio_listener_component.h"
#include "Components/camera_component.h"
#include "Rendering/model_importer.h"

namespace SlimeBoom
{
void DefaultSceneCreator::CreateDefaultScene()
{
    engine::SceneManager::CreateScene("Default Scene");
    auto go = engine::ModelImporter::LoadModelFromFBX("Slime/Slime.fbx");

    const auto camera = engine::Object::Instantiate<engine::GameObject>("Camera");
    camera->AddComponent<engine::CameraComponent>();
    camera->AddComponent<engine::AudioListenerComponent>();

    camera->Transform()->SetLocalPosition({0.0f, 0.85f, 1.5f});
}
}