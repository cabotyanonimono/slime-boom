#include "pch.h"
#include "level_up_screen_manager.h"

#include "engine_time.h"
#include "gui.h"

void SlimeBoom::LevelUpScreenManager::ScreenStart() const
{
    engine::Input::SetMouseMode(engine::kMouseMode::kNormal);
    m_card_controller_->GenerateRandomCards();
    engine::Time::Get()->TimeScale(0.0f);
    m_level_up_ui_object_->SetActive(true);
    m_timer_text_->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
    m_hp_text_->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
    m_level_text_->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
}

void SlimeBoom::LevelUpScreenManager::ScreenEnd() const
{
    engine::Input::SetMouseMode(engine::kMouseMode::kLocked);
    engine::Time::Get()->TimeScale(1.0f);
    m_level_up_ui_object_->SetActive(false);
    m_timer_text_->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    m_hp_text_->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    m_level_text_->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
}

void SlimeBoom::LevelUpScreenManager::OnInspectorGui()
{
    engine::Gui::PropertyField("Card Controller", m_card_controller_);
    engine::Gui::PropertyField("Level Up UI", m_level_up_ui_object_);
    engine::Gui::PropertyField("Level Up Controller", m_level_up_controller_);
    engine::Gui::PropertyField("Timer Text", m_timer_text_);
    engine::Gui::PropertyField("Hp Text", m_hp_text_);
    engine::Gui::PropertyField("Level Text", m_level_text_);
    
    if (ImGui::CollapsingHeader("Level Card Button"))
    {
        for (auto &button : m_buttons_)
        {
            ImGui::PushID(&button);
            engine::Gui::PropertyField("Button", button);
            ImGui::PopID();
        }
    }
}

void SlimeBoom::LevelUpScreenManager::OnStart()
{
    m_level_up_controller_->AddOnLevelUpListener([this](int){ScreenStart();});

    for (auto button : m_buttons_)
    {
        button->AddEventListener([this](){ScreenEnd();});
    }
}

CEREAL_REGISTER_TYPE(SlimeBoom::LevelUpScreenManager)