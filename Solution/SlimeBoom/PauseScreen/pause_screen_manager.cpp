#include "pch.h"
#include "pause_screen_manager.h"

#include "../game_time.h"
#include "engine.h"
#include "engine_time.h"
#include "gui.h"
#include "input.h"

namespace SlimeBoom
{
void PauseScreenManager::Pause()
{
    m_is_paused_ = true;
    engine::Input::SetMouseMode(engine::kMouseMode::kNormal);
    engine::Time::Get()->TimeScale(0.0f);
    m_pause_ui_object_->SetActive(true);

    if (const auto changer = m_text_color_changer_.CastedLock())
        changer->Apply();
}

engine::Task PauseScreenManager::Resume()
{
    m_is_resuming_ = true;
    co_await engine::WaitForNextFrame();
    engine::Input::SetMouseMode(engine::kMouseMode::kLocked);
    engine::Time::Get()->TimeScale(1.0f);
    m_pause_ui_object_->SetActive(false);

    if (const auto changer = m_text_color_changer_.CastedLock())
        changer->Restore();

    m_is_paused_ = false;
    m_is_resuming_ = false;
}

void PauseScreenManager::OnInspectorGui()
{
    engine::Gui::PropertyField("Pause UI", m_pause_ui_object_);
    engine::Gui::PropertyField("Resume Button", m_resume_button_);
    engine::Gui::PropertyField("Quit Button", m_quit_button_);
    engine::Gui::PropertyField("Quit Scene Controller", m_quit_scene_transition_controller_);
    engine::Gui::PropertyField("Text Color Changer", m_text_color_changer_);
}

void PauseScreenManager::OnStart()
{
    m_resume_button_->AddEventListener([this]()
    {
        if (m_is_paused_ && !m_is_resuming_)
            engine::Engine::coroutine.Start(Resume());
    });

    // ゲーム終了時のクリア画面のやめるボタンと同じく、TimeScaleは遷移時のGameManager::OnDestroyで戻る。
    m_quit_button_->AddEventListener([this]()
    {
        if (m_is_paused_ && !m_is_resuming_)
            m_quit_scene_transition_controller_->Execute();
    });
}

void PauseScreenManager::OnUpdate()
{
    if (!engine::Input::GetKeyDown(DirectX::Keyboard::Escape))
        return;

    if (m_is_resuming_)
        return;

    if (m_is_paused_)
    {
        engine::Engine::coroutine.Start(Resume());
        return;
    }

    // レベルアップ画面やクリア画面などで、すでに停止している間はポーズしない。
    if (GameTime::IsGameStopped())
        return;

    Pause();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PauseScreenManager)
