#include "pch.h"
#include "application.h"
#include "component_registry.h"
#include "engine.h"
#include "Windows.h"
#include "Editor/editor.h"
#include "SlimeBoom/default_scene_creator.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    const char* pixDllPath = "C:\\Program Files\\Microsoft PIX\\2601.15\\WinPixGpuCapturer.dll";

    HMODULE hPixModule = LoadLibraryA(pixDllPath);
    
    if (hPixModule == nullptr)
    {
        // パスが間違っているか、DLLが見つからない場合
        DWORD error = GetLastError();
        OutputDebugStringA("⚠️ PIX DLL のロードに失敗しました。パスを確認してください。\n");
    }
    else
    {
        OutputDebugStringA("✅ 正しいバージョンの PIX DLL のロードに成功！アタッチ可能です。\n");
    }
    
    SlimeBoom::ComponentRegistry::RegisterComponents();
    engine::Engine::on_init.AddListener([] {
        editor::Editor::Instance()->Attach();
    });
    engine::Engine::on_default_scene_creation.AddListener([] {
        SlimeBoom::DefaultSceneCreator::CreateDefaultScene();
    });
    engine::Application::Instance()->Run();

    return 0;
}