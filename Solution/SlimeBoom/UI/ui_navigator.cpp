#include "pch.h"
#include "ui_navigator.h"

#include "gui.h"

namespace ui
{
bool UINavigator::IsPressed()
{
    return engine::Input::GetKeyDown(DirectX::Keyboard::Space) || engine::Input::GetKeyDown(DirectX::Keyboard::Enter) || (engine::Input::MouseMode() == engine::kMouseMode::kNormal && engine::Input::GetMouseLeft());
}

void UINavigator::SetCurrentPosition(const engine::AssetPtr<UIButton> &ui_button)
{
    m_current_button_.CastedLock()->SetIsSelected(false);
    m_current_button_ = ui_button;
    m_current_button_.CastedLock()->SetIsSelected(true);
}

void UINavigator::MovePosition(const kDirection dir)
{
    if (const auto ui_button = m_ui_button_map_[m_current_button_][dir];
        ui_button.Lock())
        SetCurrentPosition(ui_button);
}

void UINavigator::UpdatePositionByKey()
{
    if (engine::Input::GetKeyDown(DirectX::Keyboard::W) || engine::Input::GetKeyDown(DirectX::Keyboard::Up))
    {
        MovePosition(kDirection::kUp);
    }
    if (engine::Input::GetKeyDown(DirectX::Keyboard::A) || engine::Input::GetKeyDown(DirectX::Keyboard::Left))
    {
        MovePosition(kDirection::kLeft);
    }
    if (engine::Input::GetKeyDown(DirectX::Keyboard::D) || engine::Input::GetKeyDown(DirectX::Keyboard::Right))
    {
        MovePosition(kDirection::kRight);
    }
    if (engine::Input::GetKeyDown(DirectX::Keyboard::S) || engine::Input::GetKeyDown(DirectX::Keyboard::Down))
    {
        MovePosition(kDirection::kDown);
    }
}

void UINavigator::UpdatePositionByMouse()
{
    const auto casted_current_button = m_current_button_.CastedLock();
    if (casted_current_button->IsHoveringEndedThisFrame())
    {
        casted_current_button->SetIsSelected(false);
    }

    for (auto ui_button : std::ranges::views::keys(m_ui_button_map_))
    {
        if (ui_button.CastedLock()->IsHoveringStartedThisFrame())
        {
            SetCurrentPosition(ui_button);
            return;
        }
    }
}

void UINavigator::UpdateInput()
{
    m_current_button_.CastedLock()->SetPressed(IsPressed());
}

void UINavigator::OnInspectorGui()
{
    engine::Gui::PropertyField("First Select", m_current_button_);

    for (auto &[ui_button, neighbors] : m_ui_button_map_)
    {
        ImGui::PushID(ui_button.Lock().get());

        auto new_ui_button = m_ui_button_map_.find(ui_button)->first;
        if (engine::Gui::PropertyField("UI Button", new_ui_button))
        {
            auto it = m_ui_button_map_.find(ui_button);
            if (it == m_ui_button_map_.end())
                return;

            auto arg = std::move(it->second);
            m_ui_button_map_.erase(it);
            m_ui_button_map_.emplace(new_ui_button, std::move(arg));
            ImGui::PopID();
            return;
        }

        engine::Gui::PropertyField("Up", neighbors[kDirection::kUp]);
        engine::Gui::PropertyField("Left", neighbors[kDirection::kLeft]);
        engine::Gui::PropertyField("Right", neighbors[kDirection::kRight]);
        engine::Gui::PropertyField("Down", neighbors[kDirection::kDown]);

        ImGui::PopID();
    }

    if (engine::AssetPtr<UIButton> temp;
        engine::Gui::PropertyField("Add UI Button", temp))
    {
        m_ui_button_map_.try_emplace(temp, Neighbors());
    }
}

void UINavigator::OnUpdate()
{
    UpdatePositionByKey();
    UpdatePositionByMouse();
    UpdateInput();
}
}

CEREAL_REGISTER_TYPE(ui::UINavigator)