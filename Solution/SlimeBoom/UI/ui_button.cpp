#include "pch.h"
#include "ui_button.h"

#include "application.h"
#include "engine_time.h"
#include "gui.h"
#include "input.h"
#include "Components/rect_transform.h"

namespace ui
{
void UIButton::OnAwake()
{
    m_button_state_ = kButtonState::kNormal;
    if (auto image = m_image_.CastedLock())
    {
        auto texture_buffer_data = image->shared_material.CastedLock()->shared_material_block->GetTextureBufferData("_MainTex");
        texture_buffer_data->SetTexture(m_normal_texture_);
    }
}

void UIButton::OnInspectorGui()
{
    engine::Gui::PropertyField("Image", m_image_);
    engine::Gui::PropertyField("Normal", m_normal_texture_);
    engine::Gui::PropertyField("Select", m_selected_texture_);
    engine::Gui::PropertyField("Pressed", m_pressed_texture_);
}

void UIButton::OnUpdate()
{
    if (m_image_.Lock() == nullptr || m_normal_texture_.Lock() == nullptr || m_selected_texture_.Lock() == nullptr || m_pressed_texture_.Lock() == nullptr)
        return;

    UpdateMousePosition(engine::Input::MousePosition());

    m_rect_ = m_image_.CastedLock()->GameObject()->GetComponent<engine::Renderer2D>()->NormalizedRect();
    m_rect_.pos.y = -m_rect_.pos.y;

    const auto screen_size = Vector2(static_cast<float>(engine::Application::WindowWidth()), static_cast<float>(engine::Application::WindowHeight()));
    m_rect_.pos *= screen_size * 0.5f;
    m_rect_.size *= screen_size;

    m_rect_.pos += screen_size * 0.5f;
}

size_t UIButton::AddEventListener(const std::function<void()>& callback)
{
    return m_events_.AddListener(callback);
}

void UIButton::RemoveEventListener(const size_t token)
{
    m_events_.RemoveListener(token);
}

void UIButton::UpdateMousePosition(const Vector2 pos)
{
    const bool is_contains = m_rect_.Contains(pos);
    
    if (is_contains && !m_is_hovering_)
    {
        m_prev_update_frame_ = engine::Time::Get()->Frames() + 1;
        m_is_hovering_ = true;
    }
    
    if (!is_contains && m_is_hovering_)
    {
        m_prev_update_frame_ = engine::Time::Get()->Frames() + 1;
        m_is_hovering_ = false;
    }
}

bool UIButton::IsHovering() const
{
    return m_is_hovering_;
}

bool UIButton::IsHoveringStartedThisFrame() const
{
    return m_prev_update_frame_ == engine::Time::Get()->Frames() - 1 && m_is_hovering_;
}

bool UIButton::IsHoveringEndedThisFrame() const
{
    return m_prev_update_frame_ == engine::Time::Get()->Frames() - 1 && !m_is_hovering_;
}

engine::Rect UIButton::GetRect() const
{
    return m_rect_;
}

void UIButton::SetIsSelected(const bool selected)
{
    if (selected && m_button_state_ == kButtonState::kNormal)
    {
        m_button_state_ = kButtonState::kSelect;
        auto texture_buffer_data = m_image_->shared_material.CastedLock()->shared_material_block->GetTextureBufferData("_MainTex");
        texture_buffer_data->SetTexture(m_selected_texture_);
    }
    else if (!selected && m_button_state_ == kButtonState::kSelect)
    {
        m_button_state_ = kButtonState::kNormal;
        auto texture_buffer_data = m_image_->shared_material.CastedLock()->shared_material_block->GetTextureBufferData("_MainTex");
        texture_buffer_data->SetTexture(m_normal_texture_);
    }
    else if (!selected && m_button_state_ == kButtonState::kPress)
    {
        m_button_state_ = kButtonState::kNormal;
        auto texture_buffer_data = m_image_->shared_material.CastedLock()->shared_material_block->GetTextureBufferData("_MainTex");
        texture_buffer_data->SetTexture(m_normal_texture_);
    }
}

void UIButton::SetPressed(const bool pressed)
{
    if (pressed && m_button_state_ == kButtonState::kSelect)
    {
        m_button_state_ = kButtonState::kPress;
        auto texture_buffer_data = m_image_->shared_material.CastedLock()->shared_material_block->GetTextureBufferData("_MainTex");
        texture_buffer_data->SetTexture(m_pressed_texture_);
    }
    else if (!pressed && m_button_state_ == kButtonState::kPress)
    {
        m_button_state_ = kButtonState::kSelect;
        auto texture_buffer_data = m_image_->shared_material.CastedLock()->shared_material_block->GetTextureBufferData("_MainTex");
        texture_buffer_data->SetTexture(m_selected_texture_);
        m_events_.Invoke();
    }
}

bool UIButton::IsSelected() const
{
    return m_button_state_ == kButtonState::kSelect;
}

bool UIButton::IsPressed() const
{
    return m_button_state_ == kButtonState::kPress;
}
}

CEREAL_REGISTER_TYPE(ui::UIButton)