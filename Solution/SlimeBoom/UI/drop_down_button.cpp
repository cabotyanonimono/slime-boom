#include "pch.h"
#include "drop_down_button.h"
#include "gui.h"

namespace ui
{
void DropDownButton::CacheRectTransforms()
{
    for (auto& option : m_options_)
    {
        if (option.second == nullptr)
            option.second = option.first->GameObject()->GetComponent<engine::RectTransform>();
    }
}

void DropDownButton::UpdateButtonsPosition()
{
    m_open_button_->GameObject()->GetComponent<engine::RectTransform>()->anchored_position = m_button_position_;

    for (int i = 0; i < m_options_.size(); ++i)
    {
        auto& [button, rect_transform] = m_options_[i];
        if (rect_transform == nullptr)
            CacheRectTransforms();

        rect_transform->anchored_position = m_button_position_ + m_button_position_ * (i + 1); //開くボタンの分の +1
    }
}

void DropDownButton::OnInspectorGui()
{
    engine::Gui::PropertyField("Open Button", m_open_button_);

    for (int i = 0; i < m_options_.size(); ++i)
    {
        ImGui::PushID(i);
        engine::Gui::PropertyField("Option", m_options_[i]);
        ImGui::PopID();
    }

    if (ImGui::Button("Add Option"))
        m_options_.emplace_back();
    if (ImGui::Button("Remove Option"))
        m_options_.pop_back();
}

void DropDownButton::OnUpdate()
{
    if (m_open_button_->IsSelected())
        SetIsOpen(!m_is_open_);

    for (int i = 0; i < m_options_.size(); ++i)
    {
        if (m_options_[i].first->IsPressed())
            m_on_option_selected_event_.Invoke(i);
    }
}

void DropDownButton::SetButtonPosition(const Vector2 pos)
{
    for (int i = 0; i < m_options_.size(); ++i)
    {
        auto& [button, rect_transform] = m_options_[i];
        if (rect_transform == nullptr)
            CacheRectTransforms();

        if (rect_transform == nullptr)
            continue;

        rect_transform->anchored_position = pos;
    }
}

void DropDownButton::SetButtonSize(const Vector2 size)
{
    for (auto& option : m_options_)
    {
        if (option.second == nullptr)
            CacheRectTransforms();

        if (option.second == nullptr)
            continue;

        option.second->size_delta = size;
    }
}

void DropDownButton::SetIsOpen(const bool is_open)
{
    m_is_open_ = is_open;
    for (auto& option : m_options_ | std::views::keys)
    {
        option->GameObject()->SetActive(is_open);
    }
}

void DropDownButton::AddOnOptionSelectedEvent(const std::function<void(int)>& func)
{
    m_on_option_selected_event_.AddListener(func);
}
}

CEREAL_REGISTER_TYPE(ui::DropDownButton)
