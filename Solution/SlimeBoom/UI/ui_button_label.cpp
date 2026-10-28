#include "pch.h"
#include "ui_button_label.h"

#include "gui.h"

namespace ui
{
void UIButtonLabel::OnInspectorGui()
{
    engine::Gui::PropertyField("UI Button", m_ui_button_);
    engine::Gui::PropertyField("Text Renderer", m_text_renderer_);
    engine::Gui::PropertyField("Offset", m_offset_);
}

void UIButtonLabel::OnUpdate()
{
    const auto ui_button = m_ui_button_.CastedLock();
    const auto text_renderer = m_text_renderer_.CastedLock();
    if (ui_button == nullptr || text_renderer == nullptr)
        return;

    const auto font_data = text_renderer->font_data.CastedLock();
    if (font_data == nullptr || font_data->SpriteFont() == nullptr)
        return;

    const Vector2 text_size = font_data->SpriteFont()->MeasureString(text_renderer->string.c_str());
    text_renderer->origin = text_size * 0.5f;
    const auto rect = ui_button->GetRect();
    text_renderer->position = rect.pos + rect.size * m_offset_;
}
}

CEREAL_REGISTER_TYPE(ui::UIButtonLabel)
