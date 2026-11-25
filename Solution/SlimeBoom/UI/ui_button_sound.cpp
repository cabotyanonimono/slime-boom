#include "pch.h"
#include "ui_button_sound.h"

#include "gui.h"
#include "ui_button.h"

namespace ui
{
void UIButtonSound::PlaySound(const engine::AssetPtr<engine::AudioSourceComponent>& sound)
{
    if (const auto source = sound.CastedLock())
        source->Play();
}

void UIButtonSound::OnInspectorGui()
{
    engine::Gui::PropertyField("Select Sound", m_select_sound_);
    engine::Gui::PropertyField("Press Sound", m_press_sound_);
}

void UIButtonSound::OnEnabled()
{
    const std::weak_ptr<UIButtonSound> weak_this = shared_from_base<UIButtonSound>();
    if (!m_selected_token_)
    {
        m_selected_token_ = UIButton::AddAnySelectedEventListener([weak_this]
        {
            if (const auto self = weak_this.lock())
                PlaySound(self->m_select_sound_);
        });
    }
    if (!m_pressed_token_)
    {
        m_pressed_token_ = UIButton::AddAnyPressedEventListener([weak_this]
        {
            if (const auto self = weak_this.lock())
                PlaySound(self->m_press_sound_);
        });
    }
}

void UIButtonSound::OnDisabled()
{
    if (m_selected_token_)
    {
        UIButton::RemoveAnySelectedEventListener(*m_selected_token_);
        m_selected_token_.reset();
    }
    if (m_pressed_token_)
    {
        UIButton::RemoveAnyPressedEventListener(*m_pressed_token_);
        m_pressed_token_.reset();
    }
}
}

CEREAL_REGISTER_TYPE(ui::UIButtonSound)
